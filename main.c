#include <stdio.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "moves.h"

int main() {
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);

    t_localisation rover;
    rover.pos.x=4;
    rover.pos.y=6;
    rover.ori=NORTH;
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            //Print the rover as "+" on the map

            if (rover.pos.x==j && rover.pos.y==i){
                printf("%c ", print_rover(rover.ori));
            }
            else{
                printf("%d ", map.soils[i][j]);
            }
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }


    displayMap(map);

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
    pickNMoves(moves, picked_moves, 7, 9);

    t_node* root = createRoot(rover);

    int path[9] = { -1 };

    createTree(root, picked_moves, 0, 5, path, 9, map, rover);
    printTree(root, 0);


    return 0;
}