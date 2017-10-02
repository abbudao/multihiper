#ifndef _BMP_H_
#define _BMP_H
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**** BMP file header structure ****/
typedef struct {
    unsigned short bfType;       /* Magic number for file */
} BMPMAGICNUMBER;

typedef struct {
    unsigned int   bfSize;       /* Size of file */
    unsigned short bfReserved1;  /* Reserved */
    unsigned short bfReserved2;  /* ... */
    unsigned int   bfOffBits;    /* Offset to bitmap data */
} BMPFILEHEADER;

/**** BMP file info structure ****/
typedef struct {
    unsigned int   biSize;          /* Size of info header */
    int            biWidth;         /* Width of image */
    int            biHeight;        /* Height of image */
    unsigned short biPlanes;        /* Number of color planes */
    unsigned short biBitCount;      /* Number of bits per pixel */
    unsigned int   biCompression;   /* Type of compression to use */
    unsigned int   biSizeImage;     /* Size of image data */
    int            biXPelsPerMeter; /* X pixels per meter */
    int            biYPelsPerMeter; /* Y pixels per meter */
    unsigned int   biClrUsed;       /* Number of colors used */
    unsigned int   biClrImportant;  /* Number of important colors */
} BMPINFOHEADER;

void RGB2YCbCr(unsigned char **R, unsigned char **G, unsigned char **B, int Heigth, int Width, double **Y, double **Cb, double **Cr, FILE *entrada);
void YCbCr2RGB (double **Y, double **Cb, double **Cr, int Height, int Width, double **R, double **G, double **B);
void writeR(unsigned char **R, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader, BMPINFOHEADER header);
void writeG(unsigned char **G, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader, BMPINFOHEADER header);
void writeB(unsigned char **B, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader, BMPINFOHEADER header);
void writeY(double **Y, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader, BMPINFOHEADER header);
void debug_bmp(BMPMAGICNUMBER bmpnum,BMPFILEHEADER fHeader,BMPINFOHEADER header);
void debug_channels(unsigned char *** channels, BMPINFOHEADER header);

void debug_bmp(BMPMAGICNUMBER bmpnum,BMPFILEHEADER fHeader,BMPINFOHEADER header)
{
    printf("\n bmpnum: %d", bmpnum);
    printf("\n offBits: %d", fHeader.bfOffBits);
    printf("\n Reserved1: %d", fHeader.bfReserved1);
    printf("\n Reserved2: %d", fHeader.bfReserved2);
    printf("\n bfSize: %d", fHeader.bfSize);
    printf("\n bibitCount: %d", header.biBitCount);
    printf("\n biClrImportant: %d", header.biClrImportant);
    printf("\n biClrUsed: %d", header.biClrUsed);
    printf("\n biCompression: %d", header.biCompression);
    printf("\n biHeight: %d", header.biHeight);
    printf("\n biPlanes: %d", header.biPlanes);
    printf("\n biSize: %d", header.biSize);
    printf("\n biSizeImage: %d", header.biSizeImage);
    printf("\n biWidth: %d", header.biWidth);    
    printf("\n biXPelsPerMeter: %d", header.biXPelsPerMeter);
    printf("\n biYPelsPerMeter: %d\n", header.biYPelsPerMeter);
}

void debug_channels(unsigned char *** channels, BMPINFOHEADER header){
  int i, j, b, g, r;
  for (i = 0; i < header.biHeight; i++) {
    for (j = 0; j < header.biWidth; j++) {
      printf("b[%d][%d] == %d",i,j,(int)channels[0][i][j] );
      printf("g[%d][%d] == %d",i,j,(int)channels[1][i][j] );
      printf("r[%d][%d] == %d",i,j,(int)channels[2][i][j] );
    }
  }
}
#endif
