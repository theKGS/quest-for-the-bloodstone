#ifndef RENDER_H
#define RENDER_H

#define VIEW_WIDTH 176
#define VIEW_HEIGHT 120
#define VIEW_X 320 - 1 - VIEW_WIDTH
#define VIEW_Y 64
#define VIEW_MID_X VIEW_X + VIEW_WIDTH / 2
#define VIEW_MID_Y VIEW_Y + VIEW_HEIGHT / 2

#include <allegro.h>

void drawcube(BITMAP *screen, int x, int z);

#endif