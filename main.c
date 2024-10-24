#include <stdio.h>
#include "map.h"
#include "tree.h"

int countNodes(t_node* node) {
    if (node == NULL) {
        return 0;
    }

    int count = 1;  // Count this node

    // Recursively count all children
    for (int i = 0; i < node->child_count; i++) {
        count += countNodes(node->child_list[i]);
    }

    return count;
}

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

    t_move f1, f2, f3, r1, tl, tr, tb;
    f1.available_move_count = 22;
    strcpy(f1.name,"Move forward 10 m\0");
    f2.available_move_count = 15;
    strcpy(f2.name,"Move forward 20 m\0");
    f3.available_move_count = 7;
    strcpy(f3.name,"Move forward 30 m\0");
    r1.available_move_count = 7;
    strcpy(r1.name,"Reverse 10 m\0");
    tl.available_move_count = 21;
    strcpy(tl.name,"Turn a quarter turn to the left\0");
    tr.available_move_count = 21;
    strcpy(tr.name,"Turn a quarter turn to the right\0");
    tb.available_move_count = 7;
    strcpy(tb.name,"Turning back\0");

    t_move * listOfMoves = malloc(7 * sizeof (t_move));
    listOfMoves[0] = f1;
    listOfMoves[1] = f2;
    listOfMoves[2] = f3;
    listOfMoves[3] = r1;
    listOfMoves[4] = tl;
    listOfMoves[5] = tr;
    listOfMoves[6] = tb;

    // Pick 9 random moves
    t_move* allMoves = pickNineMoves(listOfMoves);

    // Create root and tree
    t_node* treeRoot = createRootWithMoves(allMoves, 9);
    printTree(treeRoot, 0);

    // Build the tree
    printf("\n\nCreating children of Root...\n\n");
    createTree(treeRoot, allMoves, 9, 0, 5);

    // Print the tree structure
    printTree(treeRoot, 0);

    printf("Number of nodes :%d   -   Expected number of nodes : 18730", countNodes(treeRoot));

    return 0;
}
