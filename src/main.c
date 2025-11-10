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

#define MAX_SQUADS 4

int main(int argc, const char **argv)
{
    int posx = 8;
    int posy = 8;
    int dirp = F_SOUTH;
    int triggers = 0;
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

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
    {
        allegro_message("Error initialising sound system\n%s\n", allegro_error);
        return 1;
    }

    MIDI *music;
    music = load_midi("bsmus01.mid");

    play_midi(music, TRUE);

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

    BITMAP *bottle;
    bottle = load_tga("i01_bot.tga", 0);
    int held_item = 0;

    SAMPLE *snd_take_item = load_wav("snd_take.wav");
    SAMPLE *snd_drop_item = load_wav("snd_drop.wav");

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

    BITMAP *enemy_one_cultist = load_bitmap("enemy_1.tga", palette);

    struct enemy cultist;
    struct squad group[MAX_SQUADS];

    for (int i = 0; i < MAX_SQUADS; i++)
    {
        group[i].x = 0;
        group[i].y = 0;
        for (int p = 0; p < 4; p++)
        {
            group[i].units[p] = 0;
        }
    }

    group[0].units[0] = &cultist;
    group[1].units[0] = &cultist;
    group[2].units[0] = &cultist;
    group[3].units[0] = &cultist;
    group[4].units[0] = &cultist;
    group[5].units[0] = &cultist;
    group[6].units[0] = &cultist;
    group[7].units[0] = &cultist;

    cultist.picture = enemy_one_cultist;
    group[0].x = 0;
    group[0].y = 2;
    group[1].x = 3;
    group[1].y = 3;
    group[2].x = 4;
    group[2].y = 4;
    group[3].x = 5;
    group[3].y = 5;
    group[4].x = 6;
    group[4].y = 6;
    group[5].x = 7;
    group[5].y = 7;
    group[6].x = 8;
    group[6].y = 8;
    group[7].x = 9;
    group[7].y = 9;

    int k;
    int wait = 0;
    int enemy_distance;

    do
    {
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

        clear_to_color(activepage, makecol(0, 0, 0));

        for (int xp = 0; xp < MAP_SIGHT * 2 + 1; xp++)
        {
            for (int yp = 0; yp < MAP_SIGHT * 2 + 1; yp++)
            {
                if (issolid(map, posx + xp - MAP_SIGHT, posy + yp - MAP_SIGHT))
                    rectfill(activepage,
                             xp * MAP_TSIZE,
                             yp * MAP_TSIZE,
                             xp * MAP_TSIZE + MAP_TSIZE - 1,
                             yp * MAP_TSIZE + MAP_TSIZE - 1,
                             makecol(0, 255, 0));
            }
        }

        rectfill(activepage,
                 MAP_X + MAP_TSIZE * MAP_SIGHT,
                 MAP_Y + MAP_TSIZE * MAP_SIGHT,
                 MAP_X + MAP_TSIZE * MAP_SIGHT + MAP_TSIZE - 1,
                 MAP_Y + MAP_TSIZE * MAP_SIGHT + MAP_TSIZE - 1, makecol(0, 0, 255));

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

        for (int i = 0; i < MAX_SQUADS; i++)
        {
            enemy_distance = depth_position(posx, posy, dirp, group[i].x, group[i].y);
            if (enemy_distance == 3)
            {
                int offset = -32 * depth_position(posx, posy, get_next_facing(dirp), group[i].x, group[i].y);
                stretch_sprite(activepage, enemy_one_cultist,
                               VIEW_MID_X - enemy_one_cultist->w / 6 - offset,
                               VIEW_MID_Y - enemy_one_cultist->h / 6,
                               enemy_one_cultist->w / 6,
                               enemy_one_cultist->h / 6);
            }
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

        for (int i = 0; i < MAX_SQUADS; i++)
        {
            enemy_distance = depth_position(posx, posy, dirp, group[i].x, group[i].y);
            if (enemy_distance == 2)
            {
                int offset = -48 * depth_position(posx, posy, get_next_facing(dirp), group[i].x, group[i].y);
                stretch_sprite(activepage, enemy_one_cultist,
                               VIEW_MID_X - enemy_one_cultist->w / 4 - offset,
                               VIEW_MID_Y - enemy_one_cultist->h / 4,
                               enemy_one_cultist->w / 2,
                               enemy_one_cultist->h / 2);
            }
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
            draw_sprite_h_flip(activepage, angl_3, VIEW_X + 176 - 47 - 16, VIEW_Y + 19);
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
            draw_sprite(activepage, angl_2, VIEW_X + 23, VIEW_Y + 8);
        }

        if (check_if_clear_and_solid(map, posx, posy, dirp, 0, 1, 1, 1))
        {
            draw_sprite_h_flip(activepage, angl_2, VIEW_X + 176 - 49, VIEW_Y + 8);
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

        for (int i = 0; i < MAX_SQUADS; i++)
        {
            enemy_distance = depth_position(posx, posy, dirp, group[i].x, group[i].y);
            if (enemy_distance == 1 && depth_position(posx, posy, get_next_facing(dirp), group[i].x, group[i].y) == 0)
            {
                draw_sprite(activepage, enemy_one_cultist,
                            VIEW_MID_X - enemy_one_cultist->w / 2,
                            VIEW_MID_Y - enemy_one_cultist->h / 2);
            }
        }

        // Draw viewport frame
        rect(activepage, VIEW_X - 1, VIEW_Y - 1, VIEW_X + 176, VIEW_Y + 120, makecol(255, 0, 0));

        if (wait)
        {
            triggers += 1;
            //play_sample(snd_take_item, 255, 128, 1000, 0);
        }

        textprintf_centre_ex(activepage, font, SCREEN_W / 2, 180,
                             makecol(0, 100, 243), -1,
                             "Steps %d!",
                             triggers);

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
            wait = 0;
            rest(300);
        }

    } while (1);

    return 0;
}

END_OF_MAIN()