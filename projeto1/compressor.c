#include "bmp.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

FILE *fEntrada;
FILE *fSaida;
BMPMAGICNUMBER bmpnum;
BMPFILEHEADER fHeader;
BMPINFOHEADER header;
unsigned char ***channels;

int read_io(char *a, char *b);
int error_happened(FILE *file, char *name);
void close_io();
void bmp_magic();
int header_write();
void intialize_channels(BMPINFOHEADER header, unsigned char ***channels);
void read_channels(FILE *filein, unsigned char ***channels);
/* void free_channels(unsigned char *** channels); */
unsigned char bitsToByte(int bits[]);

int main(int argc, char *argv[]) {
  /*Read the input file and output file */
  printf("intializing \n");
  printf("Read_io \n");
  read_io(argv[1], argv[2]);
  printf("bmp_magic \n");
  bmp_magic();
  printf("debug_bmp \n");
  debug_bmp(bmpnum, fHeader, header);
  printf("intialize_channels \n");
  intialize_channels(header, channels);
  printf("read_channels \n");
  /* read_channels(fEntrada, channels); */
  printf("debug_channels \n");
  /*  debug_channels(channels, header);*/
  printf("header_Write\n");
  header_write(fSaida);
  printf("free \n");
  free(channels);
  printf("close_io \n");
  close_io();
  return 0;
}

int read_io(char *a, char *b) {
  fEntrada = fopen(a, "rb");
  if (error_happened(fEntrada, a)) {
    return 0;
  }
  fSaida = fopen(b, "wb");
  if (error_happened(fSaida, b)) {
    return 0;
  }
  return 1;
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

void close_io() {
  fclose(fEntrada);
  fclose(fSaida);
}

void bmp_magic() {
  fread(&bmpnum, 2, 1, fEntrada);
  fread(&fHeader, sizeof(BMPFILEHEADER), 1, fEntrada);
  fread(&header, sizeof(BMPINFOHEADER), 1, fEntrada);
}

int header_write(FILE *fSaida) {
  fwrite(&bmpnum, 2, 1, fSaida);
  fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, fSaida);
  fwrite(&header, sizeof(BMPINFOHEADER), 1, fSaida);
  return 1;
}
void intialize_channels(BMPINFOHEADER header, unsigned char ***channels) {
  int i, j;
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
  fseek(fEntrada, 54, SEEK_SET);
  printf("Seek is fine");
  for (i = 0; i < header.biHeight; i++) {
    for (j = 0; j < header.biWidth; j++) {
      int b = fgetc(fEntrada);
      int g = fgetc(fEntrada);
      int r = fgetc(fEntrada);
      printf("%d %d %d", b, g, r);
      channels[0][i][j] = (unsigned char)b;
      channels[1][i][j] = (unsigned char)g;
      channels[2][i][j] = (unsigned char)r;
    }
  }
}

void free_channels(unsigned char ***channels) {
  printf("Start of channels!");
  for (int i = 0; i < 3; i++) {
    free(channels[i]);
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
