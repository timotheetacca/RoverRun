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
    //Update the localisation of the node
    node->loc = parent_loc;

    //Update the cost of the node
    node->cost = map.costs[node->loc.pos.y][node->loc.pos.x];

    path[current_depth] = node->fixed_index; // Add the node index to it's path
    if (node->fixed_index!=-1){
        updateLocalisation(&(node->loc), node->node_move.move);
    }

    if (current_depth >= max_depth){
        return;
    }

    int child_count = nb_of_picked_moves - current_depth;
    node->child_count = child_count;
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
                createTree(node->child_list[child_index], picked_nodes, current_depth + 1, max_depth, path, nb_of_picked_moves, map, node->loc); // Recursively build the tree
                child_index++;
            }
        }
    }

}


void printTree(t_node* node, int level) {
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
            printf("L__");
        } else {
            printf("|   ");
        }
    }
    printf("(%d) (cost: %d, move: %s)\n", node->fixed_index, node->cost, node->node_move.name); // Print the node index

    for (int i = 0; i < node->child_count; i++) {
        printTree(node->child_list[i], level + 1); //Recursively print the child
    }
}
