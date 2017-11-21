#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
static const double quantization_table[8][8]={
  {16,11 ,10,16,24,40,51,61},
  {12,12,14,19,26,58,60,55},
  {1413,16,24,40,57,69,56},
  {14,17,22,29,51,87,80,62},
  {18,22,37,56,68,109,103,77},
  {24,35,55,64,81,104,113,92}, 
  {49,64,78,87,103,121,120,101},
  {72,92,95,98,112,100,103,99}};
static const double dct_table[8][8]={
  {    1,     19,    37,    55,    73,    91,    109,   127},
  {    19,    37,    55,    73,    91,    109,   127,   145},
  {    37,    55,    73,    91,    109,   127,   145,   163},
  {    55,    73,    91,    109,   127,   145,   163,   181},
  {    73,    91,    109,   127,   145,   163,   181,   199},
  {    91,    109,   127,   145,   163,   181,   199,   217},
  {    109,   127,   145,   163,   181,   199,   217,   235},
  {    127,   145,   163,   181,   199,   217,   235,   253}};

  double ***RGB2YCbCr(unsigned char ***rgb_channel, int height ,int width);
  unsigned char ***YCbCr2RGB(double ***YCbCr_channels,int height, int width);

  double ****create_double_blocks(double **channels,int height, int width);
  void dct_transform(double **channel,double **dct_vector,int width,int height);
  void dct_untransform(double **coeficients,double **channel,int width,int height);

  double **intialize_dct_coefficients(int height, int width);
  double ***intialize_channels_double(int height, int width);
  unsigned char ***intialize_channels(int height, int width);

  void free_channels_double(int height, int width, double ***channels);
  void free_channels(int height,int width ,unsigned char ***channels);


  void header_write(FILE *fSaida, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
      BMPINFOHEADER header);
void bmp_magic(FILE *fEntrada, BMPMAGICNUMBER *bmpnum, BMPFILEHEADER *fHeader,
    BMPINFOHEADER *header);

void rgb_tofile(unsigned char ***channels, FILE *fSaida, BMPINFOHEADER header);
unsigned char bitsToByte(int bits[]);
void read_channels(FILE *filein, unsigned char ***channels);
int read_io(char *a, char *b, FILE **fEntrada, FILE **fSaida);
void fileto_rgb(BMPINFOHEADER header, FILE *fEntrada,unsigned char ***channels);

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
unsigned char ***intialize_channels (int height, int width){
  int i, j;
  unsigned char ***channels;
  channels = malloc(3 * sizeof(unsigned char **));
  if (channels == NULL) {
    printf("Memory exceeded");
  }
  for (i = 0; i < 3; i++) {
    channels[i] = malloc(sizeof(unsigned char *) * height);
    if (channels[i] == NULL) {
      printf("Memory exceeded");
    }
  }
  for (i = 0; i < 3; i++) {
    for (j = 0; j < height; j++) {
      channels[i][j] = malloc(sizeof(unsigned char) * width);
      if (channels[i][j] == NULL) {
        printf("Memory exceeded");
      }
    }
  }
  return channels;
}


double ***intialize_channels_double(int height, int width) {
  int i, j;
  double ***channels;
  printf("Check1");
  channels =(double ***) malloc(3 * sizeof(double **));
  if (channels == NULL) {
    printf("Memory exceeded");
  }
  printf("Check2");
  for (i = 0; i < 3; i++) {
    channels[i] = (double **)malloc(sizeof(double *) * height);
    if (channels[i] == NULL) {
      printf("Memory exceeded");
    }
  }
  printf("Check3");
  for (i = 0; i < 3; i++) {
    for (j = 0; j < height; j++) {
      channels[i][j] = (double *) malloc(sizeof(double) * width);
      if (channels[i][j] == NULL) {
        printf("Memory exceeded");
      }
    }
  }
  return channels;
}

void header_write(FILE *fSaida, BMPMAGICNUMBER bmpnum, BMPFILEHEADER fHeader,
    BMPINFOHEADER header) {
  fwrite(&bmpnum, 2, 1, fSaida);
  fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, fSaida);
  fwrite(&header, sizeof(BMPINFOHEADER), 1, fSaida);
}

double ***RGB2YCbCr(unsigned char ***rgb_channels,int height, int width){
  double ***YCbCr_channels = intialize_channels_double(height,width);
  double Y, Cb, Cr, R, G, B;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      R = (double)rgb_channels[2][i][j];
      G = (double)rgb_channels[1][i][j];
      B = (double)rgb_channels[0][i][j];
      Y = 0.114 * B + 0.587 * G + 0.299 * R ;
      /* Y= Y>255? 255 : Y; */
      Cb = 0.564 * (B - Y);
      /* Cb= Cb>255? 255 : Cb; */
      Cr = 0.713 * (R - Y);
      /* Cr= Cr>255? 255 : Cr; */
      YCbCr_channels[0][i][j] = Y;
      YCbCr_channels[1][i][j] = Cb;
      YCbCr_channels[2][i][j] = Cr;
    }
  }
  return YCbCr_channels;
}

unsigned char ***YCbCr2RGB(double ***YCbCr_channels, int height, int width   ) {
  unsigned char ***rgb_channels = intialize_channels(height, width);
  double Y, Cb, Cr, R, G, B;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Y = YCbCr_channels[0][i][j];
      Cb = YCbCr_channels[1][i][j];
      Cr = YCbCr_channels[2][i][j];
      /* Cb =(double) YCbCr_channels[1][i][j]-128; */
      /* Cr =(double) YCbCr_channels[2][i][j]-128; */
      R = Y + 1.402 * Cr;
      G = Y - 0.344 * Cb - 0.714 * Cr;
      B = Y + 1.772 * Cb;
      /*Blue mask */
      rgb_channels[0][i][j] = (unsigned char)round(B);
      /*Green mask */
      rgb_channels[1][i][j] = (unsigned char)round(G);
      /*Red mask */
      rgb_channels[2][i][j] = (unsigned char)round(R);
    }
  }
  return rgb_channels;
}

void rgb_tofile(unsigned char ***channels, FILE *fSaida, BMPINFOHEADER header) {
  for (int i = 0; i < header.biHeight; i++) {
    for (int j = 0; j < header.biWidth; j++) {
      fputc(channels[0][i][j], fSaida);
      fputc(channels[1][i][j], fSaida);
      fputc(channels[2][i][j], fSaida);
    }
  }
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
void free_channels(int height, int width , unsigned char ***channels) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      free(channels[i][j]);
    }
    free(channels[i]);
  }
  free(channels);
}
void free_channels_double(int height, int width, double ***channels) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      free(channels[i][j]);
    }
    free(channels[i]);
  }
  free(channels);
}
void free_dct_coefficients(int height, int width, double **channels) {
  for (int j = 0; j < height; j++) {
    free(channels[j]);
  }
  free(channels);
}
void fileto_rgb(BMPINFOHEADER header, FILE *fEntrada,
    unsigned char ***channels) {
  int i, j, b, g, r;
  fseek(fEntrada, 54, SEEK_SET);
  for (i = 0; i < header.biHeight; i++) {
    for (j = 0; j < header.biWidth; j++) {
      b = fgetc(fEntrada);
      g = fgetc(fEntrada);
      r = fgetc(fEntrada);
      /* printf("%d %d %d", b, g, r); */
      channels[0][i][j] = (unsigned char)b;
      channels[1][i][j] = (unsigned char)g;
      channels[2][i][j] = (unsigned char)r;
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
  float c;

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

  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      c =  (i!=0 && j!=0)?1:0.5 ;
      for(int x=0;x<height;x++){
        for(int y=0;y<width;y++){
          dct_vector[i][j] =+ c/4 * channel_aux[x][y]*cos(((2*x+1)*i*M_PI)/16) *cos(((2*y+1)*j*M_PI)/16);
        }
      }
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
  float c;
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      coefficients_aux[i][j]= coeficients[i][j];
    }
  }
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      c =  (i!=0 && j!=0)?1:0.5 ;
      for(int x=0;x<height;x++){
        for(int y=0;y<width;y++){
          channel[i][j] =+ c/4 * coefficients_aux[x][y]*cos(((2*x+1)*i*M_PI)/16) *cos(((2*y+1)*j*M_PI)/16)  ;
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
          blocks[i][j][k][l]= blocks[i][j][k][l]/quantization_table[k][l];
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
          blocks[i][j][k][l]= blocks[i][j][k][l]*quantization_table[k][l];
        }  
      }
    }
  }
}
