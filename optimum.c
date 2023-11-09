#include "optimum.h"
#include "display.h"
#include "sekil.h"

uint32_t framebuffer_copy[SCREEN_WIDTH * SCREEN_HEIGHT];
uint32_t green_color = 0xFF00FF00;
uint32_t red_color = 0xFF0000FF;
uint32_t blue_color = 0xFFFF0000;
uint32_t yellow_color = 0xFF00FFFF;
uint32_t pink_color = 0xFF8515C7;

void copy_framebuffer(int window_number)
{
  uint32_t *current_framebuffer = get_framebuffer(window_number);
  for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
  {
    framebuffer_copy[i] = current_framebuffer[i];
  }
}
bool is_2x2_square_green(int x, int y)
{ // 2*2lik boyuttaki karelerin rengini kontrol ediyor ,x y ile başlayan.
  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 2; i++)
    {
      int index = (y + j) * SCREEN_WIDTH + (x + i);
      if (framebuffer_copy[index] != green_color)
      {
        return false;
      }
    }
  }
  return true;
}
bool is_4x4_square_blue(int x, int y)
{
  for (int j = 0; j < 4; j++)
  {
    for (int i = 0; i < 4; i++)
    {
      int index = (y + j) * SCREEN_WIDTH + (x + i);
      if (framebuffer_copy[index] != blue_color)
      {
        return false;
      }
    }
  }
  return true;
}

bool is_8x8_square_red(int x, int y)
{
  for (int j = 0; j < 8; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      int index = (y + j) * SCREEN_WIDTH + (x + i);
      if (framebuffer_copy[index] != red_color)
      {
        return false;
      }
    }
  }
  return true;
}
bool is_16x16_square_yellow(int x, int y)
{
  for (int j = 0; j < 16; j++)
  {
    for (int i = 0; i < 16; i++)
    {
      int index = (y + j) * SCREEN_WIDTH + (x + i);
      if (framebuffer_copy[index] != yellow_color)
      {
        return false;
      }
    }
  }
  return true;
}
bool are_neighbors_not_colored(int x, int y, int color1, int color2, int color3, int square_size)
{
  // Kenar komşularını kontrol etmek için sınırları aşmamak önemli.
  int startX = (x > 0) ? x - 1 : x;
  int startY = (y > 0) ? y - 1 : y;
  int endX = (x < SCREEN_WIDTH - square_size) ? x + square_size : SCREEN_WIDTH - 1;
  int endY = (y < SCREEN_HEIGHT - square_size) ? y + square_size : SCREEN_HEIGHT - 1;

  // Kenar komşularını kontrol ediyoruz.
  for (int j = startY; j <= endY; j++)
  {
    for (int i = startX; i <= endX; i++)
    {
      // Kare içindeki pikselleri atla.
      if (i >= x && i < x + square_size && j >= y && j < y + square_size)
        continue;

      int index = j * SCREEN_WIDTH + i;
      int color = framebuffer_copy[index];
      if (color != color1 && color != color2 && color != color3)
      {
        return false; // Eğer herhangi bir kenar komşusu istenilen renklerden değilse.
      }
    }
  }

  return true; // Tüm kenar komşuları istenilen renklerdeyse.
}

void convert_2x2_green_to_blue()
{

  copy_framebuffer(2);
  for (int y = 0; y < SCREEN_HEIGHT - 1; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH - 1; x++)
    {
      if (is_2x2_square_green(x, y) && are_neighbors_not_colored(x, y, green_color, blue_color, green_color, 2))
      {
        for (int j = 0; j < 2; j++)
        {
          for (int i = 0; i < 2; i++)
          {
            draw_pixel(x + i, y + j, blue_color, 2);
          }
        }

        x += 1; // 2x2 kareyi boyadıktan sonra bir sonraki kontrol noktasına geç
      }
    }
    y += 1; // Her satırın sonunda bir sonraki kontrol noktasına geç
  }
}

void convert_4x4_blue_to_red()
{
  copy_framebuffer(2);
  for (int y = 0; y < SCREEN_HEIGHT - 3; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH - 3; x++)
    {
      if (is_4x4_square_blue(x, y) && are_neighbors_not_colored(x, y, red_color, blue_color, yellow_color, 4))
      {
        for (int j = 0; j < 4; j++)
        {
          for (int i = 0; i < 4; i++)
          {
            draw_pixel(x + i, y + j, red_color, 2);
          }
        }

        x += 3;
      }
    }
    y += 3;
  }
}

void convert_4x4_red_to_8x8_yellow()
{
  copy_framebuffer(2);
  for (int y = 0; y < SCREEN_HEIGHT - 3; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH - 3; x++)
    {
      if (is_8x8_square_red(x, y) && are_neighbors_not_colored(x, y, red_color, yellow_color, pink_color, 8))
      {
        for (int j = 0; j < 8; j++)
        {
          for (int i = 0; i < 8; i++)
          {
            draw_pixel(x + i, y + j, yellow_color, 2);
          }
        }

        x += 7;
      }
    }
    y += 7;
  }
}

void convert_8x8_yellow_to_16x16_pink()
{
  copy_framebuffer(2);
  for (int y = 0; y < SCREEN_HEIGHT - 15; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH - 15; x++)
    {
      if (is_16x16_square_yellow(x, y) && are_neighbors_not_colored(x, y, pink_color, yellow_color, pink_color, 16))
      {
        for (int j = 0; j < 16; j++)
        {
          for (int i = 0; i < 16; i++)
          {
            draw_pixel(x + i, y + j, pink_color, 2);
          }
        }

        x += 15; // 16x16 bloğu boyadıktan sonra bir sonraki kontrol noktasına geç
      }
    }
    y += 15; // Her satırın sonunda bir sonraki kontrol noktasına geç
  }
}
