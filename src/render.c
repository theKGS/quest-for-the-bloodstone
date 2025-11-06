#include "render.h"

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
