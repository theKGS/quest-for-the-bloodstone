#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#define F_NORTH 0
#define F_SOUTH 1
#define F_EAST 2
#define F_WEST 3

extern int player_x;
extern int player_y;
extern int player_facing;
// extern struct maptile *map_data;

typedef struct
{
    // Tile ID per wall. Up, down, left, right
    char u_tile;
    char d_tile;
    char l_tile;
    char r_tile;
    // Whether or not it is possible to leave the tile through this wall
    char u_passable;
    char d_passable;
    char l_passable;
    char r_passable;

    char u_block_vision;
    char d_block_vision;
    char l_block_vision;
    char r_block_vision;

    char solid;
} maptile;

maptile *allocate_map();
int issolid(maptile *map, int x, int y);
int check_if_clear_and_solid(maptile *map, int px, int py, int dirp, int lr1, int fb1, int lr2, int fb2);

#endif