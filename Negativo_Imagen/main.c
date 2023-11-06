#include <stdio.h>
#include <stdlib.h>

int main()
{
   // Abrir el archivo de entrada en modo lectura binaria ("rb")
   FILE * fp = fopen("girlface.bmp", "rb");

   // Declarar arreglos y punteros para almacenar los datos de la imagen
   unsigned char  *imageData;
   unsigned char  *newImageData;
   unsigned char imageHeader[54];
   unsigned char colorTable[1024];

   // Leer los primeros 54 bytes (encabezado) del archivo BMP
   fread(imageHeader, sizeof(unsigned char), 54, fp);

   // Obtener informacin importante del encabezado, como ancho, alto y profundidad de bits
   int width = *(int*)&imageHeader[18];
   int height = *(int*)&imageHeader[22];
   int bitDepth = *(int*)&imageHeader[28];

   int imgDataSize = width * height;

   // Asignar memoria dinamica para almacenar los datos de la imagen original y la imagen negativa
   imageData = (unsigned char *)malloc(imgDataSize * sizeof(unsigned char));
   newImageData = (unsigned char *)malloc(imgDataSize * sizeof(unsigned char));

   // Si la profundidad de bits es menor o igual a 8, leer y almacenar la tabla de colores
   if (bitDepth <= 8)
   {
      fread(colorTable, sizeof(unsigned char), 1024, fp);
   }

   // Leer los datos de la imagen original y almacenarlos en imageData
   fread(imageData, sizeof(unsigned char), imgDataSize, fp);

   // Calcular la imagen negativa invirtiendo los valores de píxeles
   for (int i = 0; i < height; i++)
   {
      for (int j = 0; j < width; j++)
      {
         newImageData[i * width + j] = 255 - imageData[i * width + j];
      }
   }

   // Abrir el archivo de salida en modo escritura binaria ("wb")
   FILE *fo = fopen("girlface_negative.bmp", "wb");

   // Escribir el encabezado en el archivo de salida
   fwrite(imageHeader, sizeof(unsigned char), 54, fo);

   // Si la profundidad de bits es menor o igual a 8, escribir la tabla de colores
   if (bitDepth <= 8)
   {
      fwrite(colorTable, sizeof(unsigned char), 1024, fo);
   }

   // Escribir los datos de la imagen negativa en el archivo de salida
   fwrite(newImageData, sizeof(unsigned char), imgDataSize, fo);

   // Imprimir un mensaje de éxito
   printf(":)!\n");

   // Cerrar ambos archivos y liberar la memoria asignada dinámicamente
   fclose(fo);
   fclose(fp);
   free(imageData);
   free(newImageData);

   return 0;
}
