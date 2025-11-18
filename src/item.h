#ifndef ITEM_H
#define ITEM_H

#include <allegro.h>

#define TYPE_POTION 0
#define TYPE_WEAPON 1
#define TYPE_SPECIAL 2

typedef struct
{
    char *name;
    char *descr;
    BITMAP *sprite;
} item;

extern item repository[];


#endif