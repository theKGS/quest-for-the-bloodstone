/*
 * Copyright (C) 2025 - KGS
 * MIT License
 */

#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "render.h"
#include "utils.h"
#include "enemy.h"
#include "item.h"

#define MAX_SQUADS 4

int gameloop()
{
    int posx = 8;
    int posy = 8;
    int dirp = F_SOUTH;
    int triggers = 0;
    maptile *map = allocate_map();
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

    MIDI *music;
    music = load_midi("bsmus02.mid");

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

    SAMPLE *snd_take_item = load_wav("snd_tak2.wav");
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
            play_sample(snd_take_item, 255, 128, 1000, 0);
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

int musictest()
{
    printf("1) Track 1");
    printf("2) Track 2");
    printf("3) Track 3");
    MIDI *song;

    clear_keybuf();
    do
    {
        while (!keypressed())
        {
        };

        if (key[KEY_1])
        {
            song = load_midi("bsmus01.mid");
            play_midi(song, 1);
        }

        if (key[KEY_2])
        {
            song = load_midi("bsmus02.mid");
            play_midi(song, 1);
        }

        if (key[KEY_3])
        {
            song = load_midi("bsmus03.mid");
            play_midi(song, 1);
        }

        clear_keybuf();
    } while (!key[KEY_ESC]);
}

int inventorytest()
{
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

    install_mouse();

    BITMAP *activepage = page1;
    PALETTE palette;

    // BITMAP *testimage = load_tga("testart.tga", 0);
    BITMAP *testimage = load_tga("testart.tga", palette);

    BITMAP *item_sprites[5];

    item_sprites[0] = create_bitmap(16, 16);
    blit(testimage, item_sprites[0], 32, 0, 0, 0, 16, 16);
    item_sprites[1] = create_bitmap(16, 16);
    blit(testimage, item_sprites[1], 48, 0, 0, 0, 16, 16);
    item_sprites[2] = create_bitmap(16, 16);
    blit(testimage, item_sprites[2], 64, 0, 0, 0, 16, 16);
    item_sprites[3] = create_bitmap(16, 16);
    blit(testimage, item_sprites[3], 80, 0, 0, 0, 16, 16);
    item_sprites[4] = create_bitmap(16, 16);
    blit(testimage, item_sprites[4], 96, 0, 0, 0, 16, 16);

    set_palette(palette);

    SAMPLE *snd_take_item = load_wav("snd_take.wav");
    SAMPLE *snd_drop_item = load_wav("snd_drop.wav");

    int wait = 0;
    int k = 0;

    int inventory[16];
    int held_item = 0;
    for (int i = 0; i < 16; i++)
    {
        inventory[i] = 0;
    }

    inventory[3] = 1;
    inventory[4] = 2;
    inventory[5] = 5;
    inventory[12] = 3;
    inventory[13] = 4;

    set_mouse_sprite(NULL);

    int active = 0;

    do
    {
        k = getkey();
        switch (k)
        {
        case KEY_ESC:
            return 0;
        default:
            break;
        };

        if ((mouse_b & 1) && !active)
        {
            active = 1;
            int invposx = mouse_x / 18;
            int invposy = mouse_y / 18;

            if (invposx < 8 && invposy < 2)
            {
                if (inventory[invposx + invposy * 8] != 0 && held_item == 0)
                {
                    held_item = inventory[invposx + invposy * 8];
                    inventory[invposx + invposy * 8] = 0;
                    play_sample(snd_take_item, 255, 128, 1000, 0);
                }
                else if (inventory[invposx + invposy * 8] == 0 && held_item != 0)
                {
                    inventory[invposx + invposy * 8] = held_item;
                    held_item = 0;
                    play_sample(snd_drop_item, 255, 128, 1000, 0);
                }
                else if (inventory[invposx + invposy * 8] != 0 && held_item != 0)
                {
                    int tmp = inventory[invposx + invposy * 8];
                    inventory[invposx + invposy * 8] = held_item;
                    held_item = tmp;
                    play_sample(snd_take_item, 255, 128, 1000, 0);
                }

                if (held_item != 0)
                {
                    set_mouse_sprite(item_sprites[held_item - 1]);
                }
                else
                {
                    set_mouse_sprite(NULL);
                }
            }
        }

        if (!mouse_b & 1)
        {
            active = 0;
        }

        clear_to_color(activepage, makecol(0, 0, 0));

        // Draw inventory slots
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                // rect(activepage, x * 18, y * 18, (x + 1) * 18 - 1, (y + 1) * 18 - 1, makecol(64, 64, 128));
                blit(testimage, activepage, 0, 0, x * 18, y * 18, 18, 18);
                if (inventory[x + y * 8] > 0)
                {
                    int spr = inventory[x + y * 8];
                    masked_blit(item_sprites[spr - 1], activepage, 0, 0, x * 18 + 1, y * 18 + 1, 16, 16);
                }
            }
        }

        // rect(activepage, invposx * 18, invposy * 18, (invposx + 1) * 18 - 1, (invposy + 1) * 18 - 1, makecol(255, 255, 255));

        if (held_item != 0)
        {
            textprintf_centre_ex(activepage, font, SCREEN_W / 2, 180,
                                 makecol(0, 100, 243), -1,
                                 "%s",
                                 repository[held_item - 1].name);
        }
        show_video_bitmap(activepage);
        show_mouse(activepage);

        if (activepage == page1)
        {
            activepage = page2;
        }
        else
        {
            activepage = page1;
        }

    } while (1);

    return 0;
}

int rendertest()
{
    maptile *map = allocate_map();
    maptile *selected_tile = 0;

    map[1].u_passable = 1;
    map[2].u_block_vision = 1;
    map[3].d_block_vision = 1;
    map[41].d_passable = 1;
    map[41].l_passable = 1;
    map[41].r_passable = 1;
    map[41].l_tile = 2;
    map[41].r_tile = 6;
    map[81].u_passable = 1;

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

    install_mouse();

    BITMAP *activepage = page1;
    PALETTE palette;

    // BITMAP *testimage = load_tga("testart.tga", 0);
    BITMAP *testimage = load_tga("testart.tga", palette);

    set_palette(palette);

    int wait = 0;
    int k = 0;

    int active = 0;
    int mode = 0; // SOLID, BLOCKS VISION, TILESET

    int sliceposa[] = {0, 8, 20};
    int sliceposb[] = {7, 19, 27};

    do
    {
        k = getkey();
        switch (k)
        {
        case KEY_Q:
            mode = 0;
            break;
        case KEY_W:
            mode = 1;
            break;
        case KEY_E:
            mode = 2;
            break;
        case KEY_ESC:
            return 0;
        default:
            break;
        };

        int blockposx = (mouse_x - 180) / 28;
        int blockposy = (mouse_y - 4) / 28;
        int posx = (mouse_x - 180) % 28;
        int posy = (mouse_y - 4) % 28;

        int slicex = 0;
        int slicey = 0;

        if (posx < 8)
        {
            slicex = 0;
        }
        else if (posx >= 20)
        {
            slicex = 2;
        }
        else
        {
            slicex = 1;
        }

        if (posy < 8)
        {
            slicey = 0;
        }
        else if (posy >= 20)
        {
            slicey = 2;
        }
        else
        {
            slicey = 1;
        }

        if ((mouse_b & 1) && !active)
        {
            active = 1;
        }

        if (!mouse_b & 1)
        {
            active = 0;
        }

        clear_to_color(activepage, makecol(0, 0, 0));

        rect(activepage, 3, 3, 3 + 176, 3 + 120, makecol(255, 255, 255));

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                rect(activepage,
                     180 + x * 28,
                     4 + y * 28,
                     180 + x * 28 + 27,
                     4 + y * 28 + 27, makecol(180, 180, 180));

                if (map[y * 40 + x].u_passable)
                {
                    rectfill(activepage,
                             180 + x * 28 + 8,
                             4 + y * 28,
                             180 + x * 28 + 19,
                             4 + y * 28 + 7, makecol(255, 255, 255));
                }
            }
        }

        if (blockposx >= 0 && blockposy >= 0)
        {
            rect(activepage,
                 180 + blockposx * 28,
                 4 + blockposy * 28,
                 180 + blockposx * 28 + 27,
                 4 + blockposy * 28 + 27, makecol(0, 0, 255));

            rect(activepage,
                 180 + blockposx * 28 + sliceposa[slicex],
                 4 + blockposy * 28 + sliceposa[slicey],
                 180 + blockposx * 28 + sliceposb[slicex],
                 4 + blockposy * 28 + sliceposb[slicey], makecol(128, 128, 255));

            selected_tile = &map[blockposx + blockposy * 40];
        }

        if (selected_tile != 0)
        {
            textprintf_ex(activepage, font, 2, 128,
                          makecol(0, 100, 243), -1,
                          "S",
                          0);

            if (selected_tile->u_passable)
            {
                textprintf_ex(activepage, font, 2, 140,
                              makecol(0, 100, 243), -1,
                              "u",
                              0);
            }

            if (selected_tile->d_passable)
            {
                textprintf_ex(activepage, font, 2, 140 + 16,
                              makecol(0, 100, 243), -1,
                              "d",
                              0);
            }

            if (selected_tile->l_passable)
            {
                textprintf_ex(activepage, font, 2, 140 + 32,
                              makecol(0, 100, 243), -1,
                              "l",
                              0);
            }

            if (selected_tile->r_passable)
            {
                textprintf_ex(activepage, font, 2, 140 + 48,
                              makecol(0, 100, 243), -1,
                              "r",
                              0);
            }

            textprintf_ex(activepage, font, 32, 128,
                          makecol(0, 132, 243), -1,
                          "BV",
                          0);

            if (selected_tile->u_block_vision)
            {
                textprintf_ex(activepage, font, 32, 140,
                              makecol(0, 132, 243), -1,
                              "u",
                              0);
            }

            if (selected_tile->d_block_vision)
            {
                textprintf_ex(activepage, font, 32, 140 + 16,
                              makecol(0, 132, 243), -1,
                              "d",
                              0);
            }

            if (selected_tile->l_block_vision)
            {
                textprintf_ex(activepage, font, 32, 140 + 32,
                              makecol(0, 132, 243), -1,
                              "l",
                              0);
            }

            if (selected_tile->r_block_vision)
            {
                textprintf_ex(activepage, font, 32, 140 + 48,
                              makecol(0, 132, 243), -1,
                              "r",
                              0);
            }

            textprintf_ex(activepage, font, 62, 128,
                          makecol(0, 132, 243), -1,
                          "ID",
                          0);

            if (selected_tile->u_tile)
            {
                textprintf_ex(activepage, font, 62, 140,
                              makecol(0, 132, 243), -1,
                              "%d",
                              selected_tile->u_tile);
            }

            if (selected_tile->d_tile)
            {
                textprintf_ex(activepage, font, 62, 140 + 16,
                              makecol(0, 132, 243), -1,
                              "%d",
                              selected_tile->d_tile);
            }

            if (selected_tile->l_tile)
            {
                textprintf_ex(activepage, font, 62, 140 + 32,
                              makecol(0, 132, 243), -1,
                              "%d",
                              selected_tile->l_tile);
            }

            if (selected_tile->r_tile)
            {
                textprintf_ex(activepage, font, 62, 140 + 48,
                              makecol(0, 132, 243), -1,
                              "%d",
                              selected_tile->r_tile);
            }
        }

        show_video_bitmap(activepage);
        show_mouse(activepage);

        if (activepage == page1)
        {
            activepage = page2;
        }
        else
        {
            activepage = page1;
        }

    } while (1);
    return 0;
}

int main(int argc, const char **argv)
{
    if (allegro_init() != 0)
    {
        return 1;
    }

    install_keyboard();
    install_timer();

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
    {
        allegro_message("Error initialising sound system\n%s\n", allegro_error);
        return 1;
    }

    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    printf("1) Run game\n");
    printf("2) Music test\n");
    printf("3) Inventory simulation\n");
    printf("4) Rendering Test\n");

    clear_keybuf();
    while (!keypressed())
    {
    };

    if (key[KEY_1])
    {
        gameloop();
    }
    if (key[KEY_2])
    {
        musictest();
    }
    if (key[KEY_3])
    {
        inventorytest();
    }
    if (key[KEY_4])
    {
        rendertest();
    }
}

END_OF_MAIN()