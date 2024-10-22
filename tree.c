#include "tree.h"

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



t_move* pickNineMoves(t_move* listOfMoves){
    //Function that returns a list of nine moves picked randomly

    t_move* result = malloc(9 * sizeof (t_move));

    // Initialize the random number
    double random_number;

    //Array that contains the probability of each move
    double listOfProbabilitiesOfEachMove[7];
    for (int i=0; i<=6; i++){
        listOfProbabilitiesOfEachMove[i] = listOfMoves[i].probability;
    }

    //Array that contains the cumulative probabilities. Example : [0.1, 0.15, 0.35, 0.50, 0.60, 0.85, 1.00]
    double cumulative_probabilities[7];
    cumulative_probabilities[0] = listOfMoves[0].probability;

    for(int i = 1; i <= 6; i++){
        cumulative_probabilities[i] = cumulative_probabilities[i-1] + listOfProbabilitiesOfEachMove[i];
    }

    //Index of the result array
    int index = 0;

    //For loop to pick the 9 moves
    for(int i = 0; i <= 8; i++){
        //Generate the random number
        random_number = (double) rand() / RAND_MAX;

        for(int j = 0; i <= 6; i++) {
            if (random_number <= cumulative_probabilities[j]) {
                result[index] = listOfMoves[j];
                printf("%s", result[index].name);
                break;

            }
        }

        index++;
    }

    return result;
}