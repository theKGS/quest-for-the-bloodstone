#include <allegro.h>
#include "utils.h"
#include "map.h"

// lookup tables for slot indices
// far left, far right, close left, close right
const unsigned char slot_index_fl[4] = {0, 3, 2, 1};
const unsigned char slot_index_fr[4] = {1, 2, 0, 3};
const unsigned char slot_index_cl[4] = {2, 1, 3, 0};
const unsigned char slot_index_cr[4] = {3, 0, 1, 2};

// lookup tables for finding world coordinates from relative coordinates
const signed char x_move_offset[4] = {0, 0, -1, 1};
const signed char y_move_offset[4] = {-1, 1, 0, 0};

// lookup tables for rotating directions
const unsigned char next_facing[4] = {F_EAST, F_WEST, F_NORTH, F_SOUTH};
const unsigned char prev_facing[4] = {F_WEST, F_EAST, F_SOUTH, F_NORTH};

int getkey()
{
    if (keypressed())
    {
        if (key[KEY_W])
        {
            return KEY_W;
        };
        if (key[KEY_A])
        {
            return KEY_A;
        };
        if (key[KEY_S])
        {
            return KEY_S;
        };
        if (key[KEY_D])
        {
            return KEY_D;
        };
        if (key[KEY_Q])
        {
            return KEY_Q;
        };
        if (key[KEY_E])
        {
            return KEY_E;
        };
        if (key[KEY_ESC])
        {
            return KEY_ESC;
        }
    }

    return 0;
}

int position_offset_to_x(int posx, int dirp, int lr_steps, int fb_steps)
{
    if (lr_steps < 0)
    {
        lr_steps *= -1;
        return posx + fb_steps * x_move_offset[dirp] + lr_steps * x_move_offset[prev_facing[dirp]];
    }
    return posx + fb_steps * x_move_offset[dirp] + lr_steps * x_move_offset[next_facing[dirp]];
}

int position_offset_to_y(int posy, int dirp, int lr_steps, int fb_steps)
{
    if (lr_steps < 0)
    {
        lr_steps *= -1;
        return posy + fb_steps * y_move_offset[dirp] + lr_steps * y_move_offset[prev_facing[dirp]];
    }
    return posy + fb_steps * y_move_offset[dirp] + lr_steps * y_move_offset[next_facing[dirp]];
}

int depth_position(int posx, int posy, int dirp, int ex, int ey)
{
    switch (dirp)
    {
    case F_EAST:
        return ex - posx;
    case F_WEST:
        return posx - ex;
    case F_SOUTH:
        return ey - posy;
    default:
        return posy - ey;
    }
}

int horizontal_position(int posx, int posy, int dirp, int ex, int ey)
{
    switch (dirp)
    {
    case F_EAST:
        return posy - ey;
    case F_WEST:
        return ey - posy;
    case F_SOUTH:
        return posx - ex;
    default:
        return ex - posx;
    }
}