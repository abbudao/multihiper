#include "src/compressor.c"
int main(int argc, char *argv[]) {
  /*Read the input file and output file */
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  RGBChannel *rgb_channels;
  rgb_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  YCbCrChannel_Double *ycbcr_channels;
  ycbcr_channels= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  printf("intializing \n");
  printf("Read_io \n");
  read_io(argv[1], argv[2], &fEntrada, &fSaida);
  printf("bmp_magic \n");
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  printf("debug_bmp \n");
  debug_bmp(bmpnum, fHeader, header);
  printf("intialize_channels \n");
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  printf("fileto_rgb \n");
  fileto_rgb(fEntrada, rgb_channels);
  printf("rgb2ycbcr \n");
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  printf("YCbCr2RGB\n");
  YCbCr2RGB(ycbcr_channels,rgb_channels);
  printf("read_channels \n");
  printf("debug_channels \n");
  printf("header_Write\n");
  header_write(fSaida, bmpnum, fHeader, header);
  printf("rgb_tofile\n");
  rgb_tofile(rgb_channels, fSaida);
  printf("free \n");
  free_rgb_channels(rgb_channels);
  free_ycbcr_double_channels(ycbcr_channels);
  printf("close_io \n");
  close_io(fEntrada, fSaida);
  return 0;
}
