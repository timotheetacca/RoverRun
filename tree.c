#include "tree.h"
#include <time.h>

t_move * removeCurrentNodeFromList(t_move *list, int size, int indexToDelete) {
    if (indexToDelete < 0 || indexToDelete >= size) {
        return list;  // If index is invalid return the original list
    }
    // Free the memory of the element that we are deleting
    free(&list[indexToDelete]);
    // Shift the right elements to the left
    for (int j = indexToDelete; j < size - 1; j++) {
        list[j] = list[j + 1];
    }
    free(&list[size - 1]);

    return list;
}

t_move * createTree(t_move *list){
    // Number of elements =  Size of the whole array/Size of 1 array element
    int size = sizeof(list)/sizeof(list[0]);
    if (size == 1){
        return list;
    }
    else{
        for (int i=0; i <= (9-size); i++){
            createTree(removeCurrentNodeFromList(list,size, i));
        }
    }
}

t_move* pickNineMoves(t_move* listOfMoves) {
    // Function that returns a list of nine moves picked randomly, reducing availability

    srand(time(NULL));
    t_move* result = malloc(9 * sizeof(t_move));  // To store the picked moves

    int index = 0;
    int total_available_moves = 0;

    // Calculate total available moves
    for (int i = 0; i <= 6; i++) {
        total_available_moves += listOfMoves[i].available_move_count;
    }

    printf("Total available moves at the start: %d\n", total_available_moves);
    printf("-----------------------------------------\n");

    // Repeat 9 times to pick moves
    for (int i = 0; i < 9; i++) {
        if (total_available_moves == 0) {
            printf("No more available moves\n");
            break;
        }

        // Generate a random index within the available moves
        int random_choice = rand() % total_available_moves;
        int cumulative_sum = 0;

        printf("[Random number : %d]\n", random_choice);

        // Find the move based on the random index
        for (int j = 0; j <= 6; j++) {
            cumulative_sum += listOfMoves[j].available_move_count;

            if (random_choice < cumulative_sum) {
                result[index] = listOfMoves[j];  // Select the move
                listOfMoves[j].available_move_count--;  // Decrease the number of moves

                printf("result[%d]: %s(Moves remaining : %d)\n",
                       index + 1, result[index].name, listOfMoves[j].available_move_count);

                total_available_moves--;  // Decrease total available moves
                printf("Total available moves : %d\n", total_available_moves);
                printf("-----------------------------------------\n");
                break;

            }
        }

        index++;
    }

    return result;
}

