#ifndef UTILS_H
#define UTILS_H

extern unsigned char slot_index_fl[];
extern unsigned char slot_index_fr[];
extern unsigned char slot_index_cl[];
extern unsigned char slot_index_cr[];

int getkey();

int get_x_move_offset(int facing);
int get_y_move_offset(int facing);

int get_next_facing(int facing);
int get_prev_facing(int facing);

int position_offset_to_x(int posx, int dirp, int lr_steps, int fb_steps);
int position_offset_to_y(int posy, int dirp, int lr_steps, int fb_steps);

int depth_position(int posx, int posy, int dirp, int ex, int ey);
int horizontal_position(int posx, int posy, int dirp, int ex, int ey);

#endif