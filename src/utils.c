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

// Rotates the input direction clockwise
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

// Rotates the input direction counterclockwise
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

int position_offset_to_x(int posx, int dirp, int lr_steps, int fb_steps)
{
    if (lr_steps < 0)
    {
        lr_steps *= -1;
        return posx + fb_steps * get_x_move_offset(dirp) + lr_steps * get_x_move_offset(get_prev_facing(dirp));
    }
    return posx + fb_steps * get_x_move_offset(dirp) + lr_steps * get_x_move_offset(get_next_facing(dirp));
}

int position_offset_to_y(int posy, int dirp, int lr_steps, int fb_steps)
{
    if (lr_steps < 0)
    {
        lr_steps *= -1;
        return posy + fb_steps * get_y_move_offset(dirp) + lr_steps * get_y_move_offset(get_prev_facing(dirp));
    }
    return posy + fb_steps * get_y_move_offset(dirp) + lr_steps * get_y_move_offset(get_next_facing(dirp));
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
