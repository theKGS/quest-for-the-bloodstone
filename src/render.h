#ifndef RENDER_H
#define RENDER_H

#define VIEW_WIDTH 176
#define VIEW_HEIGHT 120
#define VIEW_X (160 - 88)
#define VIEW_Y 32
#define VIEW_MID_X VIEW_X + VIEW_WIDTH / 2
#define VIEW_MID_Y VIEW_Y + VIEW_HEIGHT / 2

#define MAP_X 0
#define MAP_Y 0
#define MAP_SIGHT 5
#define MAP_TSIZE 2

#define OFFSET_CORRIDOR_1_X 24
#define OFFSET_CORRIDOR_1_Y 8
#define OFFSET_CORRIDOR_2_X 48
#define OFFSET_CORRIDOR_2_Y 20
#define OFFSET_CORRIDOR_3_X 64
#define OFFSET_CORRIDOR_3_Y 27

#include <allegro.h>
#include "map.h"

typedef struct
{
    BITMAP *bitmap;
    char x;
    char y;
} sprite_element;

typedef struct
{
    // Tile ID per wall. Up, down, left, right
    sprite_element *flat_tile_1;
    sprite_element *flat_tile_2;
    sprite_element *flat_tile_3;
    sprite_element *side_tile_closest_1;
    sprite_element *side_tile_closest_2;
    sprite_element *side_tile_closest_3;
    sprite_element *side_tile_closest_4;
    sprite_element *end_tile_1;
    sprite_element *end_tile_2;
    sprite_element *end_tile_3;
    sprite_element *end_tile_4;
} tileset;

sprite_element *load_sprite_element_from_atlas(BITMAP *atlas, FILE *file);
tileset *load_tileset_from_file(char *tst_fname, char *atlas_fname, PALETTE palette);
void destroy_sprite_element(sprite_element *element);
void destroy_tileset(tileset *tset);

void render_items(BITMAP *page, Item_reference item, Item_reference *store, int pdir, int steps);
void render_items_on_tile_closest(BITMAP *page, Maptile tile, Item_reference *store, int pdir);


void render_view(BITMAP *page, Map *map, tileset *tilesets, int vx, int vy, int px, int py, int pdir);

#endif