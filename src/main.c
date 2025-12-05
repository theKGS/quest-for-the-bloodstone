/*
 * Copyright (C) 2025 - KGS
 * MIT License
 */

#define DEBUGMODE
#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "render.h"
#include "utils.h"
#include "enemy.h"
#include "item.h"
#include "state.h"

int gameloop()
{
    State *state = malloc(sizeof(State));

    state->dir = F_SOUTH;
    state->px = 4;
    state->py = 4;

    int triggers = 0;
    maptile *map = allocate_map();
    srand(0);

    randomise_map(map);

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

    // play_midi(music, TRUE);

    PALETTE palette;
    BITMAP *page1;
    BITMAP *page2;

    tileset *tset = load_tileset_from_file("set1.tst", "walls1.tga", palette);

    page1 = create_video_bitmap(320, 200);
    page2 = create_video_bitmap(320, 200);

    if ((!page1) || (!page2))
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to create two video memory pages\n");
        return 1;
    }

    BITMAP *activepage = page1;

    SAMPLE *snd_take_item = load_wav("snd_tak2.wav");

    set_palette(palette);

    int k;
    int wait = 0;

    do
    {
        k = getkey();
        switch (k)
        {
        case KEY_W:
            wait = 1;
            state->px += get_x_move_offset(state->dir);
            state->py += get_y_move_offset(state->dir);
            break;
        case KEY_S:
            wait = 1;
            state->px -= get_x_move_offset(state->dir);
            state->py -= get_y_move_offset(state->dir);
            break;
        case KEY_Q:
            wait = 1;
            state->dir = get_prev_facing(state->dir);
            break;
        case KEY_E:
            wait = 1;
            state->dir = get_next_facing(state->dir);
            break;
        case KEY_A:
            wait = 1;
            state->px += get_y_move_offset(state->dir);
            state->py -= get_x_move_offset(state->dir);
            break;
        case KEY_D:
            wait = 1;
            state->px -= get_y_move_offset(state->dir);
            state->py += get_x_move_offset(state->dir);
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
                if (issolid(map, state->px + xp - MAP_SIGHT, state->py + yp - MAP_SIGHT))
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
                 (position_offset_to_x(state->px, state->dir, -1, 0)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, -1, 0)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, -1, 0)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, -1, 0)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, 1, 0)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, 1, 0)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, 1, 0)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, 1, 0)) * 4 + 2, 6);

        // Z depth = 1
        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, 0, 1)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, 0, 1)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, 0, 1)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, 0, 1)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, -1, 1)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, -1, 1)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, -1, 1)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, -1, 1)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, 1, 1)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, 1, 1)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, 1, 1)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, 1, 1)) * 4 + 2, 6);

        // Z depth = 1
        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, 0, 2)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, 0, 2)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, 0, 2)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, 0, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, -1, 2)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, -1, 2)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, -1, 2)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, -1, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, -2, 2)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, -2, 2)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, -2, 2)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, -2, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, 1, 2)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, 1, 2)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, 1, 2)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, 1, 2)) * 4 + 2, 6);

        rectfill(activepage,
                 (position_offset_to_x(state->px, state->dir, 2, 2)) * 4 + 1,
                 (position_offset_to_y(state->py, state->dir, 2, 2)) * 4 + 1,
                 (position_offset_to_x(state->px, state->dir, 2, 2)) * 4 + 2,
                 (position_offset_to_y(state->py, state->dir, 2, 2)) * 4 + 2, 6);

        // Draw viewport frame
        rect(activepage, VIEW_X - 1, VIEW_Y - 1, VIEW_X + 176, VIEW_Y + 120, makecol(255, 0, 0));

        if (wait)
        {
            triggers += 1;
            // play_sample(snd_take_item, 255, 128, 1000, 0);
        }

        render_view(activepage, map, tset, VIEW_X, VIEW_Y, state->px, state->py, state->dir);

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
    printf("4) Track 4");
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

        if (key[KEY_4])
        {
            song = load_midi("bsmus04.mid");
            play_midi(song, 1);
        }


        clear_keybuf();
    } while (!key[KEY_ESC]);

    return 0;
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

    map[0].tile_wall[F_EAST] = 1;
    map[1].solid_wall[F_NORTH] = 1;
    map[2].seethrough_wall[F_NORTH] = 1;
    map[3].seethrough_wall[F_NORTH] = 1;
    map[41].solid_wall[F_NORTH] = 1;
    map[41].solid_wall[F_EAST] = 1;
    map[41].solid_wall[F_WEST] = 1;
    map[41].tile_wall[F_SOUTH] = 2;
    map[41].tile_wall[F_SOUTH] = 6;
    map[81].solid_wall[F_NORTH] = 1;
    map[40 * 2 + 3].tile_wall[F_NORTH] = 1;
    map[40 * 2 + 4].tile_wall[F_SOUTH] = 1;
    map[40 * 1 + 2].tile_wall[F_EAST] = 1;

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

    tileset *tset = load_tileset_from_file("set1.tst", "walls1.tga", palette);

    if (!tset->flat_tile_1->bitmap)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Failed to load image\n");
        return 1;
    }

    set_palette(palette);

    int k = 0;

    int active = 0;
    int mode = 0; // SOLID, BLOCKS VISION, TILESET

    int sliceposa[] = {0, 8, 20};
    int sliceposb[] = {7, 19, 27};

    int px = 3;
    int py = 3;
    int pdir = F_NORTH;

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
            selected_tile = &map[blockposx + blockposy * 40];
            if (mode == 0)
            {
                if (posy < 8 && posx > 7 && posx < 20)
                {
                    selected_tile->tile_wall[F_NORTH] = !selected_tile->tile_wall[F_NORTH];
                }
                else if (posy > 19 && posx > 7 && posx < 20)
                {
                    selected_tile->tile_wall[F_SOUTH] = !selected_tile->tile_wall[F_SOUTH];
                }
                else if (posx < 8 && posy > 7 && posy < 20)
                {
                    selected_tile->tile_wall[F_WEST] = !selected_tile->tile_wall[F_WEST];
                }
                else if (posx > 19 && posy > 7 && posy < 20)
                {
                    selected_tile->tile_wall[F_EAST] = !selected_tile->tile_wall[F_EAST];
                }
            }

            active = 1;
        }

        if (!mouse_b & 1)
        {
            active = 0;
        }

        clear_to_color(activepage, makecol(0, 0, 0));

        char wshow[23];
        char whidden[23];

        render_view(activepage, map, tset, VIEW_X, VIEW_Y, px, py, pdir);

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                rect(activepage,
                     180 + x * 28,
                     4 + y * 28,
                     180 + x * 28 + 27,
                     4 + y * 28 + 27, makecol(180, 180, 180));

                for (int d = 0; d < 4; d++)
                {
                    if (map[y * 40 + x].tile_wall[d])
                    {
                        rectfill(activepage,
                                 180 + x * 28 + 10 + get_x_move_offset(d) * 10,
                                 4 + y * 28 + 10 + get_y_move_offset(d) * 10,
                                 180 + x * 28 + 10 + get_x_move_offset(d) * 10 + 7,
                                 4 + y * 28 + 10 + get_y_move_offset(d) * 10 + 7, makecol(255, 255, 255));
                    }
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

        int mode1col = mode == 0 ? makecol(0, 255, 255) : makecol(90, 90, 90);
        int mode2col = mode == 1 ? makecol(0, 255, 255) : makecol(90, 90, 90);
        int mode3col = mode == 2 ? makecol(0, 255, 255) : makecol(90, 90, 90);

        /*         if (selected_tile != 0)
                {
                    textprintf_ex(activepage, font, 2, 128,
                                  mode1col, -1,
                                  "S");

                    if (selected_tile->u_solid)
                    {
                        textprintf_ex(activepage, font, 2, 140,
                                      makecol(0, 100, 243), -1,
                                      "u");
                    }

                    if (selected_tile->d_solid)
                    {
                        textprintf_ex(activepage, font, 2, 140 + 16,
                                      makecol(0, 100, 243), -1,
                                      "d");
                    }

                    if (selected_tile->l_solid)
                    {
                        textprintf_ex(activepage, font, 2, 140 + 32,
                                      makecol(0, 100, 243), -1,
                                      "l");
                    }

                    if (selected_tile->r_solid)
                    {
                        textprintf_ex(activepage, font, 2, 140 + 48,
                                      makecol(0, 100, 243), -1,
                                      "r");
                    }

                    textprintf_ex(activepage, font, 32, 128,
                                  mode2col, -1,
                                  "BV");

                    if (selected_tile->u_seethrough)
                    {
                        textprintf_ex(activepage, font, 32, 140,
                                      makecol(0, 132, 243), -1,
                                      "u");
                    }

                    if (selected_tile->d_seethrough)
                    {
                        textprintf_ex(activepage, font, 32, 140 + 16,
                                      makecol(0, 132, 243), -1,
                                      "d");
                    }

                    if (selected_tile->l_seethrough)
                    {
                        textprintf_ex(activepage, font, 32, 140 + 32,
                                      makecol(0, 132, 243), -1,
                                      "l");
                    }

                    if (selected_tile->r_seethrough)
                    {
                        textprintf_ex(activepage, font, 32, 140 + 48,
                                      makecol(0, 132, 243), -1,
                                      "r");
                    }

                    textprintf_ex(activepage, font, 62, 128,
                                  mode3col, -1,
                                  "ID");

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
         */
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

int tileedit()
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
    if (key[KEY_5])
    {
        tileedit();
    }
}

END_OF_MAIN()