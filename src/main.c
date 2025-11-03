/*
 * Copyright (C) 2025 - KGS
 * MIT License
 */

#include <allegro.h>
#include <stdlib.h>
#include "map.h"
#include "render.h"

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

int main(int argc, const char **argv)
{
    int posx = 2;
    int posy = 2;
    int dirp = F_SOUTH;
    int time_since_move = 0;

    if (allegro_init() != 0)
    {
        return 1;
    }

    install_keyboard();
    install_timer();
    // install_mouse();
    // show_mouse(screen);

    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Cannot set graphics mode:\r\n%s\r\n", allegro_error);
        return 1;
    }

    BITMAP *bmp;
    PALETTE palette;

    bmp = load_pcx("walls1.pcx", palette);

    if (bmp)
    {
        set_palette(desktop_palette);
        clear_to_color(screen, makecol(128, 0, 0));

        int k;

        do
        {
            k = getkey();
            switch (k)
            {
            case KEY_W:
                posx += get_x_move_offset(dirp);
                posy += get_y_move_offset(dirp);
                break;
            case KEY_S:
                posx -= get_x_move_offset(dirp);
                posy -= get_y_move_offset(dirp);
                break;
            case KEY_Q:
                dirp = get_prev_facing(dirp);
                break;
            case KEY_E:
                dirp = get_next_facing(dirp);
                break;
            case KEY_A:
                posx -= get_y_move_offset(dirp);
                posy -= get_x_move_offset(dirp);
                break;
            case KEY_D:
                posx += get_y_move_offset(dirp);
                posy += get_x_move_offset(dirp);
                break;
            case KEY_ESC:
                return 0;
            default:
                break;
            };

            // blit(bmp, screen, 0, 0, posx, posy, 64, 64);
            clear_to_color(screen, 0);
            rectfill(screen, posx * 4, posy * 4, posx * 4 + 4, posy * 4 + 4, 5);
            rectfill(screen,
                     (posx + get_x_move_offset(dirp)) * 4,
                     (posy + get_y_move_offset(dirp)) * 4,
                     (posx + get_x_move_offset(dirp)) * 4 + 4,
                     (posy + get_y_move_offset(dirp)) * 4 + 4, 6);
            rest(150);
        } while (1);
    }
    return 0;
}

END_OF_MAIN()