#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int available_move_count;
    char name[50];
} t_move;

typedef struct s_node {
    t_move move;
    int fixed_index; //fixed index of the move in the pickNMoves() list
    struct s_node** child_list;
    int child_count;
} t_node;

// Tree functions
void pickNMoves(t_move* all_moves, t_node* picked_nodes, int total_moves, int n);
t_node* createRoot();
void createTree(t_node* node, t_node* picked_nodes, int current_depth, int max_depth, int* path, int nb_picked_moves);
void printTree(t_node* node, int level);

#endif // UNTITLED1_TREE_H
