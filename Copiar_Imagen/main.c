#include <stdio.h>
#include <stdlib.h>

/*
    los archivos bmp tienen :

    54 bytes que corresponden al ImageHeader
    1028 bytes que corresponden al ColorTable
    
*/

int main()
{
    // Abrir el archivo de entrada para lectura binaria y el archivo de salida para escritura binaria
    FILE *streamIn = fopen("imagenes/cameraman.bmp", "rb"); //Archivo a copiar
    FILE *fo = fopen("imagenes/cameraman_copy.bmp", "wb");  //Se crear archivo donde se guardaran los datos copiados de streamIn

    // Verificar si el archivo de entrada se abrió correctamente
    if (streamIn == (FILE *)0)
    {
        printf("Error, no se puede abrir el archivo\n");
    }
    
    // Declarar arreglos para el encabezado y la tabla de colores
    unsigned char header[54];
    unsigned char colorTable[1024];

    // Leer los primeros 54 bytes (encabezado) del archivo BMP de entrada
    for (int i = 0; i < 54; i++)
    {
        header[i] = getc(streamIn);
    }

    // Obtener información importante del encabezado, como ancho, alto y profundidad de bits
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int bitDepth = *(int *)&header[28];

    // Si la profundidad de bits es menor o igual a 8, leer la tabla de colores
    if (bitDepth <= 8)
    {
         fread(colorTable, sizeof(unsigned char), 1024, streamIn);
    }

    // Escribir el encabezado en el archivo de salida
    fwrite(header, sizeof(unsigned char), 54, fo);

    // Declarar un búfer para los datos de píxeles
    unsigned char buf[height * width];

    // Leer los datos de píxeles del archivo de entrada
    fread(buf, sizeof(unsigned char), (height * width), streamIn);
    
    // Si la profundidad de bits es menor o igual a 8, escribir la tabla de colores en el archivo de salida
    if (bitDepth <= 8)
    {
       fwrite(colorTable, sizeof(unsigned char), 1024, fo);
    }

    // Escribir los datos de píxeles en el archivo de salida
    fwrite(buf, sizeof(unsigned char), (height * width), fo);

    // Cerrar ambos archivos
    fclose(fo);
    fclose(streamIn);

    // Mostrar un mensaje de exito y las dimensiones de la imagen
    printf("Se ha copiado la imagen !\n");
    printf("Ancho : %d\n", width);
    printf("Alto : %d\n", height);

    return 0;
}
