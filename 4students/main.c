#include <stdlib.h>
#include <stdio.h>
#include "bmp_reader.h"

int main(int argc, char* argv[])
{
int i, j;
int_bmp_pixel_t ** tab,**tab2,**tab3;

tab = Lecture_image("pingouin.bmp");
tab2 = Lecture_image("pingouin.bmp");
tab3 = Lecture_image("pingouin.bmp");

for(i = 0; i < get_img_heigh(); i++)
{
   for(j= 0; j < get_img_width(); j++)
   {
     tab[i][j].Vert = 0;
   }   
}

Ecriture_image(tab, "copie.bmp");
Liberation_image_lue(tab);
Liberation_image_lue(tab2);
Liberation_image_lue(tab3);
Liberation_finale();

return EXIT_SUCCESS;

}
