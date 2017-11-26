#include "../src/compressor.c"
#include <criterion/criterion.h>
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

Test(colorspacechange, are_colors_acceptable) {
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  RGBChannel *rgb_channels;
  RGBChannel *reverted_channels;
  rgb_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  reverted_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  YCbCrChannel_Double *ycbcr_channels;
  ycbcr_channels= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  read_io("teste.bmp","tested.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);

  float mistmatch_percent;
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if(abs(reverted_channels->B[i][j] - rgb_channels->B[i][j])>5)b++;
      if(abs(reverted_channels->R[i][j] - rgb_channels->R[i][j])>5)r++;
      if(abs(reverted_channels->G[i][j] - rgb_channels->G[i][j])>5)g++;
    }
    cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are wrong.",b,BLU,RESET);
    cr_assert_eq(r,0,"There are %d %sRed%s pixels that are wrong.",r,RED,RESET);
    cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are wrong.",g,GRN,RESET);
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(rgb_channels, fSaida);
    free_rgb_channels(rgb_channels);
    free_rgb_channels(reverted_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    close_io(fEntrada, fSaida);
    return;
  }}

Test(colorspacechange, are_colors_equiv) {
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  RGBChannel *rgb_channels;
  RGBChannel *reverted_channels;
  rgb_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  reverted_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  YCbCrChannel_Double *ycbcr_channels;
  ycbcr_channels= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  read_io("teste.bmp","tested.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);

  float mistmatch_percent;
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if(abs(reverted_channels->B[i][j] - rgb_channels->B[i][j])>2)b++;
      if(abs(reverted_channels->R[i][j] - rgb_channels->R[i][j])>2)r++;
      if(abs(reverted_channels->G[i][j] - rgb_channels->G[i][j])>2)g++;
    }
    cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are not equal.",b,BLU,RESET);
    cr_assert_eq(r,0,"There are %d  %sRed%s pixels that are not equal.",r,RED,RESET);
    cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are not equal.",g,GRN,RESET);;
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(rgb_channels, fSaida);
    free_rgb_channels(rgb_channels);
    free_rgb_channels(reverted_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    close_io(fEntrada, fSaida);
    return;
  }}

Test(colorspacechange, are_colors_equal) {
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  RGBChannel *rgb_channels;
  RGBChannel *reverted_channels;
  rgb_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  reverted_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  YCbCrChannel_Double *ycbcr_channels;
  ycbcr_channels= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  read_io("teste.bmp","tested.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);

  float mistmatch_percent;
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if(reverted_channels->B[i][j] != rgb_channels->B[i][j])b++;
      if(reverted_channels->R[i][j] != rgb_channels->R[i][j])r++;
      if(reverted_channels->G[i][j] != rgb_channels->G[i][j])g++;
    }
    cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are not perfectly matching.",b,BLU,RESET);
    cr_assert_eq(r,0,"There are %d %sRed%s pixels that are not perfectly matching.",r,RED,RESET);
    cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are not perfectly matching.",g,GRN,RESET);
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(rgb_channels, fSaida);
    free_rgb_channels(rgb_channels);
    free_rgb_channels(reverted_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    close_io(fEntrada, fSaida);
    return;
  }}
