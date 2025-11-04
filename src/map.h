#ifndef MAP_H
#define MAP_H

#define F_NORTH 0
#define F_SOUTH 1
#define F_EAST 2
#define F_WEST 3

extern int player_x;
extern int player_y;
extern int player_facing;
//extern struct maptile *map_data;

struct maptile
{
    // Tile ID per wall. Up, down, left, right
    int u_tile;
    int d_tile;
    int l_tile;
    int r_tile;
    // Whether or not it is possible to leave the tile through this wall
    int u_passable;
    int d_passable;
    int l_passable;
    int r_passable;
    int solid;
};

struct maptile *allocate_map();

#endif