#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "map.h"
#include "loc.h"
#include "moves.h"

typedef struct {
    int available_move_count;
    char name[50];
    t_move move;
} t_node_move;

typedef struct s_node {
    t_node_move node_move;
    int fixed_index; //fixed index of the move in the pickNMoves() list
    struct s_node** child_list;
    int child_count;
    t_localisation loc;
    int cost;
} t_node;


void pickNMoves(t_node_move* all_moves, t_node* picked_nodes, int total_moves, int n, int total_move_count);
t_node* createRoot(t_localisation rover);
void check_slopes(t_node* node,t_map map);
void createTree(t_node* node, t_node* picked_nodes, int current_depth, int max_depth, int* path, int nb_picked_moves, t_map map, t_localisation parent_loc);
void printTree(t_node* node, int level, t_map map);
void findSmallestNode(t_node* node, t_node** current_path, int current_depth, int current_cost, t_node** best_path, int* best_path_length, int* best_cost_total);

#endif // UNTITLED1_TREE_H
