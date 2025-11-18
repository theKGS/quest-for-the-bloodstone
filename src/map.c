#include "map.h"
#include <stdlib.h>

/*
    Allocates space for the map and sets all values to 0.
*/
maptile *allocate_map()
{
    maptile *map_data;
    map_data = malloc(40 * 40 * sizeof(maptile));

    for (int i = 0; i < 40 * 40; i++)
    {
        map_data[i].solid = 0;
        map_data[i].u_tile = 0;
        map_data[i].d_tile = 0;
        map_data[i].l_tile = 0;
        map_data[i].r_tile = 0;
        map_data[i].u_passable = 0;
        map_data[i].d_passable = 0;
        map_data[i].l_passable = 0;
        map_data[i].r_passable = 0;
    }

    return map_data;
}

int issolid(maptile *map, int x, int y)
{
    if (x < 0 || x >= 40 || y < 0 || y >= 40)
        return 1; // Always solid outside of map bounds
    return (map[y * 40 + x].solid);
};

int check_if_clear_and_solid(maptile *map, int px, int py, int dirp, int lr1, int fb1, int lr2, int fb2)
{
    return !issolid(map, position_offset_to_x(px, dirp, lr1, fb1), position_offset_to_y(py, dirp, lr1, fb1)) &&
           issolid(map, position_offset_to_x(px, dirp, lr2, fb2), position_offset_to_y(py, dirp, lr2, fb2));
}
