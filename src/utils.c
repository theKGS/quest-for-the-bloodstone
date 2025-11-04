#include <allegro.h>
#include "utils.h"
#include "map.h"

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

int get_x_move_offset(int facing)
{
    switch (facing)
    {
    case F_NORTH:
    case F_SOUTH:
        return 0;
    case F_EAST:
        return 1;
    default:
        return -1;
    }
}

int get_y_move_offset(int facing)
{
    switch (facing)
    {
    case F_EAST:
    case F_WEST:
        return 0;
    case F_SOUTH:
        return 1;
    default:
        return -1;
    }
}

int get_next_facing(int facing)
{
    switch (facing)
    {
    case F_EAST:
        return F_SOUTH;
    case F_WEST:
        return F_NORTH;
    case F_SOUTH:
        return F_WEST;
    default:
        return F_EAST;
    }
}

int get_prev_facing(int facing)
{
    switch (facing)
    {
    case F_EAST:
        return F_NORTH;
    case F_WEST:
        return F_SOUTH;
    case F_SOUTH:
        return F_EAST;
    default:
        return F_WEST;
    }
}
