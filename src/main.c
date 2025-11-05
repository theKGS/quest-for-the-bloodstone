/*
 * Copyright (C) 2025 - KGS
 * MIT License
 */

#include <allegro.h>
#include <stdlib.h>
#include "map.h"
#include "render.h"
#include "utils.h"

#define VIEW_WIDTH 176
#define VIEW_HEIGHT 120
#define VIEW_X 320 - 1 - VIEW_WIDTH
#define VIEW_Y 64
#define VIEW_MID_X VIEW_X + VIEW_WIDTH / 2
#define VIEW_MID_Y VIEW_Y + VIEW_HEIGHT / 2

/*
        2-----3
       /     /
      /     /
     /     /
    1-----4
*/

void drawcube(BITMAP *screen, int x, int z)
{
    int hp1_x = (x * 16 - 8) * 16;
    int hp2_x = (x * 16 - 8) * 16;
    int hp3_x = (x * 16 + 8) * 16;
    int hp4_x = (x * 16 + 8) * 16;
    int hp5_x = (x * 16 - 8) * 16;
    int hp6_x = (x * 16 - 8) * 16;
    int hp7_x = (x * 16 + 8) * 16;
    int hp8_x = (x * 16 + 8) * 16;

    int hp1_y = 16 * 16;
    int hp2_y = 16 * 16;
    int hp3_y = 16 * 16;
    int hp4_y = 16 * 16;
    int hp5_y = -16 * 16;
    int hp6_y = -16 * 16;
    int hp7_y = -16 * 16;
    int hp8_y = -16 * 16;

    int hp1_z = (z * 16 - 8) * 2;
    int hp2_z = (z * 16 + 8) * 2;
    int hp3_z = (z * 16 + 8) * 2;
    int hp4_z = (z * 16 - 8) * 2;
    int hp5_z = (z * 16 - 8) * 2;
    int hp6_z = (z * 16 + 8) * 2;
    int hp7_z = (z * 16 + 8) * 2;
    int hp8_z = (z * 16 - 8) * 2;

    line(screen,
         VIEW_MID_X + (hp1_x / hp1_z), VIEW_MID_Y + (hp1_y / hp1_z),
         VIEW_MID_X + (hp2_x / hp2_z), VIEW_MID_Y + (hp2_y / hp2_z), 11);

    line(screen,
         VIEW_MID_X + (hp2_x / hp2_z), VIEW_MID_Y + (hp2_y / hp2_z),
         VIEW_MID_X + (hp3_x / hp3_z), VIEW_MID_Y + (hp3_y / hp3_z), 11);

    line(screen,
         VIEW_MID_X + (hp3_x / hp3_z), VIEW_MID_Y + (hp3_y / hp3_z),
         VIEW_MID_X + (hp4_x / hp4_z), VIEW_MID_Y + (hp4_y / hp4_z), 11);

    line(screen,
         VIEW_MID_X + (hp4_x / hp4_z), VIEW_MID_Y + (hp4_y / hp4_z),
         VIEW_MID_X + (hp1_x / hp1_z), VIEW_MID_Y + (hp1_y / hp1_z), 11);

    line(screen,
         VIEW_MID_X + (hp5_x / hp5_z), VIEW_MID_Y + (hp5_y / hp5_z),
         VIEW_MID_X + (hp6_x / hp6_z), VIEW_MID_Y + (hp6_y / hp6_z), 12);

    line(screen,
         VIEW_MID_X + (hp6_x / hp6_z), VIEW_MID_Y + (hp6_y / hp6_z),
         VIEW_MID_X + (hp7_x / hp7_z), VIEW_MID_Y + (hp7_y / hp7_z), 12);

    line(screen,
         VIEW_MID_X + (hp7_x / hp7_z), VIEW_MID_Y + (hp7_y / hp7_z),
         VIEW_MID_X + (hp8_x / hp8_z), VIEW_MID_Y + (hp8_y / hp8_z), 12);

    line(screen,
         VIEW_MID_X + (hp8_x / hp8_z), VIEW_MID_Y + (hp8_y / hp8_z),
         VIEW_MID_X + (hp5_x / hp5_z), VIEW_MID_Y + (hp5_y / hp5_z), 12);

    line(screen,
         VIEW_MID_X + (hp1_x / hp1_z), VIEW_MID_Y + (hp1_y / hp1_z),
         VIEW_MID_X + (hp5_x / hp5_z), VIEW_MID_Y + (hp5_y / hp5_z), 10);

    line(screen,
         VIEW_MID_X + (hp2_x / hp2_z), VIEW_MID_Y + (hp2_y / hp2_z),
         VIEW_MID_X + (hp6_x / hp6_z), VIEW_MID_Y + (hp6_y / hp6_z), 10);

    line(screen,
         VIEW_MID_X + (hp3_x / hp3_z), VIEW_MID_Y + (hp3_y / hp3_z),
         VIEW_MID_X + (hp7_x / hp7_z), VIEW_MID_Y + (hp7_y / hp7_z), 10);

    line(screen,
         VIEW_MID_X + (hp4_x / hp4_z), VIEW_MID_Y + (hp4_y / hp4_z),
         VIEW_MID_X + (hp8_x / hp8_z), VIEW_MID_Y + (hp8_y / hp8_z), 10);
}

int check_if_clear_and_solid(struct maptile *map, int px, int py, int dirp, int lr1, int fb1, int lr2, int fb2)
{
    return !issolid(map, position_offset_to_x(px, dirp, lr1, fb1), position_offset_to_y(py, dirp, lr1, fb1)) &&
           issolid(map, position_offset_to_x(px, dirp, lr2, fb2), position_offset_to_y(py, dirp, lr2, fb2));
}

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

    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 400) != 0)
    {
        if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0)
        {
            if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0)
            {
                set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
                allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
                return 1;
            }
        }
    }

    PALETTE palette;
    BITMAP *page1;
    BITMAP *page2;

    page1 = create_video_bitmap(320, 200);
    page2 = create_video_bitmap(320, 200);

    if ((!page1) || (!page2))
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to create two video memory pages\n");
        return 1;
    }

    BITMAP *activepage = page1;

    BITMAP *walls1;
    walls1 = load_tga("walls1.tga", palette);

    BITMAP *flat_1 = create_bitmap(128, 96);
    BITMAP *flat_1a = create_bitmap(24, 96);
    BITMAP *flat_1b = create_bitmap(24, 96);
    BITMAP *flat_2 = create_bitmap(80, 59);

    blit(walls1, flat_1, 0, 0, 0, 0, 128, 96);
    blit(walls1, flat_1a, 128 - 24, 0, 0, 0, 24, 96);
    blit(walls1, flat_1b, 0, 0, 0, 0, 24, 96);
    blit(walls1, flat_2, 128, 0, 0, 0, 80, 59);

    BITMAP *angl_1 = create_bitmap(24, 120);
    BITMAP *angl_2 = create_bitmap(24, 95);

    blit(walls1, angl_1, 208, 0, 0, 0, 24, 120);
    blit(walls1, angl_2, 232, 0, 0, 0, 24, 95);

    set_palette(palette);
    clear_to_color(activepage, makecol(128, 0, 0));

    int k;
    int wait = 0;

    int render_queue_x[20];
    int render_queue_z[20];
    int curpos = 0;

    for (int i = 0; i < 20; i++)
    {
        render_queue_x[i] = 0;
        render_queue_z[i] = 0;
    }

    do
    {
        curpos = 0;
        wait = 0;
        k = getkey();
        switch (k)
        {
        case KEY_W:
            wait = 1;
            posx += get_x_move_offset(dirp);
            posy += get_y_move_offset(dirp);
            break;
        case KEY_S:
            wait = 1;
            posx -= get_x_move_offset(dirp);
            posy -= get_y_move_offset(dirp);
            break;
        case KEY_Q:
            wait = 1;
            dirp = get_prev_facing(dirp);
            break;
        case KEY_E:
            wait = 1;
            dirp = get_next_facing(dirp);
            break;
        case KEY_A:
            wait = 1;
            posx += get_y_move_offset(dirp);
            posy -= get_x_move_offset(dirp);
            break;
        case KEY_D:
            wait = 1;
            posx -= get_y_move_offset(dirp);
            posy += get_x_move_offset(dirp);
            break;
        case KEY_ESC:
            return 0;
        default:
            break;
        };

        // blit(bmp, screen, 0, 0, posx, posy, 64, 64);
        clear_to_color(activepage, 255);

        for (int xp = 0; xp < 40; xp++)
        {
            for (int yp = 0; yp < 40; yp++)
            {
                if (map[yp * 40 + xp].solid)
                    rectfill(activepage, xp * 4, yp * 4, xp * 4 + 3, yp * 4 + 3, 6);
            }
        }

        rectfill(activepage, posx * 4, posy * 4, posx * 4 + 3, posy * 4 + 3, 5);

        // Z depth = 0
        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, -1, 0)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, -1, 0)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, -1, 0)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, -1, 0)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, 1, 0)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, 1, 0)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, 1, 0)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, 1, 0)) * 4 + 2, 6);

        // Z depth = 1
        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, 0, 1)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, 0, 1)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, 0, 1)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, 0, 1)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, -1, 1)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, -1, 1)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, -1, 1)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, -1, 1)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, 1, 1)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, 1, 1)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, 1, 1)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, 1, 1)) * 4 + 2, 6);

        // Z depth = 1
        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, 0, 2)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, 0, 2)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, 0, 2)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, 0, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, -1, 2)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, -1, 2)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, -1, 2)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, -1, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, -2, 2)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, -2, 2)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, -2, 2)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, -2, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, 1, 2)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, 1, 2)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, 1, 2)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, 1, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(posx, dirp, 2, 2)) * 4 + 1,
                 (position_offset_to_y(posy, dirp, 2, 2)) * 4 + 1,
                 (position_offset_to_x(posx, dirp, 2, 2)) * 4 + 2,
                 (position_offset_to_y(posy, dirp, 2, 2)) * 4 + 2, 6);

        // Segments two steps away

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 1, 0, 2))
        {
            masked_blit(flat_2, activepage, 0, 0, VIEW_X + 47, VIEW_Y + 19, 80, 59);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, -1, 1, -1, 2))
        {
            masked_blit(flat_2, activepage, 0, 0, VIEW_X + 47 - 80, VIEW_Y + 19, 80, 59);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 1, 1, 1, 2))
        {
            masked_blit(flat_2, activepage, 0, 0, VIEW_X + 47 + 80, VIEW_Y + 19, 80, 59);
        }

        //

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 0, 0, 1))
        {
            masked_blit(flat_1, activepage, 0, 0, VIEW_X + 24, VIEW_Y + 7, 128, 96);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, -1, 0, -1, 1))
        {
            masked_blit(flat_1a, activepage, 0, 0, VIEW_X, VIEW_Y + 7, 24, 96);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 1, 0, 1, 1))
        {
            masked_blit(flat_1b, activepage, 0, 0, VIEW_X + 176 - 25, VIEW_Y + 7, 24, 96);
        }

        //
        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 1, -1, 1))
        {
            draw_sprite(activepage, angl_2, VIEW_X + 23, VIEW_Y + 6);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 1, 1, 1))
        {
            draw_sprite_h_flip(activepage, angl_2, VIEW_X + 176 - 49, VIEW_Y + 6);
        }

        //

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 0, -1, 0))
        {
            draw_sprite(activepage, angl_1, VIEW_X, VIEW_Y);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 0, 1, 0))
        {
            draw_sprite_h_flip(activepage, angl_1, VIEW_X + 176 - 25, VIEW_Y);
        }

        show_video_bitmap(activepage);

        if (activepage == page1)
        {
            activepage = page2;
        }
        else
        {
            activepage = page1;
        }

        if (wait)
        {
            rest(300);
        }

    } while (1);

    return 0;
}

END_OF_MAIN()