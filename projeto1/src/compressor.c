#include "bmp.h"
#include "color_channel.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

int RGB2YCbCr(RGBChannel *rgb_channels,YCbCrChannel_Double *ycbcr_channels);
int YCbCr2RGB(YCbCrChannel_Double *ycbcr_channels, RGBChannel *rgb_channels);

double ****create_double_blocks(double **channels,int height, int width);
void dct_transform(double **channel,double **dct_vector,int width,int height);
void dct_untransform(double **coeficients,double **channel,int width,int height);

double **intialize_dct_coefficients(int height, int width);

void header_write(FILE *fSaida, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
    BMPINFOHEADER header);
void bmp_magic(FILE *fEntrada, BMPMAGICNUMBER *bmpnum, BMPFILEHEADER *fHeader,
    BMPINFOHEADER *header);

int rgb_tofile(RGBChannel *channels, FILE *fSaida);
unsigned char bitsToByte(int bits[]);
void read_channels(FILE *filein, unsigned char ***channels);
int read_io(char *a, char *b, FILE **fEntrada, FILE **fSaida);
void fileto_rgb(FILE *fEntrada,RGBChannel *rgb_channels);

void close_io(FILE *fEntrada, FILE *fSaida);

void debug_bmp(BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
    BMPINFOHEADER header);
void debug_channels(unsigned char ***channels, BMPINFOHEADER header);
int error_happened(FILE *file, char *name);

void debug_bmp(BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
    BMPINFOHEADER header) {
  printf("\n bmpnum: %d", bmpnum.bfType);
  printf("\n offBits: %d", fHeader.bfOffBits);
  printf("\n Reserved1: %d", fHeader.bfReserved1);
  printf("\n Reserved2: %d", fHeader.bfReserved2);
  printf("\n bfSize: %d", fHeader.bfSize);
  printf("\n bibitCount: %d", header.biBitCount);
  printf("\n biClrImportant: %d", header.biClrImportant);
  printf("\n biClrUsed: %d", header.biClrUsed);
  printf("\n biCompression: %d", header.biCompression);
  printf("\n biHeight: %d", header.biHeight);
  printf("\n biPlanes: %d", header.biPlanes);
  printf("\n biSize: %d", header.biSize);
  printf("\n biSizeImage: %d", header.biSizeImage);
  printf("\n biWidth: %d", header.biWidth);
  printf("\n biXPelsPerMeter: %d", header.biXPelsPerMeter);
  printf("\n biYPelsPerMeter: %d\n", header.biYPelsPerMeter);
}

void debug_channels(unsigned char ***channels, BMPINFOHEADER header) {
  int i, j; 
  for (i = 0; i < header.biHeight; i++) {
    for (j = 0; j < header.biWidth; j++) {
      printf("b[%d][%d] == %d \t", i, j, (int)channels[0][i][j]);
      printf("g[%d][%d] == %d \t", i, j, (int)channels[1][i][j]);
      printf("r[%d][%d] == %d \n", i, j, (int)channels[2][i][j]);
    }
  }
}
int intialize_rgb_channels (RGBChannel *rgb_channels,int height, int width){
  int i, j;
  /* rgb_channels= (RGBChannel *) malloc(sizeof(RGBChannel)); */
  rgb_channels->R=(unsigned char**) malloc (sizeof(unsigned char *) * height);
  rgb_channels->B= (unsigned char**)malloc(sizeof(unsigned char *) * height);
  rgb_channels->G=(unsigned char**) malloc(sizeof(unsigned char *) * height);
  if (rgb_channels->R== NULL || rgb_channels->B==NULL || rgb_channels->G==NULL)
  {
    printf("Memory exceeded");
    return 0;
  }
  for (j = 0; j < height; j++) {
    rgb_channels->R[j]=(unsigned char*) malloc(sizeof(unsigned char ) * width);
    rgb_channels->B[j]= (unsigned char*)malloc(sizeof(unsigned char ) * width);
    rgb_channels->G[j]=(unsigned char*) malloc(sizeof(unsigned char ) * width);
    if (rgb_channels->R[j]== NULL || rgb_channels->B[j]==NULL || rgb_channels->G[j]==NULL){
      printf("Memory exceeded");
      return 0;
    }}
  rgb_channels->width= width;
  rgb_channels->height= height;
  return 1;
}
int intialize_ycbcr_uc_channels(YCbCrChannel_UC *ycbcr_channels,int height, int width){
  int i, j;
  ycbcr_channels->Y=(unsigned char**) malloc(sizeof(unsigned char *) * height);
  ycbcr_channels->Cb= (unsigned char**)malloc(sizeof(unsigned char *) * height);
  ycbcr_channels->Cr=(unsigned char**)malloc (sizeof(unsigned char *) * height);
  if (ycbcr_channels->Y== NULL || ycbcr_channels->Cb==NULL || ycbcr_channels->Cr==NULL)
  {
    printf("Memory exceeded");
    return 0;
  }
  for (j = 0; j < height; j++) {
    ycbcr_channels->Y[j]=(unsigned char*) malloc(sizeof(unsigned char ) * width);
    ycbcr_channels->Cb[j]= (unsigned char*)malloc(sizeof(unsigned char ) * width);
    ycbcr_channels->Cr[j]=(unsigned char*) malloc(sizeof(unsigned char ) * width);
    if (ycbcr_channels->Y[j]== NULL || ycbcr_channels->Cb[j]==NULL || ycbcr_channels->Cr[j]==NULL){
      printf("Memory exceeded");
      return 0;
    }
  }
  ycbcr_channels->width= width;
  ycbcr_channels->height= height;
  return 1;
}

int intialize_ycbcr_double_channels(YCbCrChannel_Double *ycbcr_channels,int height, int width){
  int i, j;
  ycbcr_channels->Y=(double**) malloc(sizeof(double *) * height);
  ycbcr_channels->Cb= (double**)malloc(sizeof(double *) * height);
  ycbcr_channels->Cr=(double**) malloc(sizeof(double *) * height);
  if (ycbcr_channels->Y== NULL|| ycbcr_channels->Cb==NULL|| ycbcr_channels->Cr==NULL)
  {
    printf("memory exceeded");
    return 0;
  }
  for (j = 0; j < height; j++) {
    ycbcr_channels->Y[j]=(double*) malloc(sizeof(double ) * width);
    ycbcr_channels->Cb[j]= (double*)malloc(sizeof(double ) * width);
    ycbcr_channels->Cr[j]=(double*) malloc(sizeof(double ) * width);
    if (ycbcr_channels->Y[j]== NULL || ycbcr_channels->Cb[j]==NULL|| ycbcr_channels->Cr[j]==NULL ){
      printf("memory exceeded");
      return 0;
    }}
  ycbcr_channels->width= width;
  ycbcr_channels->height= height;
  return 1;
}
void header_write(FILE *fSaida, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
    BMPINFOHEADER header) {
  fwrite(&bmpnum, 2, 1, fSaida);
  fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, fSaida);
  fwrite(&header, sizeof(BMPINFOHEADER), 1, fSaida);
}

int RGB2YCbCr(RGBChannel *rgb_channels, YCbCrChannel_Double *ycbcr_channels){
  int  height = rgb_channels->height;
  int  width= rgb_channels->width;
  double Y, Cb, Cr, R, G, B;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      R = (double)rgb_channels->R[i][j];
      G = (double)rgb_channels->G[i][j];
      B = (double)rgb_channels->B[i][j];
      Y = 0.114 * B + 0.587 * G + 0.299 * R ;
      Cb = 0.564 * (B - Y);
      Cr = 0.713 * (R - Y);
      ycbcr_channels->Y[i][j] = Y;
      ycbcr_channels->Cb[i][j] = Cb;
      ycbcr_channels->Cr[i][j] = Cr;
    }
  }
  return 1;
}

int YCbCr2RGB(YCbCrChannel_Double *ycbcr_channels,  RGBChannel *rgb_channels) {
  int height = ycbcr_channels->height;
  int width = ycbcr_channels->width;
  double Y, Cb, Cr, R, G, B;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Y = ycbcr_channels->Y[i][j];
      Cb = ycbcr_channels->Cb[i][j];
      Cr = ycbcr_channels->Cr[i][j];
      R = (int) round(Y + 1.402 * Cr);
      G = (int)round(Y - 0.344 * Cb - 0.714 * Cr);
      B = (int) round(Y + 1.772 * Cb);
      rgb_channels->B[i][j] = B;
      rgb_channels->G[i][j] = G;
      rgb_channels->R[i][j] = R;
    }
  }
  return 1;
}

int rgb_tofile(RGBChannel *channels, FILE *fSaida) {
  int height= channels->height;
  int width= channels->width;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      fputc(channels->B[i][j], fSaida);
      fputc(channels->G[i][j], fSaida);
      fputc(channels->R[i][j], fSaida);
    }
  }
  return 1;
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

int free_rgb_channels(RGBChannel *channels) {
  for (int j = 0; j < channels->height; j++) {
    free(channels->R[j]);
    free(channels->G[j]);
    free(channels->B[j]);
  }
  free(channels->R);
  free(channels->G);
  free(channels->B);
  free(channels);
  return 1;
}

int free_ycbcr_double_channels(YCbCrChannel_Double *channels) {
  for (int j = 0; j < channels->height; j++) {
    free(channels->Y[j]);
    free(channels->Cb[j]);
    free(channels->Cr[j]);
  }
  free(channels->Y);
  free(channels->Cb);
  free(channels->Cr);
  free(channels);
  return 1;
}

int free_ycbcr_uc(YCbCrChannel_UC *channels) {
  for (int j = 0; j < channels->height; j++) {
    free(channels->Y[j]);
    free(channels->Cb[j]);
    free(channels->Cr[j]);
  }
  free(channels->Y);
  free(channels->Cb);
  free(channels->Cr);
  free(channels);
  return 1;
}
void free_dct_coefficients(int height, int width, double **channels) {
  for (int j = 0; j < height; j++) {
    free(channels[j]);
  }
  free(channels);
}
void fileto_rgb(FILE *fEntrada,  RGBChannel *rgb_channels) {
  int i, j, b, g, r;
  fseek(fEntrada, 54, SEEK_SET);
  for (i = 0; i < rgb_channels->height; i++) {
    for (j = 0; j < rgb_channels->width; j++) {
      b = fgetc(fEntrada);
      g = fgetc(fEntrada);
      r = fgetc(fEntrada);
      rgb_channels->B[i][j] = (unsigned char)b;
      rgb_channels->G[i][j] = (unsigned char)g;
      rgb_channels->R[i][j] = (unsigned char)r;
    }
  }
}

void close_io(FILE *fEntrada, FILE *fSaida) {
  fclose(fEntrada);
  fclose(fSaida);
}
int read_io(char *a, char *b, FILE **fEntrada, FILE **fSaida) {
  *fEntrada = fopen(a, "rb");
  if (error_happened(*fEntrada, a)) {
    return 0;
  }
  *fSaida = fopen(b, "wb");
  if (error_happened(*fSaida, b)) {
    return 0;
  }
  return 1;
}
void bmp_magic(FILE *fEntrada, BMPMAGICNUMBER *bmpnum, BMPFILEHEADER *fHeader,
    BMPINFOHEADER *header) {
  fread(bmpnum, 2, 1, fEntrada);
  fread(fHeader, sizeof(BMPFILEHEADER), 1, fEntrada);
  fread(header, sizeof(BMPINFOHEADER), 1, fEntrada);
}
double **intialize_dct_coefficients(int height, int width) {
  int i;
  double **dct_vector;
  dct_vector = malloc(sizeof(double *) * height);
  if (dct_vector == NULL) {
    printf("Memory exceeded");
  }
  for (i = 0; i < height; i++) {
    dct_vector[i] = malloc(sizeof(double)* width);
    if (dct_vector[i] == NULL) {
      printf("Memory exceeded");
    }
  }
  return dct_vector;
}
void dct_transform(double **channel,double **dct_vector,int width,int height)                           
{
  double **channel_aux =  intialize_dct_coefficients(height,width);
  double ck1,ck2;

  /* for(int i=0;i<height;i++) */
  /* { */
  /*   for(int j=0;j<width;j++) */
  /*   { */
  /*     channel_aux[i][j]= channel[i][j]; */
  /*   } */
  /* } */
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      channel_aux[i][j]= dct_table[i][j];
    }
  }
  for(int k1=0;k1<height;k1++){
    for(int k2=0;k2<width;k2++){
      ck1=  (k1==0)?1/sqrt(2.0):1 ;
      ck2=  (k2==0)?1/sqrt(2.0):1 ;
      for(int n1=0;n1<height;n1++){
        for(int n2=0;n2<width;n2++){
          dct_vector[k1][k2] =+ck1*ck2* channel_aux[n1][n2]*
            cos((double)(2*k2+1) * (double)n2 * M_PI/16.0) *
            cos((double)(2*k1+1) * (double)n1 * M_PI/16.0);
        }
      }
      dct_vector[k1][k2]*=0.25;
    }
  }
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      printf("dct_vector[%d][%d]=%f\n",i,j,dct_vector[i][j]);
    }
  }
  free_dct_coefficients(height,width,channel_aux);
}
void dct_untransform(double **coeficients,double **channel,int width,int height)
{
  double **coefficients_aux=  intialize_dct_coefficients(height,width);
  double ck1,ck2; 
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      coefficients_aux[i][j]= coeficients[i][j];
    }
  }
  for(int k1=0;k1<height;k1++){
    for(int k2=0;k2<width;k2++){
      ck1=  (k1==0)?1/sqrt(2.0):1 ;
      ck2=  (k2==0)?1/sqrt(2.0):1 ;
      for(int n1=0;n1<height;n1++){
        for(int n2=0;n2<width;n2++){
          channel[k1][k2] =+ck1*ck2* coefficients_aux[n1][n2]*
            cos((double)(2*n2+1) * (double)k2 * M_PI/16.0) *
            cos((double)(2*n1+1) * (double)k1 * M_PI/16.0);
        }
      }
    }
  }
  free_dct_coefficients(height,width,coefficients_aux);
}


double ****create_double_blocks(double **channel, int height, int width){
  double ****block_matrix;
  int block_height= height/8;
  int block_width= width/8;
  if(height %8 >0)block_height++;
  if(width %8 >0)block_width++;

  block_matrix= malloc(sizeof(double***)*block_height);
  for(int i=0;i<block_height;i++){
    block_matrix[i]= malloc(sizeof(double**)*block_width);
  }
  for(int i=0;i<block_height;i++){
    for(int j=0;j<block_width;j++){
      block_matrix[i][j]= malloc(sizeof(double*)*8);
    }
  }
  for(int i=0;i<block_height;i++){
    for(int j=0;j<block_width;j++){
      for(int k=0; k<8;k++){
        block_matrix[i][j][k]= malloc(sizeof(double)*8);
      }
    }
  }
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      block_matrix[i/8][j/8][i%8][j%8]=channel[i][j];
    }
  }
  for(int i=height; i<block_height*8;i++){
    for(int j=width; j<block_width*8; j++){
      block_matrix[i/8][j/8][i%8][j%8]=0;
    }
  }

  return(block_matrix) ;
}
void block_to_channel(double****blocks, double **channel, int height, int width){
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      channel[i][j]=blocks[i/8][j/8][i%8][j%8];
    }
  }
}

void dct_on_blocks(double ****blocks, int channel_height, int channel_width){
  int block_height=channel_height/8 ;
  int block_width=channel_width/8;
  if(channel_height%8>0){
    block_height++;
  }
  if(channel_width%8>0){
    block_width++;
  }
  for(int i=0;i<block_height;i++){
    for (int j = 0; j < block_width; j++) {
      dct_transform(blocks[i][j],blocks[i][j],8,8);
    }
  }
}
void inverse_dct_on_blocks(double ****blocks, int channel_height, int channel_width){
  int block_height=channel_height/8 ;
  int block_width=channel_width/8;
  if(channel_height%8>0){
    block_height++;
  }
  if(channel_width%8>0){
    block_width++;
  }
  for(int i=0;i<block_height;i++){
    for (int j = 0; j < block_width; j++) {
      dct_untransform(blocks[i][j],blocks[i][j],8,8);
    }
  }
}
void quantize_block(double ****blocks, int channel_height, int channel_width){
  int block_height=channel_height/8 ;
  int block_width=channel_width/8;
  if(channel_height%8>0){
    block_height++;
  }
  if(channel_width%8>0){
    block_width++;
  }

  for(int i=0;i<block_height-1;i++){
    for (int j = 0; j < block_width-1; j++) {
      for (int k = 0; k < 8; k++) {
        for (int l= 0; l < 8; l++) {
          blocks[i][j][k][l]= blocks[i][j][k][l]/quantization_Y[k][l];
        }  
      }
    }
  }
}
void dequantize_block(double ****blocks, int channel_height, int channel_width){
  int block_height=channel_height/8 ;
  int block_width=channel_width/8;
  if(channel_height%8>0){
    block_height++;
  }
  if(channel_width%8>0){
    block_width++;
  }

  for(int i=0;i<block_height-1;i++){
    for (int j = 0; j < block_width-1; j++) {
      for (int k = 0; k < 8; k++) {
        for (int l= 0; l < 8; l++) {
          blocks[i][j][k][l]= blocks[i][j][k][l]*quantization_Y[k][l];
        }  
      }
    }
  }
}
