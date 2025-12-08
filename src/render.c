#define DEBUGMODE
#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "render.h"
#include "map.h"
#include "utils.h"

void draw_sprite_element(BITMAP *page, sprite_element *selm)
{
     draw_sprite(page, selm->bitmap, VIEW_X + selm->x, VIEW_Y + selm->y);
}

void draw_sprite_element_f(BITMAP *page, sprite_element *selm)
{
     draw_sprite_h_flip(page, selm->bitmap, VIEW_X + 176 - selm->x - selm->bitmap->w, VIEW_Y + selm->y);
}

void draw_sprite_element_flat(BITMAP *page, sprite_element *s_elm, int i)
{
     int vx = s_elm->x + s_elm->bitmap->w * i;
     int x = VIEW_X + s_elm->x + s_elm->bitmap->w * i;
     int y = VIEW_Y + s_elm->y;

     if (vx < 0)
     {
          blit(s_elm->bitmap, page, -vx, 0, x - vx, y, s_elm->bitmap->w + vx, s_elm->bitmap->h);
     }
     else if (s_elm->x + s_elm->bitmap->w * (i + 1) >= VIEW_WIDTH)
     {
          blit(s_elm->bitmap, page, 0, 0, x, y, (VIEW_X + VIEW_WIDTH) - x, s_elm->bitmap->h);
     }
     else
     {
          draw_sprite(page, s_elm->bitmap, x, y);
     }
}

void render_view(BITMAP *page, Map *map, tileset *tilesets, int vx, int vy, int px, int py, int pdir)
{
     Maptile *tiles = map->tiles;

     // L 3

     if (get_tile_wall(tiles, px, py, pdir, 0, 3, prev_facing[pdir]))
          draw_sprite_element(page, tilesets->side_tile_closest_4);

     if (get_tile_wall(tiles, px, py, pdir, 0, 3, next_facing[pdir]))
          draw_sprite_element_f(page, tilesets->side_tile_closest_4);

     // L 2

     for (int i = -2; i < 3; i++)
     {
          if (get_tile_wall(tiles, px, py, pdir, i, 2, pdir))
               draw_sprite_element_flat(page, tilesets->flat_tile_3, i);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 2, prev_facing[pdir]))
          draw_sprite_element(page, tilesets->side_tile_closest_3);

     if (get_tile_wall(tiles, px, py, pdir, 0, 2, next_facing[pdir]))
          draw_sprite_element_f(page, tilesets->side_tile_closest_3);

     // Special end pieces one step away
     if (get_tile_wall(tiles, px, py, pdir, 0, 1, prev_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 2, prev_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, -1, 1, pdir) != 0)
     {
          draw_sprite_element(page, tilesets->end_tile_3);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 1, next_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 2, next_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 1, 1, pdir) != 0)
     {
          draw_sprite_element_f(page, tilesets->end_tile_3);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 1, prev_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 2, prev_facing[pdir]) != 0 &&
         get_tile_wall(tiles, px, py, pdir, -1, 1, pdir) == 0)
     {
          draw_sprite_element(page, tilesets->end_tile_4);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 1, next_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 2, next_facing[pdir]) != 0 &&
         get_tile_wall(tiles, px, py, pdir, 1, 1, pdir) == 0)
     {
          draw_sprite_element_f(page, tilesets->end_tile_4);
     }

     // L 1
     for (int i = -1; i < 2; i++)
     {
          if (get_tile_wall(tiles, px, py, pdir, i, 1, pdir))
               draw_sprite_element_flat(page, tilesets->flat_tile_2, i);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 1, prev_facing[pdir]))
          draw_sprite_element(page, tilesets->side_tile_closest_2);

     if (get_tile_wall(tiles, px, py, pdir, 0, 1, next_facing[pdir]))
          draw_sprite_element_f(page, tilesets->side_tile_closest_2);

     // Special end pieces
     if (get_tile_wall(tiles, px, py, pdir, 0, 0, prev_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 0, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, prev_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, -1, 0, pdir) != 0)
     {
          draw_sprite_element(page, tilesets->end_tile_1);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 0, next_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 0, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, next_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 1, 0, pdir) != 0)
     {
          draw_sprite_element_f(page, tilesets->end_tile_1);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 0, prev_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 0, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, prev_facing[pdir]) != 0 &&
         get_tile_wall(tiles, px, py, pdir, -1, 0, pdir) == 0)
     {
          draw_sprite_element(page, tilesets->end_tile_2);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 0, next_facing[pdir]) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 0, pdir) == 0 &&
         get_tile_wall(tiles, px, py, pdir, 0, 1, next_facing[pdir]) != 0 &&
         get_tile_wall(tiles, px, py, pdir, 1, 0, pdir) == 0)
     {
          draw_sprite_element_f(page, tilesets->end_tile_2);
     }

     render_items_on_tile_distant(page, tiles[40 * py + px], map->store, pdir, 0, 0);

     // L 0

     for (int i = -1; i < 2; i++)
     {
          if (get_tile_wall(tiles, px, py, pdir, i, 0, pdir))
               draw_sprite_element_flat(page, tilesets->flat_tile_1, i);
     }

     if (get_tile_wall(tiles, px, py, pdir, 0, 0, prev_facing[pdir]))
          draw_sprite_element(page, tilesets->side_tile_closest_1);

     if (get_tile_wall(tiles, px, py, pdir, 0, 0, next_facing[pdir]))
          draw_sprite_element_f(page, tilesets->side_tile_closest_1);

     render_items_on_tile_closest(page, tiles[40 * py + px], map->store, pdir);

     /*const int p1 = 50;
     const int p2 = -30;

     line(page, VIEW_X + p1, VIEW_Y + 103, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));
     line(page, VIEW_X + 176 - p1, VIEW_Y + 103, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));
     line(page, VIEW_X + p1, VIEW_Y + 119, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));
     line(page, VIEW_X + 176 - p1, VIEW_Y + 119, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));

     line(page, VIEW_X + p2, VIEW_Y + 103, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));
     line(page, VIEW_X + 176 - p2, VIEW_Y + 103, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));
     line(page, VIEW_X + p2, VIEW_Y + 119, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));
     line(page, VIEW_X + 176 - p2, VIEW_Y + 119, VIEW_X + 87, VIEW_Y + 39, makecol(0, 255, 255));*/
}

tileset *load_tileset_from_file(char *tst_fname, char *atlas_fname, PALETTE palette)
{
     FILE *file = fopen(tst_fname, "r");

     if (!file)
     {
          set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          allegro_message("Failed to load tileset definition file %s\n", tst_fname);
          allegro_exit();
     }

     tileset *tset = malloc(sizeof(tileset));

     BITMAP *atlas = load_tga(atlas_fname, palette);

     if (!atlas)
     {
          set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          allegro_message("Failed to load tileset atlas file %s\n", atlas_fname);
          allegro_exit();
     }

     tset->flat_tile_1 = load_sprite_element_from_atlas(atlas, file);
     tset->flat_tile_2 = load_sprite_element_from_atlas(atlas, file);
     tset->flat_tile_3 = load_sprite_element_from_atlas(atlas, file);
     tset->side_tile_closest_1 = load_sprite_element_from_atlas(atlas, file);
     tset->side_tile_closest_2 = load_sprite_element_from_atlas(atlas, file);
     tset->side_tile_closest_3 = load_sprite_element_from_atlas(atlas, file);
     tset->side_tile_closest_4 = load_sprite_element_from_atlas(atlas, file);
     tset->end_tile_1 = load_sprite_element_from_atlas(atlas, file);
     tset->end_tile_2 = load_sprite_element_from_atlas(atlas, file);
     tset->end_tile_3 = load_sprite_element_from_atlas(atlas, file);
     tset->end_tile_4 = load_sprite_element_from_atlas(atlas, file);
     destroy_bitmap(atlas);
     fclose(file);
     return tset;
}

sprite_element *load_sprite_element_from_atlas(BITMAP *atlas, FILE *file)
{
     char line[10];
     int posx = 5;
     int posy = 5;
     int width = 0;
     int height = 0;
     int x = 0;
     int y = 0;

     fgets(line, 10, file); // Read the first line of of each block and ignore it.

     fgets(line, 10, file);
     posx = atoi(line);
     fgets(line, 10, file);
     posy = atoi(line);

     fgets(line, 10, file);
     width = atoi(line);
     fgets(line, 10, file);
     height = atoi(line);

     fgets(line, 10, file);
     x = atoi(line);
     fgets(line, 10, file);
     y = atoi(line);

     sprite_element *element = malloc(sizeof(sprite_element));
     element->bitmap = create_bitmap(width, height);
     blit(atlas, element->bitmap, posx, posy, 0, 0, width, height);
     element->x = x;
     element->y = y;

     return element;
}

void destroy_sprite_element(sprite_element *element)
{
     destroy_bitmap(element->bitmap);
     free(element);
}

void destroy_tileset(tileset *tset)
{
     destroy_sprite_element(tset->flat_tile_1);
     destroy_sprite_element(tset->flat_tile_2);
     destroy_sprite_element(tset->flat_tile_3);
     destroy_sprite_element(tset->side_tile_closest_1);
     destroy_sprite_element(tset->side_tile_closest_2);
     destroy_sprite_element(tset->side_tile_closest_3);
     destroy_sprite_element(tset->side_tile_closest_4);
     destroy_sprite_element(tset->end_tile_1);
     destroy_sprite_element(tset->end_tile_2);
     destroy_sprite_element(tset->end_tile_3);
     destroy_sprite_element(tset->end_tile_4);
}

void render_items(BITMAP *page, Item_reference item, Item_reference *store, int x, int y, int s)
{
     int cur_ref = item.ref;
     int cur_next = item.next;
     int offset = 0;

     while (cur_ref)
     {
          stretch_sprite(page, repository[cur_ref].sprite, VIEW_X + offset + x, VIEW_Y + y, s, s);
          cur_ref = store[cur_next].ref;
          cur_next = store[cur_next].next;
          offset += 4;
     }
}

void render_items_on_tile_closest(BITMAP *page, Maptile tile, Item_reference *store, int pdir)
{
     unsigned char l[] = {0, 3, 2, 1};
     unsigned char r[] = {1, 2, 0, 3};
     render_items(page, tile.item_slots[l[pdir]], store, 27, 103, 16);
     render_items(page, tile.item_slots[r[pdir]], store, 131, 103, 16);
}

void render_items_on_tile_distant(BITMAP *page, Maptile tile, Item_reference *store, int pdir, int x, int y)
{
     render_items(page, tile.item_slots[slot_index_fl[pdir]], store, 27, 103, 12);
     render_items(page, tile.item_slots[slot_index_fr[pdir]], store, 131, 103, 12);
     render_items(page, tile.item_slots[slot_index_cl[pdir]], store, 43, 89, 12);
     render_items(page, tile.item_slots[slot_index_cr[pdir]], store, 131, 103, 12);
}
