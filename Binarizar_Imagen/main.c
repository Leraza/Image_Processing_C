#include <stdio.h>
#include <stdlib.h>

#define WHITE   255
#define BLACK   0
#define THRESHOLD   50

int main()
{
   // Abrir el archivo de entrada y salida
   FILE *fIn = fopen("imagenes/lighthouse.bmp", "rb");
   FILE *fOut = fopen("imagenes/lighthouse_bw2.bmp", "wb");

   // Declarar arreglos para el encabezado de la imagen y la tabla de colores
   unsigned char imgHeader[54];
   unsigned char colorTable[1024];

   // Verificar si el archivo de entrada se abrió correctamente
   if (fIn == NULL)
   {
       printf("Error al abrir el archivod\n");
   }

   // Leer los primeros 54 bytes (encabezado) del archivo BMP de entrada
   for (int i = 0; i < 54; i++)
   {
       imgHeader[i] = getc(fIn);
   }

   // Escribir el encabezado en el archivo de salida
   fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

   // Obtener información importante del encabezado, como alto, ancho y profundidad de bits
   int height = *(int *)&imgHeader[22];
   int width = *(int *)&imgHeader[18];
   int bitDepth = *(int *)&imgHeader[28];

   int imgSize = height * width;

   // Si la profundidad de bits es menor o igual a 8, leer y escribir la tabla de colores
   if (bitDepth <= 8)
   {
       fread(colorTable, sizeof(unsigned char), 1024, fIn);
       fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
   }

   // Declarar un bufer para almacenar los valores de píxeles
   unsigned char buffer[imgSize];

   // Leer los valores de píxeles del archivo de entrada
   fread(buffer, sizeof(unsigned char), imgSize, fIn);

   // Convertir la imagen a blanco y negro
   for (int i = 0; i < imgSize; i++)
   {
       // Si el valor del picxel es mayor que el umbral, se establece como blanco de lo contrario negro
       buffer[i] = (buffer[i] > THRESHOLD) ? WHITE : BLACK;
   }

   // Escribir la imagen en blanco y negro en el archivo de salida
   fwrite(buffer, sizeof(unsigned char), imgSize, fOut);

   // Cerrar ambos archivos
   fclose(fIn);
   fclose(fOut);

   return 0;
}
