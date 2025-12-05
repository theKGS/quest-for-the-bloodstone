#include "map.h"
#include <stdlib.h>
#include "utils.h"

/*
    Allocates space for the map and sets all values to 0.
*/
maptile *allocate_map()
{
    maptile *map_data;
    map_data = malloc(40 * 40 * sizeof(maptile));

    for (int i = 0; i < 40 * 40; i++)
    {
        map_data[i].tile_wall[0] = 0;
        map_data[i].tile_wall[1] = 0;
        map_data[i].tile_wall[2] = 0;
        map_data[i].tile_wall[3] = 0;
        map_data[i].solid_wall[0] = 0;
        map_data[i].solid_wall[1] = 0;
        map_data[i].solid_wall[2] = 0;
        map_data[i].solid_wall[3] = 0;
    }

    for (int i = 0; i < 40; i++)
    {
        map_data[i].solid_wall[F_NORTH] = 1;
        map_data[40 * 39 + i].solid_wall[F_SOUTH] = 1;
        map_data[40 * i].solid_wall[F_WEST] = 1;
        map_data[40 * i + 39].solid_wall[F_EAST] = 1;
    }

    return map_data;
}

int issolid(maptile *map, int x, int y)
{
    return 0;
};

int get_tile_wall(maptile *map, int px, int py, int pdir, int lr, int fb, int direction)
{
    char crdx = position_offset_to_x(px, pdir, lr, fb);
    char crdy = position_offset_to_y(py, pdir, lr, fb);

    return (crdx >= 0 && crdx < 40 && crdy >= 0 && crdy < 40) &&
           map[40 * crdy + crdx].tile_wall[direction];
}

void randomise_map(maptile *map)
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
