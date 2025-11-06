#ifndef ENEMY_H
#define ENEMY_H

#include <allegro.h>

struct enemy {
    BITMAP *picture;
};

struct squad {
    struct enemy* units[4];
    int x;
    int y;
};


#endif