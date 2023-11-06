#include <stdio.h>
#include <stdlib.h>
#include "mask.h"

#define BMP_HEADER_SIZE         54
#define BMP_COLOR_TABLE_SIZE    1024
#define CUSTOM_IMG_SIZE         512*512

// Prototipos de funciones
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth);
void imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf);
void line_detector(unsigned char *_inputImgData, unsigned char *_outputImgData, int imgCols, int imgRows, int MASK[][3]);

int main()
{
    // Declaración de variables
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgBuffer[CUSTOM_IMG_SIZE];
    unsigned char imgBuffer2[CUSTOM_IMG_SIZE];

    // Nombres de archivos
    const char imgName[] = "lena512.bmp";
    const char newImgName[] = "lena_rdia.bmp";

    // Máscara para el detector de línea
    int RDIA[3][3] = { {-1,-1,2}, {-1,2,-1}, {2,-1,-1} };

    // Leer la imagen original
    imageReader(imgName, &imgHeight, &imgWidth, &imgBitDepth, imgHeader, imgColorTable, imgBuffer);

    // Aplicar el detector de línea utilizando la máscara RDIA
    line_detector(&imgBuffer[0], &imgBuffer2[0], imgWidth, imgHeight, RDIA);

    // Escribir la imagen procesada
    imageWriter(newImgName, imgHeader, imgColorTable, imgBuffer2, imgBitDepth);

    printf("funciona :D\n");

    return 0;
}

// Función para escribir una imagen en un archivo
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth)
{
    FILE *fo = fopen(imgName, "wb");
    fwrite(header, sizeof(unsigned char), 54, fo);
    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(unsigned char), 1024, fo);
    }
    fwrite(buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, fo);
    fclose(fo);
}

// Función para leer una imagen desde un archivo
void  imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf)
{
    int i;
    FILE *streamIn;
    streamIn = fopen(imgName, "rb");

    if (streamIn ==(FILE *)0)
    {
        printf("Error\n");
    }

    for (i = 0; i < 54; i++)
    {
        _header[i] = getc(streamIn);
    }

    *_width = *(int *)&_header[18];
    *_height = *(int *)&_header[22];
    *_bitDepth = *(int *)&_header[28];

    if (*_bitDepth <= 8)
    {
        fread(_colorTable, sizeof(unsigned char), 1024, streamIn);
    }

    fread(_buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, streamIn);

    fclose(streamIn);
}

// Función para aplicar el detector de línea a una imagen
void line_detector(unsigned char *_inputImgData, unsigned char *_outputImgData, int imgCols, int imgRows, int MASK[][3])
{
    int x, y, i, j, sum;

    for (y = 1; y <= imgRows - 1; y++)
    {
        for (x = 1; x <= imgCols - 1; x++)
        {
            sum = 0;
            for (i = -1; i <= 1; i++)
            {
                for (j = -1; j <= 1; j++)
                {
                    sum = sum + *(_inputImgData + x + i + (long)(y + j) * imgCols) * MASK[i + 1][j + 1];
                }
            }
            if (sum > 255)
                sum = 255;
            if (sum < 0)
                sum = 0;
            *(_outputImgData + x + (long)y * imgCols) = sum;
        }
    }
}
