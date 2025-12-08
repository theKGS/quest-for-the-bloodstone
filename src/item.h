#ifndef ITEM_H
#define ITEM_H

#include <allegro.h>

// Item info object stored in the item repository and referred to 
// by each object instance. It contains all information about the
// object.
typedef struct
{
    char *name;
    char *descr;
    BITMAP *sprite;
} Item_info;

// The object used to represent an item in the world or on a character. It
// contains a reference to an object in the repository, a "pointer" t
typedef struct {
    unsigned char ref; // pointer to item entry
    unsigned char meta; // possible metadata
    unsigned char next; // pointer to next item in list
} Item_reference;

extern Item_info repository[];

void load_item_sprites();

#endif