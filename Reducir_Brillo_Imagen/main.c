#include <stdio.h>
#include <stdlib.h>

// Definir un factor de oscuridad que se utilizará para reducir el brillo de la imagen
#define DARKNESS_FACTOR   120

// Definir el valor máximo y mínimo posible para un componente de color en una imagen (8 bits)
#define MAX_COLOR   255
#define MIN_COLOR   0

int main()
{
   // Abrir el archivo de entrada y salida
   FILE * fIn = fopen("Imagenes/lena512.bmp", "rb");
   FILE * fOut = fopen("Imagenes/lena_oscura.bmp", "wb");

   // Declarar arreglos para almacenar el encabezado de la imagen y la tabla de colores (si existe)
   unsigned char imgHeader[54];
   unsigned char colorTable[1024];

   // Leer los primeros 54 bytes (encabezado) del archivo BMP de entrada
   for(int i = 0; i < 54; i++)
   {
       imgHeader[i] = getc(fIn);
   }

   // Escribir el encabezado en el archivo de salida para mantener la estructura
   fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

   // Obtener información importante del encabezado, como altura, ancho y profundidad de bits
   int height = *(int *)&imgHeader[22];
   int width =  *(int *)&imgHeader[18];
   int bitDepth = *(int *)&imgHeader[28];

   int imgSize = height * width;

   // Si la profundidad de bits es menor o igual a 8, leer y escribir la tabla de colores
   if(bitDepth <= 8)
   {
       fread(colorTable, sizeof(unsigned char), 1024, fIn);
       fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
   }

   // Declarar un buffer para almacenar los valores de píxeles de la imagen
   unsigned char buffer[imgSize];

   // Leer los valores de píxeles del archivo de entrada
   fread(buffer, sizeof(unsigned char), imgSize, fIn);

   int temp;

   // Reducir la oscuridad de la imagen (aumentar el brillo)
   for(int i = 0; i < imgSize; i++)
   {
       temp = buffer[i] - DARKNESS_FACTOR; // Reducir el valor de píxel (aumentar el brillo)
       buffer[i] = (temp < MIN_COLOR) ? MIN_COLOR : temp; // Asegurarse de que el valor no sea menor que el mínimo
   }

   // Escribir la imagen con la oscuridad reducida en el archivo de salida
   fwrite(buffer, sizeof(unsigned char), imgSize, fOut);

   // Cerrar ambos archivos
   fclose(fIn);
   fclose(fOut);

   return 0;
}
