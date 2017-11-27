#include "src/compressor.c"
int main(int argc, char *argv[]) {
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
  BlockMatrix *block_matrix;
  BlockMatrix *dct_block_matrix_aux;
  BlockMatrix *dct_block_matrix_untransformed;
  block_matrix= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_aux= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_untransformed= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0,i,j;
  read_io("img/teste.bmp","img/test10.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  intialize_block_matrix(block_matrix, ycbcr_channels->height, ycbcr_channels->width);
  intialize_block_matrix(dct_block_matrix_aux, ycbcr_channels->height, ycbcr_channels->width);
  intialize_block_matrix(dct_block_matrix_untransformed, ycbcr_channels->height, ycbcr_channels->width);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2Blocks(ycbcr_channels, block_matrix);
  DctBlockMatrix(block_matrix, dct_block_matrix_aux);
  IdctBlockMatrix(dct_block_matrix_aux, dct_block_matrix_untransformed);
  Blocks2YCbCr(dct_block_matrix_untransformed, ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(abs(reverted_channels->B[i][j] - rgb_channels->B[i][j])>0)b++;
      if(abs(reverted_channels->R[i][j] - rgb_channels->R[i][j])>0)r++;
      if(abs(reverted_channels->G[i][j] - rgb_channels->G[i][j])>0)g++;
    }}
    cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are wrong.",b,BLU,RESET);
    cr_assert_eq(r,0,"There are %d %sRed%s pixels that are wrong.",r,RED,RESET);
    cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are wrong.",g,GRN,RESET);
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(reverted_channels, fSaida);
    close_io(fEntrada, fSaida);
    free_rgb_channels(rgb_channels);
    free_rgb_channels(reverted_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    return;
  }
