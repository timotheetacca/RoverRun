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

    t_move f1, f2, f3, r1, tl, tr, tb;
    f1.probability = 0.22;

    strcpy(f1.name,"Move forward 10 m\n");
    f2.probability = 0.15;
    strcpy(f2.name,"Move forward 20 m\n");
    f3.probability = 0.07;
    strcpy(f3.name,"Move forward 30 m\n");
    r1.probability = 0.07;
    strcpy(r1.name,"Reverse 10 m\n");
    tl.probability = 0.21;
    strcpy(tl.name,"Turn a quarter turn to the left\n");
    tr.probability = 0.21;
    strcpy(tr.name,"Turn a quarter turn to the right\n");
    tb.probability = 0.07;
    strcpy(tb.name,"Turning back\n");

    t_move * listOfMoves = malloc(7 * sizeof (t_move));
    listOfMoves[0] = f1;
    listOfMoves[1] = f2;
    listOfMoves[2] = f3;
    listOfMoves[3] = r1;
    listOfMoves[4] = tl;
    listOfMoves[5] = tr;
    listOfMoves[6] = tb;

    t_move *allMoves = pickNineMoves(listOfMoves);

    allMoves = pickNineMoves(listOfMoves);

    return 0;
}
