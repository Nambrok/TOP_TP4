#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "bmp_reader.h"

#include "mpi.h"

#define ROOT 0

int main(int argc, char* argv[])
{
        MPI_Init(&argc, &argv);
        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int i, j;

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

        MPI_Type_create_struct(width, blocklengths, offsets, types, &mpi_pixel_type); //Faut faire passer chaque ligne par ligne
        MPI_Type_commit(&mpi_pixel_type);

        //Il faut lire une seule fois
        if(rank == ROOT) {
                tabOrigin = Lecture_image("pingouin.bmp");
                height = get_img_heigh();
                width = get_img_width();
        }
        MPI_Bcast(&height, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        MPI_Bcast(&width, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

        printf("%d : height : %d, width : %d\n", rank, height, width);

        int heightLoc = height / nproc;
        tab = malloc(sizeof(int_bmp_pixel_t*) * heightLoc);
        for(i = 0; i<heightLoc; i++) {
                tab[i] = malloc(sizeof(int_bmp_pixel_t) * width);
        }

        int * sendCounts = calloc(nproc, sizeof(int));
        int * displs = calloc(nproc, sizeof(int));

        for(i = 0; i<nproc; i++) {
                sendCounts[i] = heightLoc;
                displs[i] = i * sizeof(int_bmp_pixel_t) * width;
        }

        MPI_Scatterv(tabOrigin, sendCounts, displs, mpi_pixel_type, tab, sendCounts[rank], mpi_pixel_type, ROOT, MPI_COMM_WORLD);

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

        // MPI_Scatterv(tabOrigin, sendCounts, displs, mpi_pixel_type, tab, sendCounts[rank], mpi_pixel_type, ROOT, MPI_COMM_WORLD);
        MPI_Gatherv(tab, sendCounts[rank], mpi_pixel_type, tabOrigin, sendCounts, displs, mpi_pixel_type, ROOT, MPI_COMM_WORLD);

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
                Liberation_image_lue(tabOrigin);
        }

        for(i = 0; i<heightLoc; i++) {
                free(tab[i]);
        }
        free(tab);
        free(sendCounts);
        free(displs);


        MPI_Finalize();
        return EXIT_SUCCESS;

}
