#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Abrir el archivo de entrada en modo lectura binaria y el archivo de salida en modo escritura binaria
    FILE *fIn = fopen("imagenes/lena_color.bmp", "rb");
    FILE *fOut = fopen("imagenes/lena_gray.bmp", "wb");

    // Declarar arreglos para el encabezado de la imagen y la tabla de colores
    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    // Verificar si el archivo de entrada se abrió correctamente
    if (fIn == NULL)
    {
        printf("Unable to open image\n");
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

    // Si la profundidad de bits es menor o igual a 8, leer y escribir la tabla de colores
    if (bitDepth <= 8)
    {
        fread(colorTable, sizeof(unsigned char), 1024, fIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    // Calcular el tamaño de la imagen en píxeles
    int imgSize = height * width;

    // Declarar un búfer para almacenar los valores de píxeles (RGB)
    unsigned char buffer[imgSize][3];

    // Leer los valores de píxeles (RGB) del archivo de entrada
    for (int i = 0; i < imgSize; i++)
    {
        buffer[i][0] = getc(fIn);  // Canal Rojo --> leer un byte del archivo de entrada
        buffer[i][1] = getc(fIn);  // Canal Verde
        buffer[i][2] = getc(fIn);  // Canal Azul

        int temp = 0;
        /*
        
        Convertir el valor de píxel a escala de grises utilizando ponderaciones

        Canal Rojo (R) - 0.3: El canal rojo se pondera con un valor relativamente bajo (30%) porque, en general, nuestros ojos son menos sensibles a 
        los tonos rojos en comparación con los verdes y los azules. Esto significa que los cambios en el canal rojo tienen menos impacto en la percepción visual de luminancia.

        Canal Verde (G) - 0.59: El canal verde se pondera con un valor más alto (59%) porque nuestros ojos son más sensibles al verde. Además, el verde es un color fundamental 
        en la representación de la mayoría de las imágenes naturales.
        Esto significa que los cambios en el canal verde tienen un impacto significativo en la percepción visual de luminancia.

        Canal Azul (B) - 0.11: El canal azul se pondera con el valor más bajo (11%) porque nuestros ojos son menos sensibles a los tonos azules. 
        Además, en muchas imágenes naturales, el canal azul suele conten
        */
        temp = (buffer[i][0] * 0.3) + (buffer[i][1] * 0.59) + (buffer[i][2] * 0.11);

        // Escribir el valor de píxel en escala de grises en el archivo de salida tres veces (para R, G y B)
        putc(temp, fOut);
        putc(temp, fOut);
        putc(temp, fOut);
    }

    printf("Se hizo gris c:\n");

    // Cerrar ambos archivos
    fclose(fIn);
    fclose(fOut);

    return 0;
}
