#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_move {
    int available_move_count;
    char name[50];
} t_move;

typedef struct s_node {
    t_move move;
    struct s_node** child_list;
    int cost;
    int move_index;
    int child_count;
    int * path;
} t_node;

typedef struct s_tree {
    t_node* root;
} t_tree;

void createTree(t_node* parentNode, t_move* allMoves, int totalMoves, int currentDepth, int maxDepth);
t_node** createChildList(t_move* allMoves, int totalMoves, int newChildCount,int currentDepth,t_node* parentNode);
t_move* pickNineMoves(t_move* listOfMoves);
t_node* createRootWithMoves(t_move* allMoves, int move_count);
void printTree(t_node* node, int level);

#endif // UNTITLED1_TREE_H
