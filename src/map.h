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
    Item_reference item_slots[4];
} Maptile;

typedef struct{
    Maptile *tiles;
    char w;
    char h;
    Item_reference *store;
} Map;

Maptile *allocate_map_tiles();
Map *allocate_map();
void randomise_map(Maptile *map);
int issolid(Maptile *map, int x, int y);
int get_tile_wall(Maptile *map, int px, int py, int pdir, int lr, int fb, int direction);
void drop_item(Item_reference *slot, Item_reference *store, unsigned char ref, unsigned char meta);

#endif