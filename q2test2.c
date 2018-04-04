#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "bmp_reader.h"

#include "mpi.h"

#define ROOT 0

int_bmp_pixel_t * transformerOneDim(int_bmp_pixel_t ** tab, int height, int width){
        //Renvoi un tableau d'une dimension de tab
        int_bmp_pixel_t * tabO = calloc(height * width, sizeof(int_bmp_pixel_t));
        int i, j;

        for(i = 0; i<height; i++) {
                for(j = 0; j<width; j++) {
                        tabO[(i * width) + j] = tab[i][j];
                }
        }

        return tabO;
}

int_bmp_pixel_t * transformerOneDimTransposed(int_bmp_pixel_t ** tab, int height, int width){
        //Renvoi un tableau d'une dimension de tab
        int_bmp_pixel_t * tabO = calloc(height * width, sizeof(int_bmp_pixel_t));
        int i, j;

        for(i = 0; i<width; i++) {
                for(j = 0; j<height; j++) {
                        tabO[(i * width) + j] = tab[j][i];
                }
        }

        return tabO;
}

void transformerDeuxDim(int_bmp_pixel_t ** tab, int_bmp_pixel_t * rbuf, int height, int width){
        int i, j;
        for(i = 0; i<height; i++) {
                for(j = 0; j<width; j++) {
                        tab[i][j] = rbuf[i * width + j];
                }
        }
}

//Attention transposé marche ici car la matrice est carré

void transformerDeuxDimTransposed(int_bmp_pixel_t ** tab, int_bmp_pixel_t * rbuf, int height, int width){
        int i, j;
        for(i = 0; i<width; i++) {
                for(j = 0; j<height; j++) {
                        tab[j][i] = rbuf[i * width + j];
                }
        }
}

void miroirVertical(char * file){
        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int i, j;
        int height, width;
        int_bmp_pixel_t ** tab = NULL;
        int_bmp_pixel_t * tabOneDim = NULL;

        int_bmp_pixel_t tmp;

        /* Création du type MPI pour un int_bmp_pixel_t*/
        int blocklengths[3] = {1, 1, 1};
        MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
        MPI_Datatype mpi_pixel_type;
        MPI_Aint offsets[3];

        offsets[0] = offsetof(int_bmp_pixel_t, Rouge);
        offsets[1] = offsetof(int_bmp_pixel_t, Bleu);
        offsets[2] = offsetof(int_bmp_pixel_t, Vert);

        MPI_Type_create_struct(3, blocklengths, offsets, types, &mpi_pixel_type);
        MPI_Type_commit(&mpi_pixel_type);


        if(rank == ROOT) {
                tab = Lecture_image(file);
                height = get_img_heigh();
                width = get_img_width();
                tabOneDim = transformerOneDim(tab, height, width);
        }



        MPI_Bcast(&height, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        MPI_Bcast(&width, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

        int heightLoc = height / nproc;



        int * displs, * counts;
        counts = calloc(nproc, sizeof(int));
        displs = calloc(nproc, sizeof(int));
        for(i = 0; i<nproc; i++) {
                displs[i] = i * heightLoc * width;
                counts[i] = heightLoc * width;
        }
        counts[i-1] += height%nproc * width;

        int_bmp_pixel_t * tabLocal = malloc(counts[rank] * sizeof(int_bmp_pixel_t));

        MPI_Scatterv(tabOneDim, counts, displs, mpi_pixel_type, tabLocal, counts[rank], mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        //Miroir Vertical
        int heightSelf = counts[rank] / width;
        for(i = 0; i < heightSelf; i++) {
                for(j = 0; j < width/2; j++) {
                        tmp = tabLocal[(i * width) + (width - j - 1)];
                        tabLocal[(i * width) + (width - j - 1)] = tabLocal[(i * width) + j];
                        tabLocal[i * width + j] = tmp;
                }
        }

        int_bmp_pixel_t * rbuf = NULL;


        rbuf = calloc(height * width, sizeof(int_bmp_pixel_t));


        MPI_Gatherv(tabLocal, counts[rank], mpi_pixel_type, rbuf, counts, displs, mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        if(rank == ROOT) {
                transformerDeuxDim(tab, rbuf, height, width);
                Ecriture_image(tab, "copie.bmp");
                Liberation_image_lue(tab);
        }
}

void miroirHorizontal(char * file){
        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int i, j;
        int height, width;
        int_bmp_pixel_t ** tab = NULL;
        int_bmp_pixel_t * tabOneDim = NULL;

        int_bmp_pixel_t tmp;

        /* Création du type MPI pour un int_bmp_pixel_t*/
        int blocklengths[3] = {1, 1, 1};
        MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
        MPI_Datatype mpi_pixel_type;
        MPI_Aint offsets[3];

        offsets[0] = offsetof(int_bmp_pixel_t, Rouge);
        offsets[1] = offsetof(int_bmp_pixel_t, Bleu);
        offsets[2] = offsetof(int_bmp_pixel_t, Vert);

        MPI_Type_create_struct(3, blocklengths, offsets, types, &mpi_pixel_type);
        MPI_Type_commit(&mpi_pixel_type);


        if(rank == ROOT) {
                tab = Lecture_image(file);
                height = get_img_heigh();
                width = get_img_width();
                tabOneDim = transformerOneDimTransposed(tab, height, width);
        }



        MPI_Bcast(&height, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        MPI_Bcast(&width, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

        int heightLoc = height / nproc;



        int * displs, * counts;
        counts = calloc(nproc, sizeof(int));
        displs = calloc(nproc, sizeof(int));
        for(i = 0; i<nproc; i++) {
                displs[i] = i * heightLoc * width;
                counts[i] = heightLoc * width;
        }
        counts[i-1] += height%nproc * width;

        int_bmp_pixel_t * tabLocal = malloc(counts[rank] * sizeof(int_bmp_pixel_t));

        MPI_Scatterv(tabOneDim, counts, displs, mpi_pixel_type, tabLocal, counts[rank], mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        //Miroir Vertical
        int heightSelf = counts[rank] / width;
        for(i = 0; i < heightSelf; i++) {
                for(j = 0; j < width/2; j++) {
                        tmp = tabLocal[(i * width) + (width - j - 1)];
                        tabLocal[(i * width) + (width - j - 1)] = tabLocal[(i * width) + j];
                        tabLocal[i * width + j] = tmp;
                }
        }

        int_bmp_pixel_t * rbuf = NULL;


        rbuf = calloc(height * width, sizeof(int_bmp_pixel_t));


        MPI_Gatherv(tabLocal, counts[rank], mpi_pixel_type, rbuf, counts, displs, mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        if(rank == ROOT) {
                transformerDeuxDimTransposed(tab, rbuf, height, width);
                Ecriture_image(tab, "copie.bmp");
                Liberation_image_lue(tab);
        }
}

int main(int argc, char* argv[])
{
        MPI_Init(&argc, &argv);
        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        miroirVertical("pingouin.bmp");
        miroirHorizontal("pingouin.bmp");



        MPI_Finalize();
        return EXIT_SUCCESS;

}
