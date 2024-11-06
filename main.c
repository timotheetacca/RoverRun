#include <stdio.h>
#include "map.h"
#include "tree.h"

int main() {
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
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
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
    t_move moves[7] = {
            {22, "Move forward 10 m"},
            {15, "Move forward 20 m"},
            {7,  "Move forward 30 m"},
            {7,  "Reverse 10 m"},
            {21, "Turn left"},
            {21, "Turn right"},
            {7,  "Turn back"}
    };
    t_move picked_moves[9];
    pickNMoves(moves, picked_moves, 7, 9);

    t_node* root = createRoot();

    int path[9] = { -1 };

    createTree(root, picked_moves, 0, 5, path, 9);
    printTree(root, 0);
    return 0;
}