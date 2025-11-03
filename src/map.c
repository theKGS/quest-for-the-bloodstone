#include "map.h"
#include <stdlib.h>

/*
    Allocates space for the map and sets all values to 0.
*/
void allocate_map()
{
    //struct maptile *pm;
    //map_data = malloc(40 * 40 * sizeof(struct maptile));

    for (int i = 0; i < 40 * 40; i++) {
        //map_data[i].u_tile = 0;
        //map_data[i].d_tile = 0;
        //map_data[i].l_tile = 0;
        //map_data[i].r_tile = 0;
        //map_data[i].u_passable = 0;
        //map_data[i].d_passable = 0;
        //map_data[i].l_passable = 0;
        //map_data[i].r_passable = 0;
    }
}

void fill_render_array() {

}