#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#define WINDOW1_WIDTH 128
#define WINDOW1_HEIGHT 128

#define WINDOW2_WIDTH 128
#define WINDOW2_HEIGHT 128

#define FPS 30
#define MILLISECS_PER_FRAME (1000 / FPS)

bool create_window(void);
void destroy_window(void);

void fix_framerate(void);

void clear_framebuffer(uint32_t color, int window_number);
void render_framebuffer(int window_number);
void draw_pixel(uint8_t x, uint8_t y, uint32_t color, int window_number);
void clear_screen(int window_number);
void fix_framerate(void);

uint32_t *get_framebuffer(int window_number);

#endif
