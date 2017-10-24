#include "../src/compressor.c"

int test_colorspacechange(unsigned char ***reference,
                          unsigned char ***converted, BMPINFOHEADER header);

int test_dct();

int main(int argc, char *argv[]) {
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  unsigned char ***rgb_channels;
  double ***YCbCr_channels;
  unsigned char ***reverted_channels;
  double **matrix_dct;
  read_io("teste.bmp", "tested.bmp", &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  rgb_channels = intialize_channels(header);
  rgb_channels = fileto_rgb(header, fEntrada, rgb_channels);
  YCbCr_channels = RGB2YCbCr(rgb_channels, header);
  reverted_channels = YCbCr2RGB(YCbCr_channels, header);
  test_colorspacechange(rgb_channels, reverted_channels, header);
  header_write(fSaida,bmpnum,fHeader,header);
  rgb_tofile(reverted_channels,fSaida,header);
  matrix_dct = dct_transform(YCbCr_channels[0],header.biWidth, header.biHeight);
  free_channels(header,rgb_channels);
  free_channels_double(header,YCbCr_channels);
  free_channels(header,reverted_channels);
  close_io(fEntrada,fSaida);
  return 0;
}

int test_colorspacechange(unsigned char ***reference,
                          unsigned char ***converted, BMPINFOHEADER header) {
  int b = 0, g = 0, r = 0;
  for (int i = 0; i < header.biHeight; i++) {
    for (int j = 0; j < header.biWidth; j++) {
      if (reference[0][i][j] != converted[0][i][j]){
        printf("Blue: Reference %d Converted %d\n ",reference[0][i][j],converted[0][i][j]);
        b++;}
      if (reference[1][i][j] != converted[1][i][j]){
        printf("Green: Reference %d Converted %d \n ",reference[1][i][j],converted[1][i][j]);
        g++;}
      if (reference[2][i][j] != converted[2][i][j]){
        printf("Red: Reference %d Converted %d \n ",reference[2][i][j],converted[2][i][j]);
        r++;}
    }
  }
  if (b == 0 && g == 0 && r == 0) {
    printf("Success! Test Unit [01]: Colorspace change was done sucessfully!\n");
    return 1;
  } else {
    float mistmatch_percent= (float)(b+g+r)/(float) (3*header.biHeight*header.biWidth);
    printf("Error! Test Unit [01]: Colorspace change was done unsucessfully\n");
    printf("Mistmatch numbers (total pixels %d) \n Blue %d \n Green %d \n Red %d \n",header.biHeight*header.biWidth,b,g,r);
    printf("Mismatch percentage: %f \n",mistmatch_percent);
    return 0;
  }

}

  /* int test_dct(){ */

/* return  1; */
  /* } */
