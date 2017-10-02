#include <stdio.h>
#include <stdlib.h>


unsigned char bitsToByte(int bits[]) {
    unsigned char meuByte = 0;
    int i =0;
    
    for( i=0; i<7; i++) {
        meuByte += bits[i];
        meuByte <<= 1;
    }
    meuByte += bits[i];
    
    return meuByte;
}

int main(int argc, char *argv[])
{
  FILE *fp;
  unsigned char byte;
  int bits[8] = {1,1,0,0,1,1,1,0};

  if(!(fp = fopen("bits.bin", "wb"))){
            printf("Error: could not open input file." );
            exit(1);
    }
  
  byte = bitsToByte(bits);
    
  fputc(byte, fp);
  fclose(fp);
  
  if(!(fp = fopen("bits.bin", "rb"))){
            printf("Error: could not open input file." );
            exit(1);
  }
    
  unsigned char res = fgetc(fp);
  fclose(fp);
  
  printf("%d", res);
 
  system("PAUSE");	
  return 0;
}
