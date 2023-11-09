#ifndef OPTIMUM_H
#define OPTIMUM_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

void copy_framebuffer(int window_number);
bool is_4x4_square_blue(int x, int y);
bool is_2x2_square_green(int x, int y);
bool is_8x8_square_red(int x, int y);
bool is_16x16_square_yellow(int x, int y);
bool are_neighbors_not_colored(int x, int y, int color1, int color2, int color3, int square_size);

void convert_4x4_blue_to_red();
void convert_2x2_green_to_blue();
void convert_4x4_red_to_8x8_yellow();
void convert_8x8_yellow_to_16x16_pink();

#endif