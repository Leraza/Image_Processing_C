#include <stdio.h>
#include <stdlib.h>

// Definiciones de tamaños de encabezado y tabla de colores BMP
#define BMP_HEADER_SIZE         54
#define BMP_COLOR_TABLE_SIZE    1024
#define CUSTOM_IMG_SIZE         1024*1024

void imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf);
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth);

int main()
{
   // Declaración de variables para almacenar información de la imagen
   int imgWidth, imgHeight, imgBitDepth;
   unsigned char imgHeader[BMP_HEADER_SIZE];
   unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
   unsigned char imgBuffer[CUSTOM_IMG_SIZE];

   // Nombres de los archivos de imagen de entrada y salida
   const char imgName[] = "imagenes/man.bmp";
   const char newImgName[] = "imagenes/man_copy.bmp";

   // Llama a la función para leer la imagen de entrada
   imageReader(imgName, &imgWidth, &imgHeight, &imgBitDepth, &imgHeader[0], &imgColorTable[0], &imgBuffer[0]);

   // Llama a la función para escribir la imagen de salida
   imageWriter(newImgName, imgHeader, imgColorTable, imgBuffer, imgBitDepth);

   printf("se ha copiado la imagen :) \n");

   return 0;
}

// Función para leer una imagen BMP
void imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf)
{
    int i;
    FILE *streamIn;
    streamIn = fopen(imgName, "rb");

    if (streamIn == (FILE *)0)
    {
        printf("No se pudo leer la imagen\n");
    }

    // Leer los primeros 54 bytes (encabezado) del archivo BMP de entrada
    for (i = 0; i < 54; i++)
    {
        _header[i] = getc(streamIn);
    }

    // Obtener información importante del encabezado, como ancho, alto y profundidad de bits
    *_width = *(int *)&_header[18];
    *_height = *(int *)&_header[22];
    *_bitDepth = *(int *)&_header[28];

    // Si la profundidad de bits es menor o igual a 8, leer la tabla de colores
    if (*_bitDepth <= 8)
    {
        fread(_colorTable, sizeof(unsigned char), 1024, streamIn);
    }

    // Leer los datos de la imagen
    fread(_buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, streamIn);

    fclose(streamIn);
}

// Funcion para escribir una imagen BMP
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth)
{
    FILE *fo = fopen(imgName, "wb");
    fwrite(header, sizeof(unsigned char), 54, fo);

    // Si la profundidad de bits es menor o igual a 8, escribir la tabla de colores
    if (bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(unsigned char), 1024, fo);
    }

    // Escribir los datos de la imagen
    fwrite(buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, fo);
    fclose(fo);
}
