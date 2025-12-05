#ifndef STATE_H
#define STATE_H

typedef struct
{
    signed char px;
    signed char py;
    char dir;

    int inventory_1[8];
    int inventory_2[8];
    int inventory_3[8];
    int inventory_4[8];
} State;

#endif