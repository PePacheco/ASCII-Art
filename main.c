#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// para compilar: mingw32-make -f Makefile.mk

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);
char pixelToChar(int mean);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

char pixelToChar(int mean) 
{
    // . : c o C O 8 @  //32
    if (mean > 223 && mean < 255) {
        return '@';
    } 
    if (mean > 191) {
        return '8';
    }
    if(mean > 159) {
        return 'O';
    }
    if(mean > 127) {
        return 'C';
    }
    if(mean > 95) {
        return 'o';
    }
    if(mean > 63) {
        return 'c';
    }
    if(mean > 31) {
        return ':';
    }
    if(mean > 0) {
        return '.';
    }
    return '.';
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc == 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);
    int fatorReducao = (int)argv[2];

    //printf("Primeiros 10 pixels da imagem:\n");
    // for(int i=0; i<100; i++) {
    //     float i = (0.3 * (int)pic.img[i].r + 0.59 * (int)pic.img[i].g + 0.11 * (int)pic.img[i].b);
    //     printf("[%d %d %d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    // }
    //printf("\n");

    // Inverte as cores
    int c = 0;
    int size = ((pic.width + 1)*pic.height);
    char content[size];
    for(int i=0; i<size; i++) {
        
        if(c == pic.width - 1) {
            content[i] = '\n';
            c = 0;
            continue;
        }
        int red = pic.img[i].r * 0.3;
        int green = pic.img[i].g * 0.59;
        int blue = pic.img[i].b * 0.11;
        int intensity = red + green + blue;

        pic.img[i].r = intensity;
        pic.img[i].g = intensity;
        pic.img[i].b = intensity;
        content[i] = pixelToChar(intensity);
        c++;
    }
    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height,
        3, (const unsigned char*) pic.img);

    // Exemplo: gravando um arquivo saida.html
    FILE* arq = fopen("saida.html", "w"); // criar o arquivo: w
    if(arq == NULL) // 0, falso
    {
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    char* html="<html>\n \
                    <head>\n \
                    </head>\n \
                    <body style='background: black;' leftmargin=0 topmargin=0>\n \
                        <style>\n \
                            pre {\n \
                                color: white;\n \
                                font-family: Consolas;\n \
                                font-size: 8px;\n \
                            } \n \
                        </style>\n \
                        <pre> \n \
                        ";

    char* html2 = "     </pre>\n \
                    </body>\n \
                </html>";

    fprintf(arq, html);
    fprintf(arq, content);
    fprintf(arq, html2);

    fclose(arq);
    free(pic.img);
}