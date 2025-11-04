/*
 * Copyright (C) 2025 - KGS
 * MIT License
 */

#include <allegro.h>
#include <stdlib.h>
#include "map.h"
#include "render.h"
#include "utils.h"

#define VIEW_X 64
#define VIEW_Y 64

int main(int argc, const char **argv)
{
    int posx = 8;
    int posy = 8;
    int dirp = F_SOUTH;
    struct maptile *map = allocate_map();

    map[4].solid = 1;
    map[20].solid = 1;
    map[40].solid = 1;
    map[56].solid = 1;
    map[63].solid = 1;
    map[13].solid = 1;
    map[14].solid = 1;
    map[40 * 10 + 10].solid = 1;
    map[40 * 20 + 20].solid = 1;
    map[40 * 30 + 30].solid = 1;
    map[40 * 30 + 31].solid = 1;
    map[40 * 30 + 32].solid = 1;
    map[40 * 31 + 32].solid = 1;

    if (allegro_init() != 0)
    {
        return 1;
    }

    install_keyboard();
    install_timer();

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
        set_palette(default_palette);
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
                posx += get_y_move_offset(dirp);
                posy -= get_x_move_offset(dirp);
                break;
            case KEY_D:
                posx -= get_y_move_offset(dirp);
                posy += get_x_move_offset(dirp);
                break;
            case KEY_ESC:
                return 0;
            default:
                break;
            };

            // blit(bmp, screen, 0, 0, posx, posy, 64, 64);
            clear_to_color(screen, 255);

            for (int xp = 0; xp < 40; xp++)
            {
                for (int yp = 0; yp < 40; yp++)
                {
                    if (map[yp * 40 + xp].solid)
                        rectfill(screen, xp * 4, yp * 4, xp * 4 + 3, yp * 4 + 3, 6);
                }
            }

            rectfill(screen, posx * 4, posy * 4, posx * 4 + 3, posy * 4 + 3, 5);

            // F
            rectfill(screen,
                     (posx + get_x_move_offset(dirp)) * 4 + 1,
                     (posy + get_y_move_offset(dirp)) * 4 + 1,
                     (posx + get_x_move_offset(dirp)) * 4 + 2,
                     (posy + get_y_move_offset(dirp)) * 4 + 2, 6);

            // FF
            rectfill(screen,
                     (posx + 2 * get_x_move_offset(dirp)) * 4 + 1,
                     (posy + 2 * get_y_move_offset(dirp)) * 4 + 1,
                     (posx + 2 * get_x_move_offset(dirp)) * 4 + 2,
                     (posy + 2 * get_y_move_offset(dirp)) * 4 + 2, 6);

            // FFF
            rectfill(screen,
                     (posx + 3 * get_x_move_offset(dirp)) * 4 + 1,
                     (posy + 3 * get_y_move_offset(dirp)) * 4 + 1,
                     (posx + 3 * get_x_move_offset(dirp)) * 4 + 2,
                     (posy + 3 * get_y_move_offset(dirp)) * 4 + 2, 6);

            // FL
            rectfill(screen,
                     (posx + get_x_move_offset(dirp) + get_x_move_offset(get_prev_facing(dirp))) * 4 + 1,
                     (posy + get_y_move_offset(dirp) + get_y_move_offset(get_prev_facing(dirp))) * 4 + 1,
                     (posx + get_x_move_offset(dirp) + get_x_move_offset(get_prev_facing(dirp))) * 4 + 2,
                     (posy + get_y_move_offset(dirp) + get_y_move_offset(get_prev_facing(dirp))) * 4 + 2, 6);

            // FR
            rectfill(screen,
                     (posx + get_x_move_offset(dirp) + get_x_move_offset(get_next_facing(dirp))) * 4 + 1,
                     (posy + get_y_move_offset(dirp) + get_y_move_offset(get_next_facing(dirp))) * 4 + 1,
                     (posx + get_x_move_offset(dirp) + get_x_move_offset(get_next_facing(dirp))) * 4 + 2,
                     (posy + get_y_move_offset(dirp) + get_y_move_offset(get_next_facing(dirp))) * 4 + 2, 6);

            // FFL
            rectfill(screen,
                     (posx + 2 * get_x_move_offset(dirp) + get_x_move_offset(get_prev_facing(dirp))) * 4 + 1,
                     (posy + 2 * get_y_move_offset(dirp) + get_y_move_offset(get_prev_facing(dirp))) * 4 + 1,
                     (posx + 2 * get_x_move_offset(dirp) + get_x_move_offset(get_prev_facing(dirp))) * 4 + 2,
                     (posy + 2 * get_y_move_offset(dirp) + get_y_move_offset(get_prev_facing(dirp))) * 4 + 2, 6);

            // FFR
            rectfill(screen,
                     (posx + 2 * get_x_move_offset(dirp) + get_x_move_offset(get_next_facing(dirp))) * 4 + 1,
                     (posy + 2 * get_y_move_offset(dirp) + get_y_move_offset(get_next_facing(dirp))) * 4 + 1,
                     (posx + 2 * get_x_move_offset(dirp) + get_x_move_offset(get_next_facing(dirp))) * 4 + 2,
                     (posy + 2 * get_y_move_offset(dirp) + get_y_move_offset(get_next_facing(dirp))) * 4 + 2, 6);

            // FFLL
            rectfill(screen,
                     (posx + 2 * get_x_move_offset(dirp) + 2 * get_x_move_offset(get_prev_facing(dirp))) * 4 + 1,
                     (posy + 2 * get_y_move_offset(dirp) + 2 * get_y_move_offset(get_prev_facing(dirp))) * 4 + 1,
                     (posx + 2 * get_x_move_offset(dirp) + 2 * get_x_move_offset(get_prev_facing(dirp))) * 4 + 2,
                     (posy + 2 * get_y_move_offset(dirp) + 2 * get_y_move_offset(get_prev_facing(dirp))) * 4 + 2, 6);

            // FFRR
            rectfill(screen,
                     (posx + 2 * get_x_move_offset(dirp) + 2 * get_x_move_offset(get_next_facing(dirp))) * 4 + 1,
                     (posy + 2 * get_y_move_offset(dirp) + 2 * get_y_move_offset(get_next_facing(dirp))) * 4 + 1,
                     (posx + 2 * get_x_move_offset(dirp) + 2 * get_x_move_offset(get_next_facing(dirp))) * 4 + 2,
                     (posy + 2 * get_y_move_offset(dirp) + 2 * get_y_move_offset(get_next_facing(dirp))) * 4 + 2, 6);

            // rectfill(screen, VIEW_X, VIEW_Y, VIEW_X + 176 - 1, VIEW_Y + 120 - 1, 13);

            rest(350);
        } while (1);
    }
    return 0;
}

END_OF_MAIN()