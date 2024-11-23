#include "tree.h"
#include "moves.h"

void pickNMoves(t_node_move* all_moves, t_node* picked_nodes, int total_moves, int nb_of_move_to_pick) {
    /**
     * Picks N moves with decreasing probability each time a move is picked
     *
     * @param t_node_move* all_moves A list of all available moves for the rover
     * @param t_node* picked_nodes An array to store the selected moves as nodes
     * @param int total_moves The total number of available moves
     * @param int nb_of_move_to_pick The number of moves to pick
     * @return none
     */
    srand(time(NULL));
    int total_nb_of_move_available = 0;

    for (int i = 0; i < total_moves; i++) { // Count the total number of moves across all_moves
        total_nb_of_move_available += all_moves[i].available_move_count;
    }

    int picked_moves = 0;
    while (picked_moves < nb_of_move_to_pick && total_nb_of_move_available > 0) {
        int random_choice  = rand() % total_nb_of_move_available;
        int cumulative_sum  = 0;

        for (int i = 0; i < total_moves; i++) {
            cumulative_sum  += all_moves[i].available_move_count;
            if (random_choice  < cumulative_sum  && all_moves[i].available_move_count > 0) {
                picked_nodes[picked_moves].node_move = all_moves[i]; // Copy the picked move in the picked_nodes list
                picked_nodes[picked_moves].fixed_index = picked_moves; // Assign an index to the node that will never change
                picked_nodes[picked_moves].child_list = NULL;
                picked_nodes[picked_moves].child_count = 0;

                all_moves[i].available_move_count--; // Reduce the number of available move for the picked move
                total_nb_of_move_available--; // Reduce the total number of available moves
                picked_moves++;
                break;
            }
        }
    }
}

void check_slopes(t_node* node,t_map map){
    /**
      * Check for slopes effect and "conveyor belt" effect
      *
      * @param t_node* node The current node we are at
      * @param t_map map Mars map for the rover
      * @return none
      */
    while (isValidLocalisation(node->loc.pos, map.x_max, map.y_max) && map.soils[node->loc.pos.y][node->loc.pos.x] >= 5) {
        switch (map.soils[node->loc.pos.y][node->loc.pos.x]) { //Check for slopes and change rover's position
            case 5: // North
                node->loc.pos.y = node->loc.pos.y  - 1;
                break;
            case 6: //East
                node->loc.pos.x = node->loc.pos.x  + 1;
                break;
            case 7: //South
                node->loc.pos.y = node->loc.pos.y + 1;
                break;
            case 8: //West
                node->loc.pos.x = node->loc.pos.x -1;
                break;
            default:
                break;
        }
    }
}


t_node* createRoot(t_localisation rover) {
    /**
     * Creates the root of the tree
     *
     * @return t_node* A pointer to the root
     */
    t_node* root = (t_node*)malloc(sizeof(t_node));
    strcpy(root->node_move.name, "Root");
    root->fixed_index = -1;
    root->child_list = NULL;
    root->child_count = 0;
    root->loc.pos.x = rover.pos.x;
    root->loc.pos.y = rover.pos.y;
    root->loc.ori = rover.ori;
    return root;
}


void createTree(t_node* node, t_node* picked_nodes, int current_depth, int max_depth, int* path, int nb_of_picked_moves, t_map map, t_localisation parent_loc) {
    /**
     * Creates a tree recursively from the picked nodes with a max depth
     *
     * @param t_node* node The current node we are at
     * @param t_node* picked_nodes The list of picked moves
     * @param int current_depth The current depth in the tree
     * @param int max_depth The maximum depth of the tree
     * @param int* path An array with the node's path
     * @param int nb_of_picked_moves The number of picked moves at the start
     * @param t_map map Mars map for the rover
     * @param t_localisation parent_loc Location of the parent in order to move the child
     * @return none
     */
    node->loc = parent_loc; //Update the localisation of the node
    path[current_depth] = node->fixed_index; // Add the node index to the path

    if (node->fixed_index != -1) {
        if(map.soils[node->loc.pos.y][node->loc.pos.x] == ERG) { //If the case is an erg square
            if(node->node_move.move == F_20){
                node->loc = move(node->loc, F_10);
            }
            else if(node->node_move.move == F_30){
                node->loc = move(node->loc, F_20);
            }
        }else {
            node->loc=move(node->loc, node->node_move.move); // Update the location of created node if not at the root
        }
    }
    t_localisation loc_before_slopes = node->loc;
    check_slopes(node,map);

    int valid_move = 1;
    if (node->node_move.move == F_20 || node->node_move.move == F_30) { // Handle the in-between moves for F_20 and F_30
        int steps;
        if (node->node_move.move == F_20) {
            steps = 1;
        } else if (node->node_move.move == F_30) {
            steps = 2;
        }
        t_localisation temp_loc = loc_before_slopes;
        for (int step = 0; step < steps; step++) { // Move forward step times to check if we passed over a crevasse
            temp_loc = move(temp_loc, F_10);
            if (!isValidLocalisation(temp_loc.pos, map.x_max, map.y_max)) {
                valid_move = 0;
            }
        }
    }

    if ((!isValidLocalisation(node->loc.pos, map.x_max, map.y_max) || valid_move == 0)) {
            node->cost = 10000; // Set cost at 10 000 for invalid positions
    } else {
        node->cost = map.costs[node->loc.pos.y][node->loc.pos.x]; // Get the cost from the map
    }


    if (current_depth >= max_depth || node->cost >=10000 || node->cost ==0){
        return;
    }
    
    //Node creation if the tree isn't stopped
    int child_count = nb_of_picked_moves - current_depth;
    node->child_count = child_count;

    //If a case is a reg case
    if(map.soils[node->loc.pos.y][node->loc.pos.x] == REG ) {
        if(node->child_count > 4){
            node->child_count = 4;
        }
    }

    node->child_list = (t_node**)malloc(child_count * sizeof(t_node*));

    if(isValidLocalisation(node->loc.pos,map.x_max,map.y_max) == 1){

        int child_index = 0;
        for (int i = 0; i < nb_of_picked_moves && child_index < child_count; i++) {

            int current_index = picked_nodes[i].fixed_index;

            int used_node = 0;// Check if the current index has already been used before
            for (int j = 0; j <= current_depth; j++) {
                if (path[j] == current_index) {
                    used_node = 1;
                }
            }

            if (used_node!=1) { // If the node has not been used create the subtree of this node
                node->child_list[child_index] = (t_node*)malloc(sizeof(t_node));
                *node->child_list[child_index] = picked_nodes[i]; // Copy the picked node
                createTree(node->child_list[child_index], picked_nodes, current_depth + 1,
                           max_depth, path, nb_of_picked_moves, map, node->loc); // Recursively build the tree
                child_index++;
            }
        }
    }

}


void printTree(t_node* node, int level, t_map map) {
    /**
     * Prints the tree
     *
     * @param t_node* node The current node
     * @param int level The current level in the tree
     * @return none
     */

    if (node == NULL){
        return; // Stops if the node is NULL
    }

    for (int i = 0; i < level; i++) { // Print the indentation of the tree
        if (i == level - 1) {
            printf("%c%c%c",195,196,196);
        }
        else {
            printf("%c   ",179);
        }
    }

    if (node->fixed_index == -1){
        printf("(Root) (Cost: %d  [x:%d  y:%d  ori:%d  land:%d])\n", node->cost,node->loc.pos.x,
               node->loc.pos.y,node->loc.ori, map.soils[node->loc.pos.y][node->loc.pos.x]);
    }
    else{
        if (node->cost == 10000){
            printf("(%d) (Cost: %d  [x:%d  y:%d  ori:%d  land: Rover lost]  Move: %s)\n", node->fixed_index,
                   node->cost,node->loc.pos.x,node->loc.pos.y,node->loc.ori,node->node_move.name);
        }
        else {
            printf("(%d) (Cost: %d  [x:%d  y:%d  ori:%d  land:%d]  Move: %s)\n", node->fixed_index,
                   node->cost, node->loc.pos.x, node->loc.pos.y, node->loc.ori,
                   map.soils[node->loc.pos.y][node->loc.pos.x], node->node_move.name);
        }
    }

    for (int i = 0; i < node->child_count; i++) {
        printTree(node->child_list[i], level + 1, map); //Recursively print the child
    }
}

void findSmallestNode(t_node* node, t_node** current_path, int current_depth, int current_cost, t_node** best_path, int* best_path_length, int* best_cost_total) {
    /**
     * Recursively finds the smallest node at the end of the tree
     * If the 2 nodes has the same end cost, retuen the one with the smallest path cost
     *
     * @param t_node* node The current node
     * @param t_node** current_path The path from the root to the current node
     * @param int current_depth The depth of the current node
     * @param int current_cost The total cost of the path to the current node
     * @param t_node** best_path The best path found
     * @param int* best_path_length The length of the best path
     * @param int* best_cost The total cost of the best path
     * @return none
     */
    current_path[current_depth] = node;

    if (node->child_count == 0) {
        current_cost += node->cost;

        if (*best_cost_total == -1 || current_cost < *best_cost_total ||
            (current_cost == *best_cost_total && current_depth + 1 < *best_path_length)) { // Compare the current path cost with the best cost
            *best_cost_total = current_cost;
            *best_path_length = current_depth + 1;
            for (int i = 0; i <= current_depth; i++) { //copy the path
                best_path[i] = current_path[i];
            }
        }
        return;
    }

    // Add the current node's cost to the total cost of the path
    current_cost += node->cost;

    // Recursively call all the child nodes
    for (int i = 0; i < node->child_count; i++) {
        findSmallestNode(node->child_list[i], current_path, current_depth + 1, current_cost,
                         best_path, best_path_length, best_cost_total);
    }
}
