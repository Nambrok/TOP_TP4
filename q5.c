#include <stdio.h>
#include <stdlib.h>

#include "bmp_reader.h"

void somme(int_bmp_pixel_t * res, int_bmp_pixel_t ** tab, int i, int j, int height, int width){
        double resRed = 0;
        double resBlue = 0;
        double resGreen = 0;
        int nombreCases = 0;

        resRed += tab[i][j].Rouge;
        resBlue += tab[i][j].Bleu;
        resGreen += tab[i][j].Vert;
        nombreCases++;

        if(i > 1) {
                resRed += tab[i-1][j].Rouge;
                resBlue += tab[i-1][j].Bleu;
                resGreen += tab[i-1][j].Vert;
                nombreCases++;
        }
        if(i > 1 && j > 1) {
                resRed += tab[i-1][j-1].Rouge;
                resBlue += tab[i-1][j-1].Bleu;
                resGreen += tab[i-1][j-1].Vert;
                nombreCases++;
        }
        if(j > 1) {
                resRed += tab[i][j-1].Rouge;
                resBlue += tab[i][j-1].Bleu;
                resGreen += tab[i][j-1].Vert;
                nombreCases++;
        }
        if(i > 1 && j < width-1) {
                resRed += tab[i-1][j+1].Rouge;
                resBlue += tab[i-1][j+1].Bleu;
                resGreen += tab[i-1][j+1].Vert;
                nombreCases++;
        }
        if(i<height-1) {
                resRed += tab[i+1][j].Rouge;
                resBlue += tab[i+1][j].Bleu;
                resGreen += tab[i+1][j].Vert;
                nombreCases++;
        }
        if(i < height-1 && j < width-1) {
                resRed += tab[i+1][j+1].Rouge;
                resBlue += tab[i+1][j+1].Bleu;
                resGreen += tab[i+1][j+1].Vert;
                nombreCases++;
        }
        if(j > 1 && i < height-1) {
                resRed += tab[i+1][j-1].Rouge;
                resBlue += tab[i+1][j-1].Bleu;
                resGreen += tab[i+1][j-1].Vert;
                nombreCases++;
        }

        // tab[i][j].Rouge = (int) (resRed / nombreCases);
        // tab[i][j].Bleu = (int) (resBlue / nombreCases);
        // tab[i][j].Vert = (int) (resGreen / nombreCases);

        res[i * width + j].Rouge = (int) (resRed / nombreCases);
        res[i * width + j].Bleu = (int) (resBlue / nombreCases);
        res[i * width + j].Vert = (int) (resGreen / nombreCases);
}

void transformerDeuxDim(int_bmp_pixel_t ** tab, int_bmp_pixel_t * rbuf, int height, int width){
        int i, j;
        for(i = 0; i<height; i++) {
                for(j = 0; j<width; j++) {
                        tab[i][j] = rbuf[i * width + j];
                }
        }
}

int main(int argc, char * argv[]){


        int i, j;
        int height, width;

        int_bmp_pixel_t ** tab = Lecture_image("pingouin.bmp");
        height = get_img_heigh();
        width = get_img_width();

        int_bmp_pixel_t * res = calloc(height * width, sizeof(int_bmp_pixel_t));

        for(i = 0; i<height; i++) {
                for(j = 0; j<width; j++) {
                        somme(res, tab, i, j, height, width);
                }
        }

        transformerDeuxDim(tab, res, height, width);

        Ecriture_image(tab, "copie.bmp");
        Liberation_image_lue(tab);

        return EXIT_SUCCESS;
}
