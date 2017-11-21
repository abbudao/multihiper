#ifndef _BMP_H_
#define _BMP_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**** BMP file header structure ****/
typedef struct {
  unsigned short bfType; /* Magic number for file */
} BMPMAGICNUMBER;

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
  double **block
}Block;

#endif
