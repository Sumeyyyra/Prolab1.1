
#include <stdio.h>
#include "sekil.h"

vec2_t sekil1[100];
vec2_t sekil2[100];
vec2_t sekil3[100];
int sekil1Index;
int sekil2Index;
int sekil3Index;

void coordinate_control()
{
  printf("Sekil 1 Koordinatlari:\n");
  for (int i = 0; i < sekil1Index; i++)
  {
    printf("(%d,%d) ", sekil1[i].x, sekil1[i].y);
  }
  printf("\n");

  printf("Sekil 2 Koordinatlari:\n");
  for (int i = 0; i < sekil2Index; i++)
  {
    printf("(%d,%d) ", sekil2[i].x, sekil2[i].y);
  }
  printf("\n");

  printf("Sekil 3 Koordinatlari:\n");
  for (int i = 0; i < sekil3Index; i++)
  {
    printf("(%d,%d) ", sekil3[i].x, sekil3[i].y);
  }
  printf("\n");
}