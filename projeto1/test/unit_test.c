#include "../src/compressor.c"

int test_colorspacechange(unsigned char ***reference,
                          unsigned char ***converted, BMPINFOHEADER header);

int main(int argc, char *argv[]) {
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  unsigned char ***rgb_channels;
  unsigned char ***YCbCr_channels;
  unsigned char ***reverted_channels;
  read_io("../img/teste.bmp", "../img/tested.bmp", &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  rgb_channels = intialize_channels(header);
  rgb_channels = fileto_rgb(header, fEntrada, rgb_channels);
  YCbCr_channels = RGB2YCbCr(rgb_channels, header);
  reverted_channels = YCbCr2RGB(YCbCr_channels, header);
  test_colorspacechange(rgb_channels, reverted_channels, header);

  return 0;
}

int test_colorspacechange(unsigned char ***reference,
                          unsigned char ***converted, BMPINFOHEADER header) {
  int b = 0, g = 0, r = 0;
  for (int i = 0; i < header.biHeight; i++) {
    for (int j = 0; j < header.biWidth; j++) {
      if (reference[0][i][j] != converted[0][i][j])
        b++;
      if (reference[1][i][j] != converted[1][i][j])
        g++;
      if (reference[2][i][j] != converted[2][i][j])
        r++;
    }
  }
  if (b == 0 && g == 0 && r == 0) {
    return 1;
  } else {
    float mistmatch_percent= (float)(b+g+r)/(float) (3*header.biHeight*header.biWidth);
    printf("Test Unit Error [01]: Colorspace change was done unsucessfully\n");
    printf("Mistmatch numbers (total pixels %d) \n Blue %d \n Green %d \n Red %d \n",header.biHeight*header.biWidth,b,g,r);
    printf("Mismatch percentage: %f \n",mistmatch_percent);
    return 0;
  }
}
