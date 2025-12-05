#ifndef ITEM_H
#define ITEM_H

#include <allegro.h>

#define TYPE_POTION 0
#define TYPE_WEAPON 1
#define TYPE_SPECIAL 2

// Item info object stored in the item repository and referred to 
// by each object instance. It contains all information about the
// object.
typedef struct
{
    char *name;
    char *descr;
    BITMAP *sprite;
} item_info;

// The object used to represent an item in the world or on a character. It
// contains a reference to an object in the repository, a "pointer" t
typedef struct {
    char ref; // pointer to item
    char meta; // possible metadata
    char next;
} item_reference;

extern item_info repository[];

#endif