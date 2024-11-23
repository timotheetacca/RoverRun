#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "moves.h"

int main() {

// Initialisation of the random number to spawn the robot
    srand(time(NULL));
    rand();

// Creation of the map
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n\n", map.y_max, map.x_max);

    t_localisation rover;
// Chooses a location as long as it spawn on invalid spot
    do {
        rover.pos.x = (rand() % 2) * (rand() % map.x_max);
        rover.pos.y = (rand() % 2) * (rand() % map.y_max);
    } while (map.soils[rover.pos.y][rover.pos.x] == 4);


    if (rover.pos.x != 0 && rover.pos.x != map.x_max){
        if (rover.pos.y == 0){
            rover.ori = SOUTH;
        }
        else{
            rover.ori = NORTH;
        }
    }
    else {
        if (rover.pos.x == 0) {
            rover.ori = EAST;
        } else {
            rover.ori = WEST;
        }
    }


    printf("Rover spawned at (x: %d, y: %d) facing %d\n", rover.pos.x, rover.pos.y, rover.ori);


    // printf the costs and the soil, aligned left 5 digits (given functions)
    printSoils(map, rover);
    printCost(map);


    printf("\n");
    displayMap(map, rover);

    t_node_move moves[7] = {
            {22, "Move forward 10 m", F_10},
            {15, "Move forward 20 m",F_20},
            {7,  "Move forward 30 m",F_30},
            {7,  "Reverse 10 m",B_10},
            {21, "Turn left",T_LEFT},
            {21, "Turn right",T_RIGHT},
            {7,  "Turn back",U_TURN}
    };
    t_node picked_moves[9];
    int base_reached = 0;
    int total_move_count=100;

    while (total_move_count>=0 && base_reached==0) {
        pickNMoves(moves, picked_moves, 7, 9, total_move_count);

        /* Print picked moves
        printf("\nPicked Moves:\n");
        for (int i = 0; i < 9; i++) {
            printf("(%d)[%s]\n", picked_moves[i].fixed_index, picked_moves[i].node_move.name);
        }
        printf("\n");
        */

        t_node* root = createRoot(rover);
        int path[9] = { -1 };
        createTree(root, picked_moves, 0, 5, path, 9, map, rover);

        // Find the best path
        t_node* current_path[50];
        t_node* best_path[50];
        int best_path_length = 0;
        int best_cost = -1;

        findSmallestNode(root, current_path, 0, 0, best_path, &best_path_length, &best_cost);

        // Print the best path and its cost
        printf("\nBest path:\n");
        for (int i = 1; i < best_path_length; i++) {
            printf("(%d)[%s]\n", best_path[i]->fixed_index, best_path[i]->node_move.name);
            rover.pos.x = best_path[i]->loc.pos.x;
            rover.pos.y = best_path[i]->loc.pos.y;
            rover.ori = best_path[i]->loc.ori;

            if (map.soils[best_path[i]->loc.pos.y][best_path[i]->loc.pos.x] == BASE_STATION) {
                base_reached = 1;
            }
        }
        printf("\nTotal cost: %d\n\n\n\n\n", best_cost);


        // Display the updated map
        displayMap(map, rover);

        if (!base_reached) {
            printf("\nBase not reached. Picking new moves...\n");
        }
    }

    if (base_reached) {
        printf("Congrats! The rover reached the base\n");
    } else {
        printf("Game Over. The rover did not reach the base.\n");
    }

    return 0;
}