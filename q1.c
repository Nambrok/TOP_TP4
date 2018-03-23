#include <stdlib.h>
#include <stdio.h>
#include "bmp_reader.h"

int main(int argc, char* argv[])
{
        int i, j;
        int_bmp_pixel_t ** tab;

        tab = Lecture_image("pingouin.bmp");

        //Miroir Vertical
        for(i = 0; i < get_img_heigh(); i++)
        {
                for(j= 0; j < get_img_width()/2; j++)
                {
                        // tab[i][j].Vert = 0;
                        tab[i][get_img_width() - j - 1] = tab[i][j];

                }
        }

        //Miroir horizontal
        for(i = 0; i < get_img_heigh()/2; i++)
        {
                for(j= 0; j < get_img_width(); j++)
                {
                        // tab[i][j].Vert = 0;
                        tab[get_img_heigh() - i - 1][j] = tab[i][j];

                }
        }

        Ecriture_image(tab, "copie.bmp");
        Liberation_image_lue(tab);

        return EXIT_SUCCESS;

}
