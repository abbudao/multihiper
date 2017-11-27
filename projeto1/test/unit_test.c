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
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0,i,j;
  read_io("img/teste.bmp","img/colorspacechange_are_colors_acceptable.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(abs(reverted_channels->B[i][j] - rgb_channels->B[i][j])>5)b++;
      if(abs(reverted_channels->R[i][j] - rgb_channels->R[i][j])>5)r++;
      if(abs(reverted_channels->G[i][j] - rgb_channels->G[i][j])>5)g++;
    }}
    cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are wrong.",b,BLU,RESET);
    cr_assert_eq(r,0,"There are %d %sRed%s pixels that are wrong.",r,RED,RESET);
    cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are wrong.",g,GRN,RESET);
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(reverted_channels, fSaida);
    free_rgb_channels(rgb_channels);
    free_rgb_channels(reverted_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    close_io(fEntrada, fSaida);
    return;
  }

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
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0,i,j;
  read_io("img/teste.bmp","img/colorpsacechange_are_colors_equiv.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(abs(reverted_channels->B[i][j] - rgb_channels->B[i][j])>2)b++;
      if(abs(reverted_channels->R[i][j] - rgb_channels->R[i][j])>2)r++;
      if(abs(reverted_channels->G[i][j] - rgb_channels->G[i][j])>2)g++;
    }}
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
  }

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
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0,i,j;
  read_io("img/teste.bmp","img/colorspacechange_are_colors_equal.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(reverted_channels->B[i][j] != rgb_channels->B[i][j])b++;
      if(reverted_channels->R[i][j] != rgb_channels->R[i][j])r++;
      if(reverted_channels->G[i][j] != rgb_channels->G[i][j])g++;
    }}
    cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are not perfectly matching.",b,BLU,RESET);
    cr_assert_eq(r,0,"There are %d %sRed%s pixels that are not perfectly matching.",r,RED,RESET);
    cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are not perfectly matching.",g,GRN,RESET);
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(reverted_channels, fSaida);
    free_rgb_channels(rgb_channels);
    free_rgb_channels(reverted_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    close_io(fEntrada, fSaida);
    return;
  }

Test(blocks, creation_is_fine) {
  FILE *fEntrada;
  FILE *fSaida;
  BMPMAGICNUMBER bmpnum;
  BMPFILEHEADER fHeader;
  BMPINFOHEADER header;
  RGBChannel *rgb_channels;
  RGBChannel *reverted_channels;
  rgb_channels= (RGBChannel *) malloc(sizeof(RGBChannel));
  YCbCrChannel_Double *ycbcr_channels;
  ycbcr_channels= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  BlockMatrix *block_matrix;
  block_matrix= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  int height, width,i,j,k,l,m,n,y=0,cb=0,cr=0;
  read_io("img/teste.bmp","img/block_creation_is_fine.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, rgb_channels->height, rgb_channels->width);
  intialize_block_matrix(block_matrix, ycbcr_channels->height, ycbcr_channels->width);
  height=  ycbcr_channels->height;
  width= ycbcr_channels->width;
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2Blocks(ycbcr_channels, block_matrix);
  Blocks2YCbCr(block_matrix,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,rgb_channels);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      k=i/8;
      l=j/8;
      m=i%8;
      n=j%8;
      if(ycbcr_channels->Y[i][j] != block_matrix->Y_block[k][l].values[m][n])y++;
      if(ycbcr_channels->Cb[i][j] != block_matrix->Cb_block[k][l].values[m][n])cb++;
      if(ycbcr_channels->Cr[i][j] != block_matrix->Cr_block[k][l].values[m][n])cr++;
    }}
    cr_assert_eq(y,0,"The Y channel presented %d problematic pixels on block conversion.",y);
    cr_assert_eq(cr,0,"The Cr channel presented %d problematic pixels on block conversion.",cr);
    cr_assert_eq(cb,0,"The Cb channel presented %d problematic pixels on block conversion.",cb);
    header_write(fSaida, bmpnum, fHeader, header);
    rgb_tofile(rgb_channels, fSaida);
    free_rgb_channels(rgb_channels);
    free_ycbcr_double_channels(ycbcr_channels);
    close_io(fEntrada, fSaida);
    return;
  }
Test(dct, is_reversible) {
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
  block_matrix= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_aux= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  int height, width,i,j,b=0,r=0,g=0;
  read_io("img/teste.bmp","img/dct_is_reversible.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, rgb_channels->height, rgb_channels->width);
  intialize_block_matrix(block_matrix, ycbcr_channels->height, ycbcr_channels->width);
  intialize_block_matrix(dct_block_matrix_aux, ycbcr_channels->height, ycbcr_channels->width);
  height=ycbcr_channels->height;
  width=ycbcr_channels->width;
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2Blocks(ycbcr_channels, block_matrix);
  DctBlockMatrix(block_matrix, dct_block_matrix_aux);
  IdctBlockMatrix(dct_block_matrix_aux,block_matrix);
  Blocks2YCbCr(block_matrix,ycbcr_channels);
  YCbCr2RGB(ycbcr_channels,reverted_channels);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(abs(reverted_channels->B[i][j] - rgb_channels->B[i][j])>2)b++;
      if(abs(reverted_channels->R[i][j] - rgb_channels->R[i][j])>2)r++;
      if(abs(reverted_channels->G[i][j] - rgb_channels->G[i][j])>2)g++;
    }}
  cr_assert_eq(b,0,"There are %d %sBlue%s pixels that are not equal.",b,BLU,RESET);
  cr_assert_eq(r,0,"There are %d %sRed%s pixels that are not equal.",r,RED,RESET);
  cr_assert_eq(g,0,"There are %d %sGreen%s pixels that are not equal.",g,GRN,RESET);;
  header_write(fSaida, bmpnum, fHeader, header);
  rgb_tofile(reverted_channels, fSaida);
  free_rgb_channels(rgb_channels);
  free_ycbcr_double_channels(ycbcr_channels);
  close_io(fEntrada, fSaida);
  return;
}
Test(dct, works_on_sample_table) {
  int i,j;
  Block before_dct;
  Block aux;
  Block after_dct;
  double epsilon=0.1;
  int error_count=0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      before_dct.values[i][j]= dct_sample_table_before[i][j];
    }
  }
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      after_dct.values[i][j]= dct_sample_table_after[i][j];
    }
  }
  Block2DCT(&before_dct,&aux);
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if(fabs(aux.values[i][j]-after_dct.values[i][j])>epsilon)error_count++;
    }
  }
  cr_assert_eq(error_count,0,"%d values were different than expected on DCT transform",error_count);
  return;
}
Test(idct, works_on_sample_table) {
  int i,j;
  Block before_dct;
  Block aux;
  Block back_normal;
  double epsilon=0.1;
  int error_count=0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      before_dct.values[i][j]= dct_sample_table_before[i][j];
    }
  }
  Block2DCT(&before_dct,&aux);
  DCT2Block(&aux,&back_normal);
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if(fabs(back_normal.values[i][j]-before_dct.values[i][j])>epsilon)error_count++;
    }
  }
  cr_assert_eq(error_count,0,"%d values were different than expected on IDCT transform",error_count);
  return;
}
Test(dct, on_image){
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
  YCbCrChannel_Double *ycbcr_channels_aux;
  ycbcr_channels_aux= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  BlockMatrix *block_matrix;
  BlockMatrix *dct_block_matrix_aux;
  BlockMatrix *dct_block_matrix_untransformed;
  block_matrix= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_aux= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_untransformed= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0,i,j;
  read_io("img/teste.bmp","img/dct_on_image.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels_aux, header.biHeight, header.biWidth);
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
  free_rgb_channels(rgb_channels);
  free_rgb_channels(reverted_channels);
  free_ycbcr_double_channels(ycbcr_channels);
  close_io(fEntrada, fSaida);
  return;
}

Test(quantization, on_Image)
{
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
  YCbCrChannel_Double *ycbcr_channels_aux;
  ycbcr_channels_aux= (YCbCrChannel_Double *) malloc(sizeof(YCbCrChannel_Double));
  BlockMatrix *block_matrix;
  BlockMatrix *dct_block_matrix_aux;
  BlockMatrix *dct_block_matrix_untransformed;
  block_matrix= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_aux= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  dct_block_matrix_untransformed= (BlockMatrix *) malloc(sizeof(BlockMatrix));
  int height=  reverted_channels->height;
  int width= reverted_channels->width;
  int b = 0, g = 0, r = 0,i,j;
  read_io("img/teste.bmp","img/quantization_on_image.bmp" , &fEntrada, &fSaida);
  bmp_magic(fEntrada, &bmpnum, &fHeader, &header);
  intialize_rgb_channels(rgb_channels, header.biHeight, header.biWidth);
  intialize_rgb_channels(reverted_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels, header.biHeight, header.biWidth);
  intialize_ycbcr_double_channels(ycbcr_channels_aux, header.biHeight, header.biWidth);
  intialize_block_matrix(block_matrix, ycbcr_channels->height, ycbcr_channels->width);
  intialize_block_matrix(dct_block_matrix_aux, ycbcr_channels->height, ycbcr_channels->width);
  intialize_block_matrix(dct_block_matrix_untransformed, ycbcr_channels->height, ycbcr_channels->width);
  fileto_rgb(fEntrada, rgb_channels);
  RGB2YCbCr(rgb_channels,ycbcr_channels);
  YCbCr2Blocks(ycbcr_channels, block_matrix);
  DctBlockMatrix(block_matrix, dct_block_matrix_aux);
  QuantizeBlockMatrix(dct_block_matrix_aux,block_matrix);
  DequantizeBlockMatrix(block_matrix,dct_block_matrix_aux);
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
  free_rgb_channels(rgb_channels);
  free_rgb_channels(reverted_channels);
  free_ycbcr_double_channels(ycbcr_channels);
  close_io(fEntrada, fSaida);
  return;
}

Test(zigzagscan, append_block){
int i=0,j=0,value=0;
Block *block;
block= (Block*) malloc(sizeof(Block));
ZigZagVector *zigzag;
zigzag= (ZigZagVector *) malloc(sizeof(ZigZagVector));
intialize_zigzag_vector(zigzag,1,1);
for (i = 0; i < 8; i++) {
  for (j = 0;  j< 8; j++) {
   block->values[i][j]=i+j;
  }
}
append_block(zigzag,block);
cr_assert_eq(zigzag->vector[63],14,"valor esperado 14 obtido %d",zigzag->vector[63]);
cr_assert_eq(zigzag->vector[0],0,"valor esperado 0 obtido %d",zigzag->vector[0]);
free(zigzag);
free(block);
}
