#include "compressor.c"

int test_colorspacechange(unsigned char *** reference, unsigned char *** converted,int height, int width){
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if( reference[0][i][j] != converted[0][i][j]) return 0;
      if( reference[1][i][j] != converted[1][i][j]) return 0;
      if( reference[2][i][j] != converted[2][i][j]) return 0;
    }
  }
  return 1;
}

