#include "../src/compressor.c"

int test_colorspacechange(unsigned char ***reference,
    unsigned char ***converted, int height, int width);


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
  double ****double_blocks;
 matrix_dct=intialize_dct_coefficients(8,8) ;
 YCbCr_channels= intialize_channels_double(8,8);
  dct_transform(NULL,matrix_dct,8,8 );
  dct_untransform(matrix_dct,YCbCr_channels[0],8,8 );
  printf("Referencia: %f \n",matrix_dct[0][0]);
  printf("Obtido: %f \n",YCbCr_channels[0][0][0]);
  /* read_io("teste.bmp", "tested.bmp", &fEntrada, &fSaida); */
  /* bmp_magic(fEntrada, &bmpnum, &fHeader, &header); */
  /* rgb_channels = intialize_channels(header.biHeight,header.biWidth); */
  /* fileto_rgb(header, fEntrada, rgb_channels); */
  /* YCbCr_channels = RGB2YCbCr(rgb_channels, header.biHeight,header.biWidth); */
  /* printf("Raw YCbCr[0][0[0]]:%f\n",YCbCr_channels[0][0][0]); */
  /* reverted_channels = YCbCr2RGB(YCbCr_channels, header.biHeight,header.biWidth); */
  /* test_colorspacechange(rgb_channels, reverted_channels, header.biHeight,header.biWidth); */
  /* header_write(fSaida,bmpnum,fHeader,header); */
  /* rgb_tofile(reverted_channels,fSaida,header); */
  /* double_blocks=create_double_blocks(YCbCr_channels[0], header.biHeight, header.biWidth); */
/* printf("After block creation YCbCr[0][0[0]]:%f\n",  double_blocks[0][0][0][0]); */
  /* dct_on_blocks(double_blocks,header.biHeight, header.biWidth); */
  /* printf("After DCT on blocks:%f \n",double_blocks[0][0][0][0]); */
  /* inverse_dct_on_blocks(double_blocks,header.biHeight, header.biWidth); */
  /* printf("After inverse DCT:%f \n",double_blocks[0][0][0][0]); */
  /* quantize_block(double_blocks, header.biHeight, header.biWidth); */
  /* printf("After quantization:%f \n",double_blocks[0][0][0][0]); */
  /* dequantize_block(double_blocks, header.biHeight, header.biWidth); */
  /* printf("After dequantization:%f \n",double_blocks[0][0][0][0]); */
  /* inverse_dct_on_blocks(double_blocks,header.biHeight, header.biWidth); */
  /* printf("After inverse DCT:%f \n",double_blocks[0][0][0][0]); */
  /* block_to_channel(double_blocks,YCbCr_channels[0],header.biHeight,header.biWidth); */
  /* printf("After Block to channel YCbCr[0][0[0]]:%f\n",YCbCr_channels[0][0][0]); */
  /* matrix_dct = intialize_dct_coefficients(header.biHeight, header.biWidth); */
  /* dct_transform(YCbCr_channels[0],matrix_dct,header.biWidth, header.biHeight); */
  /* free_channels(header.biHeight, header.biWidth,rgb_channels); */
  /* free_channels_double(header.biHeight, header.biWidth,YCbCr_channels); */
  /* free_channels(header.biHeight, header.biWidth,reverted_channels); */
  /* close_io(fEntrada,fSaida); */
  return 0;
}

int test_colorspacechange(unsigned char ***reference,
    unsigned char ***converted, int height, int width) {
  int b = 0, g = 0, r = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
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
    float mistmatch_percent= (float)(b+g+r)/(float) (3*height*width);
    printf("Error! Test Unit [01]: Colorspace change was done unsucessfully\n");
    printf("Mistmatch numbers (total pixels %d) \n Blue %d \n Green %d \n Red %d \n",height*width,b,g,r);
    printf("Mismatch percentage: %f \n",mistmatch_percent);
    return 0;
  }

}

int test_dct(double **channel,double **dct_vector, int width, int height){
  dct_transform(channel, dct_vector,height,width);
  /* dct_untransform(dct_vector, channel,height, width); */
  return 1;
}
