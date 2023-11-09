#include "display.h"

static SDL_Window *window1 = NULL;
static SDL_Window *window2 = NULL;
static SDL_Renderer *renderer1 = NULL;
static SDL_Renderer *renderer2 = NULL;

static uint32_t *framebuffer1 = NULL;
static uint32_t *framebuffer2 = NULL;

static SDL_Texture *framebuffer_texture1 = NULL;
static SDL_Texture *framebuffer_texture2 = NULL;
int i = 0;

static uint16_t window_width = 800;
static uint16_t window_height = 800;

bool create_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // İlk pencere için yapıların oluşturulması
    window1 = SDL_CreateWindow("Pencere 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
    renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_PRESENTVSYNC);
    framebuffer1 = malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
    framebuffer_texture1 = SDL_CreateTexture(renderer1, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    // İkinci pencere için yapıların oluşturulması
    window2 = SDL_CreateWindow("Pencere 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
    renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_PRESENTVSYNC);
    framebuffer2 = malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
    framebuffer_texture2 = SDL_CreateTexture(renderer2, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    return true;
}

void draw_pixel(uint8_t x, uint8_t y, uint32_t color, int window_number)
{
    uint32_t *target_framebuffer = (window_number == 1) ? framebuffer1 : framebuffer2;

    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
    {
        return;
    }
    target_framebuffer[(SCREEN_WIDTH * y) + x] = color;
}

void render_framebuffer(int window_number)
{
    SDL_Renderer *target_renderer = (window_number == 1) ? renderer1 : renderer2;
    SDL_Texture *target_texture = (window_number == 1) ? framebuffer_texture1 : framebuffer_texture2;
    uint32_t *target_framebuffer = (window_number == 1) ? framebuffer1 : framebuffer2;

    SDL_UpdateTexture(target_texture, NULL, target_framebuffer, (int)(SCREEN_WIDTH * sizeof(uint32_t)));
    SDL_RenderCopy(target_renderer, target_texture, NULL, NULL);
    SDL_RenderPresent(target_renderer);
}

void clear_framebuffer(uint32_t color, int window_number)
{
    uint32_t *target_framebuffer = (window_number == 1) ? framebuffer1 : framebuffer2;

    for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        target_framebuffer[i] = color;
    }
}

void clear_screen(int window_number)
{
    SDL_Renderer *target_renderer = (window_number == 1) ? renderer1 : renderer2;

    SDL_SetRenderDrawColor(target_renderer, 0, 0, 0, 255);
    SDL_RenderClear(target_renderer);
}

void destroy_window(void)
{
    free(framebuffer1);
    free(framebuffer2);
    SDL_DestroyTexture(framebuffer_texture1);
    SDL_DestroyTexture(framebuffer_texture2);
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window1);
    SDL_DestroyWindow(window2);
    SDL_Quit();
}
void fix_framerate(void)
{
    static int32_t previous_frame_time = 0;
    int time_ellapsed = SDL_GetTicks() - previous_frame_time;
    int time_to_wait = MILLISECS_PER_FRAME - time_ellapsed;
    if (time_to_wait > 0 && time_to_wait < MILLISECS_PER_FRAME)
    {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();
}

uint32_t *get_framebuffer(int window_number)
{
    return (window_number == 1) ? framebuffer1 : framebuffer2;
}
