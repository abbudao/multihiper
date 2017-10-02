#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char ***RGB2YCbCr(unsigned char ***rgb_channel, BMPINFOHEADER header);
void YCbCr2RGB(double **Y, double **Cb, double **Cr, int Height, int Width,
               double **R, double **G, double **B);
void writeR(unsigned char **R, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
            BMPINFOHEADER header);
void writeG(unsigned char **G, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
            BMPINFOHEADER header);
void writeB(unsigned char **B, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
            BMPINFOHEADER header);
void writeY(double **Y, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
            BMPINFOHEADER header);
void debug_bmp(BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
               BMPINFOHEADER header);
void debug_channels(unsigned char ***channels, BMPINFOHEADER header);
unsigned char ***intialize_channels(BMPINFOHEADER header);
void header_write(FILE *fSaida, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
                  BMPINFOHEADER header);
void bmp_magic(FILE *fEntrada, BMPMAGICNUMBER *bmpnum, BMPFILEHEADER *fHeader,
               BMPINFOHEADER *header);
void rgb_tofile(unsigned char ***channels, FILE *fSaida, BMPINFOHEADER header);
unsigned char bitsToByte(int bits[]);
int error_happened(FILE *file, char *name);
void read_channels(FILE *filein, unsigned char ***channels);
void free_channels(BMPINFOHEADER header, unsigned char ***channels);
unsigned char ***fileto_rgb(BMPINFOHEADER header, FILE *fEntrada,
                            unsigned char ***channels);
void close_io(FILE *fEntrada, FILE *fSaida);
int read_io(char *a, char *b, FILE **fEntrada, FILE **fSaida);




void debug_bmp(BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
               BMPINFOHEADER header) {
  printf("\n bmpnum: %d", bmpnum.bfType);
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

void debug_channels(unsigned char ***channels, BMPINFOHEADER header) {
  int i, j, b, g, r;
  for (i = 0; i < header.biHeight; i++) {
    for (j = 0; j < header.biWidth; j++) {
      printf("b[%d][%d] == %d", i, j, (int)channels[0][i][j]);
      printf("g[%d][%d] == %d", i, j, (int)channels[1][i][j]);
      printf("r[%d][%d] == %d", i, j, (int)channels[2][i][j]);
    }
  }
}
unsigned char ***intialize_channels(BMPINFOHEADER header) {
  int i, j;
  unsigned char ***channels;
  channels = malloc(3 * sizeof(unsigned char **));
  if (channels == NULL) {
    printf("Memory exceeded");
  }
  for (i = 0; i < 3; i++) {
    channels[i] = malloc(sizeof(unsigned char *) * header.biHeight);
    if (channels[i] == NULL) {
      printf("Memory exceeded");
    }
  }
  for (i = 0; i < 3; i++) {
    for (j = 0; j < header.biHeight; j++) {
      channels[i][j] = malloc(sizeof(unsigned char) * header.biWidth);
      if (channels[i][j] == NULL) {
        printf("Memory exceeded");
      }
    }
  }
  return channels;
}
void header_write(FILE *fSaida, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
                  BMPINFOHEADER header) {
  fwrite(&bmpnum, 2, 1, fSaida);
  fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, fSaida);
  fwrite(&header, sizeof(BMPINFOHEADER), 1, fSaida);
}

unsigned char ***RGB2YCbCr(unsigned char ***rgb_channels,
                           BMPINFOHEADER header) {
  unsigned char ***YCbCr_channels = intialize_channels(header);
  for (int i = 0; i < header.biHeight; i++) {
    for (int j = 0; j < header.biWidth; j++) {
      /* printf("%d %d %d", b, g, r); */
      YCbCr_channels[0][i][j] =
          (unsigned char)(0.114 * (float)rgb_channels[0][i][j] +
                          0.587 * (float)rgb_channels[1][i][j] +
                          0.299 * (float)rgb_channels[2][i][j]);
      YCbCr_channels[1][i][j] =
          (unsigned char)0.564 *
          ((float)rgb_channels[0][i][j] - (float)YCbCr_channels[0][i][j]);
      YCbCr_channels[1][i][j] =
          (unsigned char)0.713 *
          ((float)rgb_channels[2][i][j] - (float)YCbCr_channels[0][i][j]);
    }
  }
  return YCbCr_channels;
}

void rgb_tofile(unsigned char ***channels, FILE *fSaida, BMPINFOHEADER header) {
  for (int i = 0; i < header.biHeight; i++) {
    for (int j = 0; j < header.biWidth; j++) {
      fputc(channels[0][i][j], fSaida);
      fputc(channels[1][i][j], fSaida);
      fputc(channels[2][i][j], fSaida);
    }
  }
}
unsigned char bitsToByte(int bits[]) {
  unsigned char meuByte = 0;
  int i = 0;
  for (i = 0; i < 7; i++) {
    meuByte += bits[i];
    meuByte <<= 1;
  }
  meuByte += bits[i];

  return meuByte;
}
int error_happened(FILE *file, char *name) {
  if (file == NULL) {
    printf("File named '%s' open error: ", name);
    switch (errno) {
    case EPERM:
      printf("Operation not permitted\n");
      break;
    case ENOENT:
      printf("File not found\n");
      break;
    case EACCES:
      printf("Permission denied\n");
      break;
    case ENAMETOOLONG:
      printf("Filename is too long\n");
      break;
    default:
      printf("Unknown error\n");
    }
    return (1);
  } else {
    return (0);
  }
}
void free_channels(BMPINFOHEADER header, unsigned char ***channels) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < header.biHeight; j++) {
      free(channels[i][j]);
    }
    free(channels[i]);
  }
}
unsigned char ***fileto_rgb(BMPINFOHEADER header, FILE *fEntrada,
                            unsigned char ***channels) {
  int i, j, b, g, r;
  fseek(fEntrada, 54, SEEK_SET);
  printf("Seek is fine");
  for (i = 0; i < header.biHeight; i++) {
    for (j = 0; j < header.biWidth; j++) {
      int b = fgetc(fEntrada);
      int g = fgetc(fEntrada);
      int r = fgetc(fEntrada);
      /* printf("%d %d %d", b, g, r); */
      channels[0][i][j] = (unsigned char)b;
      channels[1][i][j] = (unsigned char)g;
      channels[2][i][j] = (unsigned char)r;
    }
  }
  return channels;
}

void close_io(FILE *fEntrada, FILE *fSaida) {
  fclose(fEntrada);
  fclose(fSaida);
}
int read_io(char *a, char *b, FILE **fEntrada, FILE **fSaida) {
  *fEntrada = fopen(a, "rb");
  if (error_happened(*fEntrada, a)) {
    return 0;
  }
  *fSaida = fopen(b, "wb");
  if (error_happened(*fSaida, b)) {
    return 0;
  }
  return 1;
}
void bmp_magic(FILE *fEntrada, BMPMAGICNUMBER *bmpnum, BMPFILEHEADER *fHeader,
               BMPINFOHEADER *header) {
  fread(bmpnum, 2, 1, fEntrada);
  fread(fHeader, sizeof(BMPFILEHEADER), 1, fEntrada);
  fread(header, sizeof(BMPINFOHEADER), 1, fEntrada);
}


