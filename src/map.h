#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include "item.h"

#define F_NORTH 0
#define F_SOUTH 1
#define F_WEST 2
#define F_EAST 3

typedef struct
{
    // Tile ID per wall. Up, down, left, right
    char tile_wall[4];
    // Whether or not it is possible to leave the tile through this wall
    char solid_wall[4];
    char seethrough_wall[4];
    char slot[];
} maptile;

typedef struct
{
    unsigned char store[256];
    unsigned char itemgrid[80*80];
} itemtracker;

maptile *allocate_map();
void randomise_map(maptile *map);
int issolid(maptile *map, int x, int y);
int check_if_clear_and_solid(maptile *map, int px, int py, int dirp, int lr1, int fb1, int lr2, int fb2);
int get_tile_wall(maptile *map, int px, int py, int pdir, int lr, int fb, int direction);

#endif