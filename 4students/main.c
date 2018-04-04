#include <stdlib.h>
#include <stdio.h>
#include "bmp_reader.h"

int main(int argc, char* argv[])
{
int i, j;
    int width = get_img_width_onemalloc("pingouin.bmp");
    int_bmp_pixel_t (*tab)[width] = Lecture_image_onemalloc("pingouin.bmp");

for(i = 0; i < get_img_heigh(); i++)
{
   for(j= 0; j < width; j++)
   {
     //tab[i][j].Vert = 0;
   }   
}

Ecriture_image_onemalloc(tab, "copie.bmp");
Liberation_image_lue_onemalloc(tab);
Liberation_finale();

return EXIT_SUCCESS;

}
