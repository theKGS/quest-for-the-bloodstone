/*
 * Copyright (C) 2025 - KGS
 * MIT License
 */

#include <allegro.h>
#include <stdlib.h>
#include "map.h"
#include "render.h"
#include "utils.h"
#include "enemy.h"


int main(int argc, const char **argv)
{
    int posx = 8;
    int posy = 8;
    int dirp = F_SOUTH;
    struct maptile *map = allocate_map();
    srand(0);

    for (int xp = 0; xp < 40; xp++)
    {
        for (int yp = 0; yp < 40; yp++)
        {
            int r = rand() % 100;
            if (r >= 75)
                map[yp * 40 + xp].solid = 1;
        }
    }

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
    BITMAP *flat_3 = create_bitmap(48, 37);

    blit(walls1, flat_1, 0, 0, 0, 0, 128, 96);
    blit(walls1, flat_1a, 128 - 24, 0, 0, 0, 24, 96);
    blit(walls1, flat_1b, 0, 0, 0, 0, 24, 96);
    blit(walls1, flat_2, 128, 0, 0, 0, 80, 59);
    blit(walls1, flat_3, 128, 59, 0, 0, 48, 37);

    BITMAP *angl_1 = create_bitmap(24, 120);
    BITMAP *angl_2 = create_bitmap(24, 95);
    BITMAP *angl_3 = create_bitmap(16, 59);
    BITMAP *angl_4 = create_bitmap(8, 35);

    blit(walls1, angl_1, 208, 0, 0, 0, 24, 120);
    blit(walls1, angl_2, 232, 0, 0, 0, 24, 95);
    blit(walls1, angl_3, 256, 0, 0, 0, 16, 59);
    blit(walls1, angl_4, 272, 0, 0, 0, 8, 25);

    set_palette(palette);
    clear_to_color(activepage, makecol(128, 0, 0));

    BITMAP *enemy_one_cultist = load_bitmap("enemy_1.tga", palette);

    struct enemy cultist;
    struct squad group;
    group.units[0] = &cultist;
    group.units[1] = 0;
    group.units[2] = 0;
    group.units[3] = 0;

    cultist.picture = enemy_one_cultist;
    group.x = 2;
    group.y = 2;

    int k;
    int wait = 0;
    int enemy_distance;

    int render_queue_x[20];
    int render_queue_z[20];

    for (int i = 0; i < 20; i++)
    {
        render_queue_x[i] = 0;
        render_queue_z[i] = 0;
    }

    do
    {
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
                    rectfill(activepage, xp * 4, yp * 4, xp * 4 + 3, yp * 4 + 3, makecol(0, 255, 0));
            }
        }

        rectfill(activepage, posx * 4, posy * 4, posx * 4 + 3, posy * 4 + 3, makecol(0, 0, 255));

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

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 2, 0, 3))
        {
            draw_sprite(activepage, flat_3, VIEW_X + 63, VIEW_Y + 26);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, -1, 2, -1, 3))
        {
            draw_sprite(activepage, flat_3, VIEW_X + 63 - 48, VIEW_Y + 26);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, -2, 2, -2, 3))
        {
            draw_sprite(activepage, flat_3, VIEW_X + 63 - 2 * 48, VIEW_Y + 26);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 1, 2, 1, 3))
        {
            draw_sprite(activepage, flat_3, VIEW_X + 63 + 48, VIEW_Y + 26);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 2, 2, 2, 3))
        {
            draw_sprite(activepage, flat_3, VIEW_X + 63 + 2 * 48, VIEW_Y + 26);
        }

        // Segments two steps away

        enemy_distance = depth_position(posx, posy, dirp, group.x, group.y);
        if (enemy_distance == 2)
        {
            int offset = -48 * depth_position(posx, posy, get_next_facing(dirp), group.x, group.y);
            stretch_sprite(activepage, enemy_one_cultist,
                           VIEW_MID_X - enemy_one_cultist->w / 4 - offset,
                           VIEW_MID_Y - enemy_one_cultist->h / 4,
                           enemy_one_cultist->w / 2,
                           enemy_one_cultist->h / 2);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 1, 0, 2))
        {
            draw_sprite(activepage, flat_2, VIEW_X + 47, VIEW_Y + 19);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, -1, 1, -1, 2))
        {
            draw_sprite(activepage, flat_2, VIEW_X + 47 - 80, VIEW_Y + 19);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 1, 1, 1, 2))
        {
            draw_sprite(activepage, flat_2, VIEW_X + 47 + 80, VIEW_Y + 19);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 2, -1, 2))
        {
            draw_sprite(activepage, angl_3, VIEW_X + 47, VIEW_Y + 19);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 2, 1, 2))
        {
            draw_sprite_h_flip(activepage, angl_3, VIEW_X + 176 - 47, VIEW_Y + 19);
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

        enemy_distance = depth_position(posx, posy, dirp, group.x, group.y);
        if (enemy_distance == 1 && depth_position(posx, posy, get_next_facing(dirp), group.x, group.y) == 0)
        {
            draw_sprite(activepage, enemy_one_cultist,
                        VIEW_MID_X - enemy_one_cultist->w / 2,
                        VIEW_MID_Y - enemy_one_cultist->h / 2);
        }

        // Draw viewport frame
        rect(activepage, VIEW_X - 1, VIEW_Y - 1, VIEW_X + 176, VIEW_Y + 120, makecol(255, 0, 0));

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