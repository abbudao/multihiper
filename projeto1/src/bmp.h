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

typedef struct {
  unsigned int bfSize;        /* Size of file */
  unsigned short bfReserved1; /* Reserved */
  unsigned short bfReserved2; /* ... */
  unsigned int bfOffBits;     /* Offset to bitmap data */
} BMPFILEHEADER;

/**** BMP file info structure ****/
typedef struct {
  unsigned int biSize;         /* Size of info header */
  int biWidth;                 /* Width of image */
  int biHeight;                /* Height of image */
  unsigned short biPlanes;     /* Number of color planes */
  unsigned short biBitCount;   /* Number of bits per pixel */
  unsigned int biCompression;  /* Type of compression to use */
  unsigned int biSizeImage;    /* Size of image data */
  int biXPelsPerMeter;         /* X pixels per meter */
  int biYPelsPerMeter;         /* Y pixels per meter */
  unsigned int biClrUsed;      /* Number of colors used */
  unsigned int biClrImportant; /* Number of important colors */
} BMPINFOHEADER;
#endif
