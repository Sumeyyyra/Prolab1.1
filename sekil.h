#ifndef SEKIL_H
#define SEKIL_H

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
  int x;
  int y;
} vec2_t;

size_t yazdir(void *ptr, size_t boyut, size_t nmemb, void *stream);
void target_row(const char *kaynakDosyaYolu, const char *hedefDosyaYolu, int istenilen_satir);
bool coordinate_check(vec2_t k1, vec2_t k2);
int curl_main(void);

void separate_coordinates(const char *hedefDosyaYolu);
void find_shape(vec2_t vertices[], int n, vec2_t sekil1[], int *sekil1Index, vec2_t sekil2[], int *sekil2Index, vec2_t sekil3[], int *sekil3Index);
bool is_point_inside_polygon(vec2_t *polygon, int n, vec2_t point);
void draw_gray(vec2_t sekil[], int sekilIndex, uint32_t color, int window_number);
int fill_polygon(vec2_t *polygon, int n, uint32_t color, int window_number);

#endif // SEKIL_H
