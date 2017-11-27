#include "bmp.h"
#include "reference_tables.h"
#include "color_channel.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
int RGB2YCbCr(RGBChannel *rgb_channels,YCbCrChannel_Double *ycbcr_channels);
int YCbCr2RGB(YCbCrChannel_Double *ycbcr_channels, RGBChannel *rgb_channels);

int intialize_block_matrix(BlockMatrix *block_matrix,int source_height, int source_width);
int Block2DCT(Block *block,Block *dct_block);
int Block2Vector(Block *block,ZigZagVector *zig_zag);
int DCT2Block(Block *block,Block *dct_block);
void dct_untransform(double **coeficients,double **channel,int width,int height);

int YCbCr2Blocks(YCbCrChannel_Double *ycbcr_channels, BlockMatrix *block_matrix);
int Blocks2YCbCr(BlockMatrix *block_matrix, YCbCrChannel_Double *ycbcr_channels);

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
void print_block(Block block){
  int i, j;
  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      if(j==0){
        printf("|");
      }
      printf(" %-6.1f ",block.values[i][j]);
      if(j==7){
        printf("|");
      }
    }
    printf("\n");
  }
  printf("\n");
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
int intialize_zigzag_vector(ZigZagVector *zig_zag,int block_height, int block_width){
  zig_zag->block_height=block_height;
  zig_zag->block_width=block_width;
  zig_zag->size= 64*block_height*block_width;
  zig_zag->vector=(unsigned char*)sizeof((unsigned char)*size);
  zig_zag->next_index=0;
  return 1;
}
int append_block(ZigZagVector *zig_zag,Block *block){
  int i,x=0,y=0;
  int index=zig_zag->next_index;
  int right=1,diag_left_down=0,down=0,diag_right_up=0;
  for(i=0;i<64;i++){
    if(right){
      y++;
      right=0
        if(x==0){
          diag_left_down=1;
        }
        else{
          diag_right_up=1;
        }
    }
    else if(diag_left_down){
      y--;
      x++;
      if(y==0){
        diag_left_down=0;
        down=1;
      }
      else if(x==7){
        diag_left_down=0;
        right=1;
      }
      }
    else if(down){
        down=0;
        x++;
      if(y==0){
        diag_right_up=1
      }
      else{
        diag_left_down=1;
      }
    }
    else if(diag_right_up){
      x--;
      y++;
      if(x==0){
        right=1;
      }
      else if(y==7){
        down=1;
      }
    }
    zig_zag->vector[index+i]=block->values[x][y];
  }
  zig_zag->next_index=next_index+64;
  return 1;
}
int intialize_rgb_channels (RGBChannel *rgb_channels,int height, int width){
  int i, j;
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

int intialize_block_matrix(BlockMatrix *block_matrix,int source_height, int source_width){
  int i,j;
  int height= source_height/8;
  int width= source_width/8;
  if(source_height%8 >0)height++;
  if(source_width%8 >0)width++;
  block_matrix->Y_block= (Block **) malloc(sizeof(Block*) *height);
  block_matrix->Cb_block= (Block **) malloc(sizeof(Block*) *height);
  block_matrix->Cr_block= (Block **) malloc(sizeof(Block*) *height);
  for (i = 0; i < height; ++i) {
    block_matrix->Y_block[i]=(Block *) malloc(sizeof(Block)*width); 
    block_matrix->Cb_block[i]=(Block *) malloc(sizeof(Block)*width); 
    block_matrix->Cr_block[i]=(Block *) malloc(sizeof(Block)*width); 
  }
  block_matrix->height= height;
  block_matrix->width= width;
  block_matrix->source_height=source_height;
  block_matrix->source_width= source_width;
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
      R = (double)rgb_channels->R[i][j]-128;
      G = (double)rgb_channels->G[i][j]-128;
      B = (double)rgb_channels->B[i][j]-128;
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
      rgb_channels->B[i][j] = B+128;
      rgb_channels->G[i][j] = G+128;
      rgb_channels->R[i][j] = R+128;
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
int Block2DCT(Block *block,Block *dct_block)
{
  double c_i, c_j;
  int i,j,x,y;
  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      dct_block->values[i][j]=0;
      for(x=0;x<8;x++){
        for(y=0;y<8;y++){
          dct_block->values[i][j]+=block->values[x][y]*
            cos(((double)(2*x+1) * (double)i * (double)M_PI)/(double)16.0) *
            cos(((double)(2*y+1) * (double)j* M_PI)/(double)16.0);
        }
      }
      c_i=  (i==0)?1/sqrt(2.0):1 ;
      c_j=  (j==0)?1/sqrt(2.0):1 ;
      dct_block->values[i][j]=dct_block->values[i][j]*(c_i*c_j)/4;
    }
  }
  return 1;
}
int DCT2Block(Block *dct_block,Block *block)
{
  double c_i, c_j;
  int i,j,x,y;
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      block->values[x][y]=0;
      for(i=0;i<8;i++){
        for(j=0;j<8;j++){
          c_i=  (i==0)?(double)(1/sqrt(2.0)):1 ;
          c_j=  (j==0)?(double)(1/sqrt(2.0)):1 ;
          block->values[x][y]+=c_i*c_j*dct_block->values[i][j]*
            cos(((double)(2*x+1) * (double)i *(double)M_PI)/(double)16.0) *
            cos(((double)(2*y+1) * (double)j*(double)M_PI)/(double)16.0);
        }
      }
      block->values[x][y]=block->values[x][y]*0.25;
    }
  }
  return 1;
}
int YCbCr2Blocks(YCbCrChannel_Double *ycbcr_channels, BlockMatrix *block_matrix){
  int i,j;
  int source_height= block_matrix->source_height;
  int source_width = block_matrix -> source_width;
  for (i = 0; i < source_height; i++) {
    for (j = 0;  j< source_width; j++){
      block_matrix->Y_block[i/8][j/8].values[i%8][j%8]= ycbcr_channels->Y[i][j];
      block_matrix->Cb_block[i/8][j/8].values[i%8][j%8]= ycbcr_channels->Cb[i][j];
      block_matrix->Cr_block[i/8][j/8].values[i%8][j%8]= ycbcr_channels->Cr[i][j];
    } 
  }
  /*Create padding */
  for(int i=source_height; i<block_matrix->height*8;i++){
    for(int j=source_width; j<block_matrix->width*8; j++){
      block_matrix->Y_block[i/8][j/8].values[i%8][j%8]= 0;
      block_matrix->Cb_block[i/8][j/8].values[i%8][j%8]=0;
      block_matrix->Cr_block[i/8][j/8].values[i%8][j%8]=0;
    }
  }
  return 1;
}
int Blocks2YCbCr(BlockMatrix *block_matrix, YCbCrChannel_Double *ycbcr_channels){
  int i,j;
  int height = block_matrix->source_height;
  int width = block_matrix->source_width;
  for(i=0;i<height;i++){
    for (j = 0; j < width; j++) {
      ycbcr_channels->Y[i][j]=  block_matrix->Y_block[i/8][j/8].values[i%8][j%8];
      ycbcr_channels->Cb[i][j]=  block_matrix->Cb_block[i/8][j/8].values[i%8][j%8];
      ycbcr_channels->Cr[i][j]=  block_matrix->Cr_block[i/8][j/8].values[i%8][j%8];
    }}
  return 1;
}

int DctBlockMatrix(BlockMatrix *block_matrix_input,BlockMatrix *block_matrix_output){
  int i,j;
  for(i=0;i<block_matrix_input->height;i++){
    for (j = 0; j <block_matrix_output->width ; j++) {
      Block2DCT(&(block_matrix_input->Y_block[i][j]),&(block_matrix_output->Y_block[i][j]));
      Block2DCT(&(block_matrix_input->Cb_block[i][j]),&(block_matrix_output->Cb_block[i][j]));
      Block2DCT(&(block_matrix_input->Cr_block[i][j]),&(block_matrix_output->Cr_block[i][j]));
    }
  }
  return 1;
}
int IdctBlockMatrix(BlockMatrix *idct_block_matrix_input, BlockMatrix *idct_block_matrix_output){
  int i,j;
  for(i=0;i<idct_block_matrix_input->height;i++){
    for (j = 0; j <idct_block_matrix_input->width; j++) {
      DCT2Block(&(idct_block_matrix_input->Y_block[i][j]),&(idct_block_matrix_output->Y_block[i][j]));
      DCT2Block(&(idct_block_matrix_input->Cb_block[i][j]),&(idct_block_matrix_output->Cb_block[i][j]));
      DCT2Block(&(idct_block_matrix_input->Cr_block[i][j]),&(idct_block_matrix_output->Cr_block[i][j]));
    }
  }
  return 1;
}

int QuantizeBlock(Block *block,Block *output)
{
  int i,j;
  for (i = 0; i < 8; i++) {
    for (j= 0; j < 8; j++) {
      output->values[i][j]= block->values[i][j]/quantization_Y[i][j];
    }  
  }
  return 1;
}
int DequantizeBlock(Block *block,Block *output){
  int i,j;
  for (i = 0; i < 8; i++) {
    for (j = 0;  j< 8; j++) {
      output->values[i][j]= block->values[i][j]*quantization_Y[i][j];
    }
  }
  return 1;
}
int QuantizeBlockMatrix(BlockMatrix *block_matrix_input,BlockMatrix *block_matrix_output){
  int i,j;
  for(i=0;i<block_matrix_input->height;i++){
    for (j=0; j <block_matrix_output->width ; j++) {
      QuantizeBlock(&(block_matrix_input->Y_block[i][j]),&(block_matrix_output->Y_block[i][j]));
      QuantizeBlock(&(block_matrix_input->Cb_block[i][j]),&(block_matrix_output->Cb_block[i][j]));
      QuantizeBlock(&(block_matrix_input->Cr_block[i][j]),&(block_matrix_output->Cr_block[i][j]));
    }
  }
  return 1;
}
int DequantizeBlockMatrix(BlockMatrix *block_matrix_input,BlockMatrix *block_matrix_output){
  int i,j;
  for(i=0;i<block_matrix_input->height;i++){
    for (j = 0; j <block_matrix_output->width ; j++) {
      DequantizeBlock(&(block_matrix_input->Y_block[i][j]),&(block_matrix_output->Y_block[i][j]));
      DequantizeBlock(&(block_matrix_input->Cb_block[i][j]),&(block_matrix_output->Cb_block[i][j]));
      DequantizeBlock(&(block_matrix_input->Cr_block[i][j]),&(block_matrix_output->Cr_block[i][j]));
    }
  }
  return 1;
}
