//
// Created by flasque on 19/10/2024.
//

#ifndef UNTITLED1_MAP_H
#define UNTITLED1_MAP_H

#define COST_UNDEF 65535
#include "loc.h"

/**
 * @brief Enum for the possible soils of the map
 */
typedef enum e_soil
{
    BASE_STATION,
    PLAIN,
    ERG,
    REG,
    CREVASSE,
    SLOPE_NORTH,
    SLOPE_EAST,
    SLOPE_SOUTH,
    SLOPE_WEST
} t_soil;

/**
 * @brief Array of costs for the soils
 */
static const int _soil_cost[9] = {0, 1, 2, 4, 10000, 5 , 6, 7, 8};

/**
 * @brief Structure for the map

 */
typedef struct s_map
{
    t_soil  **soils;
    int     **costs;
    int     x_max;
    int     y_max;
} t_map;

/**
 * @brief Function to initialise the map from a file
 * @param filename : the name of the file
 * @return the map
 */
t_map createMapFromFile(char *);

/**
 * @brief Function to create a standard training map (11x11 with only plains and base station in the middle)
 * @param none
 * @return a standard map
 */
 t_map createTrainingMap();

/**
 * @brief display the map with characters
 * @param map : the map to display
 */
void displayMap(t_map, t_localisation rover);
char print_rover(int);
void printCost(t_map map);
void printSoils(t_map map, t_localisation rover);
void displayPathVisual(t_map map, int (*display_path_visual)[map.y_max], t_localisation *list_pos, int length_list);


#endif //UNTITLED1_MAP_H
