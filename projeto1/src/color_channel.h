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
  double values[8][8];
}Block;
typedef struct{
Block **Y_block;
Block **Cb_block;
Block **Cr_block;
int width;
int height;
int source_width;
int source_height;
}BlockMatrix;
typedef struct{
unsigned char *vector;
int size;
int block_height;
int block_width;
int next_index;
}ZigZagVector;
#endif
