#include "tree.h"
#include <time.h>

t_node** createChildList(t_move* allMoves, int totalMoves, int newChildCount, int currentDepth, t_node* parentNode) {
    newChildCount = totalMoves - 1;
    if (newChildCount == 0 ) {
        return NULL;
    }

    t_node** newChildList = (t_node**)malloc(newChildCount * sizeof(t_node*));
    int j = 0;
    for (int i = 0; i < totalMoves; i++) {
        // Allocate memory for each child node
        newChildList[j] = (t_node*)malloc(sizeof(t_node));

        // Allocate memory for the child's path
        newChildList[j]->path = (int*)malloc((currentDepth + 1) * sizeof(int));

        // Copy the old path from parentNode to child
        for (int k = 0; k <= currentDepth; k++) {
            newChildList[j]->path[k] = parentNode->path[k];
        }

        // Add the current move index at the next depth level
        newChildList[j]->path[currentDepth] = i;

        newChildList[j]->move = allMoves[i];
        newChildList[j]->move_index = i;
        newChildList[j]->child_count = 0;
        newChildList[j]->child_list = NULL;

        j++;
    }
    return newChildList;
}


void createTree(t_node* parentNode, t_move* allMoves, int totalMoves, int currentDepth, int maxDepth) {
    if (currentDepth >= maxDepth || totalMoves == 0) {
        return;
    }
    if (parentNode->move_index != -1){
        parentNode->child_list = createChildList(allMoves, totalMoves, parentNode->child_count, currentDepth, parentNode);
    }
    for (int i = 0; i < parentNode->child_count; i++) {
        createTree(parentNode->child_list[i], allMoves, totalMoves - 1, currentDepth + 1, maxDepth);
    }
    free(parentNode->path);
}


t_node* createRootWithMoves(t_move* allMoves, int move_count) {
    t_node *root = (t_node *)malloc(sizeof(t_node));
    strcpy(root->move.name, "Root");
    root->move_index = -1;  // Root doesn't have a specific move
    root->child_count = move_count;
    root->child_list = (t_node **)malloc(move_count * sizeof(t_node *));
    root->path = (int* )malloc(sizeof(int));
    for (int i = 0; i < move_count; i++) {
        root->child_list[i] = (t_node *)malloc(sizeof(t_node));
        root->child_list[i]->path = (int* )malloc(2*sizeof(int));
        root->child_list[i]->move = allMoves[i];
        root->child_list[i]->move_index = i;
        root->child_list[i]->child_count = 0;
        root->child_list[i]->child_list = NULL;
        root->child_list[i]->path[0] = -1;
        root->child_list[i]->path[1] = root->child_list[i]->move_index;
    }

    return root;
}

void printTree(t_node* node, int level) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            printf("L__");
        } else {
            printf("|   ");
        }
    }
    printf("(%d)", node->move_index);
    printf("   (Path: ");
    for (int i=0; i < level+1;i++){
        printf("%d/", node->path[i]);
    }
    printf(")\n");
    for (int i = 0; i < node->child_count; i++) {
        printTree(node->child_list[i], level + 1);
    }
}

// Function to pick 9 random moves from a list
t_move* pickNineMoves(t_move* listOfMoves) {
    srand(time(NULL));
    t_move* result = malloc(9 * sizeof(t_move));
    int index = 0;
    int total_available_moves = 0;

    // Calculate total available moves
    for (int i = 0; i <= 6; i++) {
        total_available_moves += listOfMoves[i].available_move_count;
    }

    // Pick 9 moves randomly
    for (int i = 0; i < 9; i++) {
        if (total_available_moves == 0) {
            printf("No more available moves\n");
            break;
        }

        int random_choice = rand() % total_available_moves;
        int cumulative_sum = 0;

        for (int j = 0; j <= 6; j++) {
            cumulative_sum += listOfMoves[j].available_move_count;

            if (random_choice < cumulative_sum) {
                result[index] = listOfMoves[j];
                listOfMoves[j].available_move_count--;
                total_available_moves--;
                index++;
                break;
            }
        }
    }
    return result;
}