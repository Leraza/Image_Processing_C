#include <stdio.h>
#include <stdlib.h>

#define MAX_PIXEL   255

int main()
{
    // Abrir el archivo de entrada "lena_color.bmp" en modo lectura binaria
    FILE  *fIn = fopen("Imagenes/lena_color.bmp", "rb");
    // Crear un archivo de salida "lena_sepia.bmp" en modo escritura binaria
    FILE  *fOut = fopen("Imagenes/lena_sepia.bmp", "wb");

    // Declarar arreglos para el encabezado de la imagen y la tabla de colores
    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    // Verificar si el archivo de entrada se abrió correctamente
    if (fIn == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return 1; // Salir del programa con un código de error
    }

    // Leer los primeros 54 bytes (encabezado) del archivo BMP de entrada
    for (int i = 0; i < 54; i++)
    {
        imgHeader[i] = getc(fIn);
    }
    
    // Escribir el encabezado en el archivo de salida
    fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

    // Obtener información importante del encabezado, como alto, ancho y profundidad de bits
    int height = *(int*)&imgHeader[22];
    int width  = *(int*)&imgHeader[18];
    int bitDepth = *(int*)&imgHeader[28];

    // Si la profundidad de bits es menor o igual a 8, leer y escribir la tabla de colores
    if (bitDepth <= 8)
    {
        fread(colorTable, sizeof(unsigned char), 1024, fIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    // Calcular el tamaño de la imagen en píxeles
    int imgSize = height * width;
    int r, g, b;
    unsigned char buffer[imgSize][3];

    // Leer los valores de píxeles originales del archivo de entrada
    for (int i = 0; i < imgSize; i++)
    {
        r = g = b = 0;
        buffer[i][0] = getc(fIn);      // Canal Rojo
        buffer[i][1] = getc(fIn);      // Canal Verde
        buffer[i][2] = getc(fIn);      // Canal Azul

        // Aplicar la conversión a efecto sepia a los canales de color
        r = (buffer[i][0] * 0.393) + (buffer[i][1] * 0.769) + (buffer[i][2] * 0.189);
        g = (buffer[i][0] * 0.349) + (buffer[i][1] * 0.686) + (buffer[i][2] * 0.168);
        b = (buffer[i][0] * 0.272) + (buffer[i][1] * 0.534) + (buffer[i][2] * 0.131);

        // Asegurarse de que los valores no superen MAX_PIXEL (255)
        if (r > MAX_PIXEL) { r = MAX_PIXEL; }
        if (g > MAX_PIXEL) { g = MAX_PIXEL; }
        if (b > MAX_PIXEL) { b = MAX_PIXEL; }

        // Escribir los valores de los canales de color modificados en el archivo de salida
        putc(b, fOut); // Canal Azul
        putc(g, fOut); // Canal Verde
        putc(r, fOut); // Canal Rojo
    }

    // Cerrar ambos archivos
    fclose(fOut);
    fclose(fIn);

    return 0; // Terminar el programa con éxito
}
