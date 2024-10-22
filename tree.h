#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif //UNTITLED1_TREE_H

typedef struct s_move
{
    double probability;
    char name[50];
} t_move ;

typedef struct s_node
{
    t_move move;
    int cost;
    struct s_node *left ;
    struct s_node *right ;
} t_node;

typedef struct s_tree
{
    t_node *root ;
} t_tree ;

t_move * removeCurrentNodeFromList(t_move *list, int size, int i);
t_move * createTree(t_move *list);
t_move* pickNineMoves(t_move* );

