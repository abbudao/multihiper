#include "src/compressor.c"
int main(int argc, char *argv[]) {
  /*Read the input file and output file */
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  unsigned char ***rgb_channels;
  double ***YCbCr_channels;
  unsigned char ***reconverted_channels;
  printf("intializing \n");
  printf("Read_io \n");
  read_io(argv[1], argv[2],&fEntrada,&fSaida);
  printf("bmp_magic \n");
  bmp_magic(fEntrada, &bmpnum,&fHeader,&header);
  printf("debug_bmp \n");
  /* debug_bmp(bmpnum, fHeader, header); */
  printf("intialize_channels \n");
  rgb_channels=  intialize_channels(header);
  rgb_channels= fileto_rgb(header,fEntrada, rgb_channels);
  YCbCr_channels=RGB2YCbCr(rgb_channels, header);
 reconverted_channels=YCbCr2RGB(YCbCr_channels,header) ;
  printf("read_channels \n");
  /* read_channels(fEntrada, rgb_channels); */
  printf("debug_channels \n");
  /*  debug_channels(rgb_channels, header);*/
  printf("header_Write\n");
  header_write(fSaida,bmpnum,fHeader,header);
  printf("rgb_tofile\n");
  rgb_tofile(reconverted_channels,fSaida,header);
  printf("%d",reconverted_channels[0][1][2]);
  printf("free \n");
  free_channels(header,rgb_channels);
  free_channels_double(header,YCbCr_channels);
  free_channels(header,reconverted_channels);
  printf("close_io \n");
  close_io(fEntrada,fSaida);
  return 0;
}
