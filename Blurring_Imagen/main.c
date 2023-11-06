#include <stdio.h>
#include <stdlib.h>

int main()
{
    float base = 9;
     // Abrir el archivo de entrada "frutas.bmp" en modo lectura binaria
     FILE *fIn = fopen("frutas.bmp", "rb");
     // Crear un archivo de salida "frutas_blur.bmp" en modo escritura binaria
     FILE *fOut = fopen("frutas_blur.bmp", "wb");

     // Declarar arreglos para el encabezado de la imagen y la tabla de colores
     unsigned char imgHeader[54];
     unsigned char colorTable[1024];

     // Verificar si el archivo de entrada se abrió correctamente
     if (fIn == NULL)
     {
         printf("Error, no se abrió el archivo\n");
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
     int width  = *(int *)&imgHeader[18];
     int bitDepth = *(int *)&imgHeader[28];

     // Si la profundidad de bits es menor o igual a 8, leer y escribir la tabla de colores
     if (bitDepth <= 8)
     {
         fread(colorTable, sizeof(unsigned char), 1024, fIn);
         fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
     }

     // Calcular el tamaño de la imagen en píxeles
     int imgSize =  height * width;

     // Declarar arreglos para almacenar los valores de píxeles originales y los valores de píxeles de salida
     unsigned char buffer[imgSize][3];  // Para los valores de píxeles originales (RGB)
     unsigned char output_buffer[imgSize][3];  // Para los valores de píxeles de salida (RGB)

     // Leer los valores de píxeles originales del archivo de entrada
     for (int i = 0; i < imgSize; i++)
     {
         buffer[i][2] = getc(fIn);  // Canal Rojo
         buffer[i][1] = getc(fIn);  // Canal Verde
         buffer[i][0] = getc(fIn);  // Canal Azul
     }

     // Declarar un kernel de filtrado promedio 3x3 para el desenfoque
     float kernel[3][3] = {{1.0/base, 1.0/base, 1.0/base},
                           {1.0/base, 1.0/base, 1.0/base},
                           {1.0/base, 1.0/base, 1.0/base}
                         };

    // Aplicar el filtro de desenfoque a la imagen
    for (int x = 1; x < height - 1; x++)
    {
        for (int y = 1; y < width - 1; y++)
        {
            float sum0 = 0.0;
            float sum1 = 0.0;
            float sum2 = 0.0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    sum0 += (float)kernel[i+1][j+1] * buffer[(x+i)*width+(y+j)][0];
                    sum1 += (float)kernel[i+1][j+1] * buffer[(x+i)*width+(y+j)][1];
                    sum2 += (float)kernel[i+1][j+1] * buffer[(x+i)*width+(y+j)][2];
                }
            }

            output_buffer[x*width+y][0] =  sum0;
            output_buffer[x*width+y][1] =  sum1;
            output_buffer[x*width+y][2] =  sum2;
        }
    }

    // Escribir los valores de píxeles de salida en el archivo de salida
    for (int i = 0; i < imgSize; i++)
    {
        putc(output_buffer[i][2], fOut);  // Canal Rojo
        putc(output_buffer[i][1], fOut);  // Canal Verde
        putc(output_buffer[i][0], fOut);  // Canal Azul
    }

    // Cerrar ambos archivos
    fclose(fOut);
    fclose(fIn);
    return 0;
}