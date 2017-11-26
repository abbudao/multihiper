#ifndef _COLOR_CHANNEL_H
#define _COLOR_CHANNEL_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct  {
  unsigned char **R;
  unsigned char **G;
  unsigned char **B;
  int height;
  int width;
} RGBChannel;
typedef struct  {
  unsigned char **Y;
  unsigned char **Cb;
  unsigned char **Cr;
  int height;
  int width;
} YCbCrChannel_UC;
typedef struct  {
  double **Y;
  double **Cb;
  double **Cr;
  int height;
  int width;
} YCbCrChannel_Double;
typedef struct{
  double **block;
}Block;
static const double quantization_Y[8][8]={
  {16, 11, 10, 16, 24,  40,  51,  61},
  {12, 12, 14, 19, 26,  58,  60,  55},
  {14, 13, 16, 24, 40,  57,  69,  56},
  {14, 17, 22, 29, 51,  87,  80,  62},
  {18, 22, 37, 56, 68,  109, 103, 77},
  {24, 35, 55, 64, 81,  104, 113, 92},
  {49, 64, 78, 87, 103, 121, 120, 101},
  {72, 92, 95, 98, 112, 100, 103, 99}};
static const double quantization_Cb[8][8]={
  {16, 11, 10, 16, 24,  40,  51,  61},
  {12, 12, 14, 19, 26,  58,  60,  55},
  {14, 13, 16, 24, 40,  57,  69,  56},
  {14, 17, 22, 29, 51,  87,  80,  62},
  {18, 22, 37, 56, 68,  109, 103, 77},
  {24, 35, 55, 64, 81,  104, 113, 92},
  {49, 64, 78, 87, 103, 121, 120, 101},
  {72, 92, 95, 98, 112, 100, 103, 99}};
static const double quantization_Cr[8][8]={
  {16, 11, 10, 16, 24,  40,  51,  61},
  {12, 12, 14, 19, 26,  58,  60,  55},
  {14, 13, 16, 24, 40,  57,  69,  56},
  {14, 17, 22, 29, 51,  87,  80,  62},
  {18, 22, 37, 56, 68,  109, 103, 77},
  {24, 35, 55, 64, 81,  104, 113, 92},
  {49, 64, 78, 87, 103, 121, 120, 101},
  {72, 92, 95, 98, 112, 100, 103, 99}};


static const double dct_table[8][8]={
  {1,   19,  37,  55,  73,  91,  109, 127},
  {19,  37,  55,  73,  91,  109, 127, 145},
  {37,  55,  73,  91,  109, 127, 145, 163},
  {55,  73,  91,  109, 127, 145, 163, 181},
  {73,  91,  109, 127, 145, 163, 181, 199},
  {91,  109, 127, 145, 163, 181, 199, 217},
  {109, 127, 145, 163, 181, 199, 217, 235},
  {127, 145, 163, 181, 199, 217, 235, 253}};

#endif
