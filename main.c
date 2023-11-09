#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "display.h"
#include "sekil.h"
#include "control.h"
#include "optimum.h"

//Extern değişkenleri başka dosyalarda dinamik olarak kullanmak ve işlemek için kullandık.
extern FILE *fp;
extern FILE *fpkaynak;

int count = 0;
extern int platform;
extern uint32_t framebuffer_copy[SCREEN_WIDTH * SCREEN_HEIGHT];
extern uint32_t green_color;
extern uint32_t red_color;
extern uint32_t blue_color;
extern uint32_t yellow_color;
extern uint32_t pink_color;

int area1 = 0;
int area2 = 0;
int area3 = 0;
bool is_running1 = false;
bool is_running2 = false;
extern vec2_t vertices[100];
extern vec2_t sekil1[100];
extern vec2_t sekil2[100];
extern vec2_t sekil3[100];
extern int sekil1Index;
extern int sekil2Index;
extern int sekil3Index;

int gr, bl, re, ye, pi;

void process_input(void)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      is_running1 = false;
      is_running2 = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        is_running1 = false;
        is_running2 = false;
      }
      break;
    }
  }
}

int main(void)
{
  // curl kütüphanesi ile istenilen satırı dosya işlerimlerine aktaran fonksiyon.
  curl_main();

  // Kullanıcıdan isterleri alıyoruz.
  int unit_drilling, unit_platform;
  printf("Birim sondaj miktarini girin:");
  scanf("%d", &unit_drilling);
  printf("Birim platform maliyetini girin:");
  scanf("%d", &unit_platform);

  is_running1 = true;
  is_running2 = true;
  create_window();

  // Koordinatları dosyadan ayıklıyoruz.
  const char *hedefDosyaYolu = "kullanicinin_istedigi.txt";
  separate_coordinates(hedefDosyaYolu);

  int n = sizeof(vertices) / sizeof(vertices[0]);

  // Ayıkladığımız koordinatların tekrarına kadar ( ve , ile ayrım yapan fonksiyonumuz çağırıyoruz.
  find_shape(vertices, n, sekil1, &sekil1Index, sekil2, &sekil2Index, sekil3, &sekil3Index);

  // Sekillerin koordinatlarını bulmak isterseniz aşağıdaki yorum satırını kaldırınız.
  // coordinate_control();

  // Alanlar (Şekilllerin alanlarını görmek isterseniz yorum satırlarını kaldırın.)
  if (sekil1Index != 0)
  {
    area1 = (fill_polygon(sekil1, sekil1Index, 0, 0) - (sekil1Index - 3));
    // printf("Sekil1 Alan: %d\n",area1);
  }
  if (sekil2Index != 0)
  {
    area2 = (fill_polygon(sekil2, sekil2Index, 0, 0) - (sekil2Index - 3));
    // printf("Sekil2 Alan: %d\n",area2);
  }
  if (sekil3Index != 0)
  {
    area3 = (fill_polygon(sekil3, sekil3Index, 0, 0) - (sekil3Index - 3));
    // printf("Sekil3 Alan: %d\n",area3);
  }
  int platform;
  int sum_area = area1 + area2 + area3;
  printf("Toplam Alan : %d\n", sum_area);
  int rezerv = sum_area * 10;
  printf("Rezerv Değeri: %d\n", rezerv);

  bool optimum_calculated = false;
  while (is_running1 && is_running2)
  {
    fix_framerate();
    process_input();

    // Pencere 1 için (Sadece şekli göster):
    clear_framebuffer(0xFF000000, 1);
    // Eğer sınırlayan dikdörtgeni görmek isterseniz yorum satırını kaldırın.
    /*draw_gray(sekil1, sekil1Index, 0x11111111, 1);
    draw_gray(sekil2, sekil2Index, 0x11111111, 1);
    draw_gray(sekil3, sekil3Index, 0x11111111, 1);*/

    //
    fill_polygon(sekil1, sekil1Index, 0xFF00FF00, 1);
    fill_polygon(sekil2, sekil2Index, 0xFF00FF00, 1);
    fill_polygon(sekil3, sekil3Index, 0xFF00FF00, 1);

    // Pencere 2 için (Optimum işlemleri):
    clear_framebuffer(0xFF000000, 2);

    // Eğer sınırlayan dikdörtgeni görmek isterseniz yorum satırını kaldırın.
    /*draw_gray(sekil1, sekil1Index, 0x11111111, 2);
    draw_gray(sekil2, sekil2Index, 0x11111111, 2);
    draw_gray(sekil3, sekil3Index, 0x11111111, 2);*/

    fill_polygon(sekil1, sekil1Index, 0xFF00FF00, 2);
    fill_polygon(sekil2, sekil2Index, 0xFF00FF00, 2);
    fill_polygon(sekil3, sekil3Index, 0xFF00FF00, 2);
    copy_framebuffer(2);

    convert_2x2_green_to_blue();
    convert_4x4_blue_to_red();
    convert_4x4_red_to_8x8_yellow();
    convert_8x8_yellow_to_16x16_pink();

    copy_framebuffer(2);
    if (!optimum_calculated)
    {
      for (int y = 0; y < SCREEN_HEIGHT; y++)
      {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
          // Get the color of the current pixel.
          uint32_t color = framebuffer_copy[y * SCREEN_WIDTH + x];

          // Check the color and increment the respective counter.
          if (color == green_color)
          {
            gr++;
          }
          else if (color == blue_color)
          {
            bl++;
          }
          else if (color == red_color)
          {
            re++;
          }
          else if (color == yellow_color)
          {
            ye++;
          }
          else if (color == pink_color)
          {
            pi++;
          }
        }
      }
      int g = 0;
      int b = 0;
      int r = 0;
      int y = 0;
      int p = 0;

      if (gr != 0)
      {
        g = gr;
      }
      if (bl != 0)
      {
        b = bl / 4;
      }
      if (re != 0)
      {
        r = re / 16;
      }
      if (ye != 0)
      {
        y = ye / 64;
      }
      if (pi != 0)
      {
        p = pi / 256;
      }

      platform = g + b + r + y + p;
      int drilling_cost = unit_drilling * sum_area;
      int platform_cost = platform * unit_platform;

      int sum_less = drilling_cost + platform_cost;
      int profit = rezerv - sum_less;

      printf("Toplam kariniz :%d", profit);

      optimum_calculated = true;
    }

    render_framebuffer(2);
    clear_screen(1);
    clear_screen(2);
  }

  destroy_window();

  return EXIT_SUCCESS;
}