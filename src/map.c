#include "map.h"
#include <stdlib.h>
#include "utils.h"

/*
    Allocates space for the map tiles
*/
Maptile *allocate_map_tiles()
{
    Maptile *tiles;
    tiles = malloc(40 * 40 * sizeof(Maptile));

    for (int i = 0; i < 40 * 40; i++)
    {
        tiles[i].tile_wall[0] = 0;
        tiles[i].tile_wall[1] = 0;
        tiles[i].tile_wall[2] = 0;
        tiles[i].tile_wall[3] = 0;
        tiles[i].solid_wall[0] = 0;
        tiles[i].solid_wall[1] = 0;
        tiles[i].solid_wall[2] = 0;
        tiles[i].solid_wall[3] = 0;

        for (int s = 0; s < 4; s++)
        {
            tiles[i].item_slots[s].meta = 0;
            tiles[i].item_slots[s].ref = 0;
            tiles[i].item_slots[s].next = 0;
        }
    }

    for (int i = 0; i < 40; i++)
    {
        tiles[i].tile_wall[F_NORTH] = 1;
        tiles[40 * 39 + i].tile_wall[F_SOUTH] = 1;
        tiles[40 * i].tile_wall[F_WEST] = 1;
        tiles[40 * i + 39].tile_wall[F_EAST] = 1;
    }

    return tiles;
}

/*
    Allocates a new Map
*/
Map *allocate_map()
{
    Map *map = malloc(sizeof(Map));
    map->w = 40;
    map->h = 40;
    map->tiles = allocate_map_tiles();

    map->store = malloc(256 * sizeof(Item_reference));

    for (int i = 0; i < 256; i++)
    {
        map->store[i].ref = 0;
        map->store[i].meta = 0;
        map->store[i].next = 0;
    }

    return map;
}

void destroy_map(Map *map)
{
    free(map->tiles);
    free(map->store);
    free(map);
}

int issolid(Maptile *map, int x, int y)
{
    return 0;
};

int get_tile_wall(Maptile *map, int px, int py, int pdir, int lr, int fb, int direction)
{
    char crdx = position_offset_to_x(px, pdir, lr, fb);
    char crdy = position_offset_to_y(py, pdir, lr, fb);

    return (crdx >= 0 && crdx < 40 && crdy >= 0 && crdy < 40) &&
           map[40 * crdy + crdx].tile_wall[direction];
}

void randomise_map(Maptile *map)
{
    for (size_t x = 0; x < 40; x++)
    {
        for (size_t y = 0; y < 39; y++)
        {
            char p = rand() % 2;
            map[40 * y + x].tile_wall[F_SOUTH] = p;
            map[40 * (y + 1) + x].tile_wall[F_NORTH] = p;
        }
    }

    for (size_t x = 0; x < 39; x++)
    {
        for (size_t y = 0; y < 40; y++)
        {
            char p = rand() % 2;
            map[40 * y + x].tile_wall[F_EAST] = p;
            map[40 * y + x + 1].tile_wall[F_WEST] = p;
        }
    }
}


/*
    Add an item to a map slot
*/
void drop_item(Item_reference *slot, Item_reference *store, unsigned char ref, unsigned char meta)
{
    if (slot->ref)
    {
        int free = -1;
        for (int index = 1; index < 256; index++)
        {
            if (store[index].ref == 0)
            {
                free = index;
                break;
            }
        }

        store[free] = *slot;
        *slot = (Item_reference){ref, meta, free};
    }
    else
    {
        *slot = (Item_reference){ref, meta, 0};
    }
}

/*
    Remove an item from a map slot
*/
void remove_item(Item_reference *slot, Item_reference *store)
{
    if (slot->ref)
    {
        int index = slot->next;
        *slot = store[index];
        store[index] = (Item_reference){0, 0, 0};
    }
}