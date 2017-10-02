/*Execute por linha de comando: >> TesteBMP arquivo.bmp <enter>*/

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    BMPMAGICNUMBER bmpnum;
    BMPFILEHEADER fHeader;
    BMPINFOHEADER header;
    
    FILE *fEntrada;
    FILE *fSaida;
     int i, j;
    
    fEntrada = fopen(argv[1], "rb");    
    fSaida = fopen("saida.bmp", "wb");
    
     //leitura do header do BMP, escrita do mesmo em arquivo de saída
    printf("\n bmpnum: %d", fread(&bmpnum, 2, 1, fEntrada));
    
    fread(&fHeader, sizeof(BMPFILEHEADER), 1, fEntrada);
    printf("\n offBits: %d", fHeader.bfOffBits);
    printf("\n Reserved1: %d", fHeader.bfReserved1);
    printf("\n Reserved2: %d", fHeader.bfReserved2);
    printf("\n bfSize: %d", fHeader.bfSize);
    

    fread(&header, sizeof (BMPINFOHEADER), 1, fEntrada);
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
    printf("\n biYPelsPerMeter: %d", header.biYPelsPerMeter);

    
    char c = getch();
    
    
    //escrita dos headers em arquivo de saída

    fwrite(&bmpnum, 2, 1, fSaida);

    fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, fSaida);

    fwrite(&header, sizeof(BMPINFOHEADER), 1, fSaida);

    //posiociona o ptr do arquivo na área de dados, após o byte 54
    fseek( fEntrada, 54, SEEK_SET );

    for (i = 0; i < header.biHeight; i++) {
            for (j = 0; j < header.biWidth; j++) {
                fgetc(fEntrada);
                fputc((unsigned char) 0, fSaida); //canal B
                
                fputc((unsigned char) fgetc(fEntrada), fSaida); // canal G
                
                fgetc(fEntrada);
                fputc((unsigned char) 0, fSaida); // canal R
            }
        }

    fclose(fSaida);
    fclose(fSaida);
  
  system("PAUSE");	
  return 0;
}
