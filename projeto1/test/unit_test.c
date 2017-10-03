#include "../src/compressor.c"

int test_colorspacechange(unsigned char *** reference, unsigned char *** converted,BMPINFOHEADER header);
int test_colorspacechange(unsigned char *** reference, unsigned char *** converted,BMPINFOHEADER header){
  for (int i = 0; i < header.biHeight; i++) {
    for (int j = 0; j < header.biWidth; j++) {
      if( reference[0][i][j] != converted[0][i][j]) printf("Blue : ref %d conv %d",reference[0][i][j],converted[0][i][j]);
      if( reference[1][i][j] != converted[1][i][j]) printf("Green : ref %d conv %d",reference[1][i][j],converted[1][i][j]);
      if( reference[2][i][j] != converted[2][i][j]) printf("Green : ref %d conv %d",reference[1][i][j],converted[1][i][j]);
    }
  }
  return 1;
}
int main(int argc, char *argv[])
{
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  unsigned char ***rgb_channels;
  unsigned char ***YCbCr_channels;
  unsigned char ***reverted_channels;
  read_io("../img/teste.bmp", "../img/tested.bmp",&fEntrada,&fSaida);
  bmp_magic(fEntrada, &bmpnum,&fHeader,&header);
  rgb_channels=  intialize_channels(header);
  rgb_channels= fileto_rgb(header,fEntrada, rgb_channels);
  YCbCr_channels=RGB2YCbCr(rgb_channels, header);
  reverted_channels= YCbCr2RGB(YCbCr_channels,header);
  if(!test_colorspacechange(rgb_channels,reverted_channels,header)){
    printf("Test Unit Error [01]: Colorspace change was done unsucessfully");
  }


  return 0;
}
