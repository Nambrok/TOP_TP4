#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "bmp_reader.h"

#include "mpi.h"

#define ROOT 0

int_bmp_pixel_t ** reallocationTab(int_bmp_pixel_t ** tabOrigin, int height, int width){
        int_bmp_pixel_t * tmp = malloc(height * width * sizeof(int_bmp_pixel_t));
        int_bmp_pixel_t ** newTab = NULL;
        int i, j;
        newTab = malloc(sizeof(int_bmp_pixel_t*) * height);
        for(i = 0; i<height; i++) {
                newTab[i] = &(tmp[i * width]);
        }

        for(i = 0; i<height; i++) {
                for(j = 0; j<width; j++) {
                        newTab[i][j] = tabOrigin[i][j];
                }
        }
        // Liberation_image_lue(tabOrigin);
        return newTab;
}

int main(int argc, char* argv[])
{
        MPI_Init(&argc, &argv);
        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int i, j;

        int_bmp_pixel_t * tabTmp = NULL;
        int_bmp_pixel_t ** tab = NULL;
        int_bmp_pixel_t ** tabOrigin = NULL;

        int height, width;
        int_bmp_pixel_t tmp;

/* Création du type MPI pour un int_bmp_pixel_t*/
        int blocklengths[3] = {1, 1, 1};
        MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
        MPI_Datatype mpi_pixel_type;
        MPI_Aint offsets[3];

        offsets[0] = offsetof(int_bmp_pixel_t, Rouge);
        offsets[1] = offsetof(int_bmp_pixel_t, Bleu);
        offsets[2] = offsetof(int_bmp_pixel_t, Vert);


        MPI_Type_create_struct(1, blocklengths, offsets, types, &mpi_pixel_type);
        MPI_Type_commit(&mpi_pixel_type);

        //Il faut lire une seule fois
        if(rank == ROOT) {
                tabOrigin = Lecture_image("pingouin.bmp");
                height = get_img_heigh();
                width = get_img_width();
                tabOrigin = reallocationTab(tabOrigin, height, width);
        }
        MPI_Bcast(&height, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        MPI_Bcast(&width, 1, MPI_INT, ROOT, MPI_COMM_WORLD);



        printf("%d : height : %d, width : %d\n", rank, height, width);


        int heightLoc = height / nproc;

        if(height/nproc % 2) {
                printf("Nombre impair attention.\n");
                //TODO: Prendre en compte une heightLoc différente pour plusieurs d'entre eux
        }

        tabTmp = malloc(heightLoc * width * sizeof(int_bmp_pixel_t));
        tab = malloc(sizeof(int_bmp_pixel_t*) * heightLoc);
        for(i = 0; i<heightLoc; i++) {
                tab[i] = &tabTmp[i * width];
        }

        // int * sendCounts = calloc(nproc, sizeof(int));
        // int * displs = calloc(nproc, sizeof(int));


        // for(i = 0; i<nproc; i++) {
        //         sendCounts[i] = heightLoc;
        //         displs[i] = i * sizeof(int_bmp_pixel_t);
        // }

        // MPI_Scatterv(tabOrigin, sendCounts, displs, mpi_pixel_type, tab, sendCounts[rank], mpi_pixel_type, ROOT, MPI_COMM_WORLD);
        MPI_Scatter(tabOrigin[0], height*width, mpi_pixel_type, tab[0], height*width, mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        //Miroir Vertical
        for(i = 0; i < heightLoc; i++)
        {
                for(j = 0; j < width/2; j++)
                {
                        printf("Rang : %d : (R,G,B) : (%d, %d, %d)\n", rank, tab[i][j].Rouge, tab[i][j].Vert, tab[i][j].Bleu);
                        tmp = tab[i][width - j - 1];
                        tab[i][width - j - 1] = tab[i][j];
                        tab[i][j] = tmp;
                }
        }

        // MPI_Gatherv(tab, sendCounts[rank], mpi_pixel_type, tabOrigin, sendCounts, displs, mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        //Miroir horizontal
        // for(i = 0; i < get_img_heigh()/2; i++)
        // {
        //         for(j= 0; j < get_img_width(); j++)
        //         {
        //                 // tab[i][j].Vert = 0;
        //                 tmp = tab2[get_img_heigh() - i - 1][j];
        //                 tab2[get_img_heigh() - i - 1][j] = tab[i][j];
        //                 tab2[i][j] = tmp;
        //         }
        // }

        if(rank == 0) {
                Ecriture_image(tabOrigin, "copie.bmp");
                // Liberation_image_lue(tabOrigin);
        }


        MPI_Finalize();
        return EXIT_SUCCESS;

}
