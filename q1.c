#include <stdlib.h>
#include <stdio.h>
#include "bmp_reader.h"

int main(int argc, char* argv[])
{
        int i, j;
        int_bmp_pixel_t ** tab;

        tab = Lecture_image("pingouin.bmp");

        int_bmp_pixel_t ** tab2;

        tab2 = Lecture_image("pingouin.bmp");

        //Miroir Vertical
        for(i = 0; i < get_img_heigh(); i++)
        {
                for(j= 0; j < get_img_width()/2; j++)
                {
                        // tab[i][j].Vert = 0;
                        tab2[i][get_img_width() - j - 1] = tab[i][j];
                        tab2[i][j] = tab[i][get_img_width() - j - 1];

                }
        }

        //Miroir horizontal
        for(i = 0; i < get_img_heigh()/2; i++)
        {
                for(j= 0; j < get_img_width(); j++)
                {
                        // tab[i][j].Vert = 0;
                        tab2[get_img_heigh() - i - 1][j] = tab[i][j];
                        tab2[i][j] = tab2[get_img_heigh() - i - 1][j];
                }
        }

        Ecriture_image(tab2, "copie.bmp");
        Liberation_image_lue(tab);
        Liberation_image_lue(tab2);

        return EXIT_SUCCESS;

}
