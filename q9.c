#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "bmp_reader.h"

#include "mpi.h"

#define ROOT 0

void somme(int_bmp_pixel_t * res, int_bmp_pixel_t * tab, int i, int j, int height, int width){
        double resRed = 0;
        double resBlue = 0;
        double resGreen = 0;
        int nombreCases = 0;

        resRed += tab[i * width + j].Rouge;
        resBlue += tab[i * width + j].Bleu;
        resGreen += tab[i * width + j].Vert;
        nombreCases++;

        if(i > 0) {
                resRed += tab[(i-1) * width + j].Rouge;
                resBlue += tab[(i-1) * width + j].Bleu;
                resGreen += tab[(i-1) * width + j].Vert;
                nombreCases++;
        }
        if(i > 0 && j > 0) {
                resRed += tab[(i-1) * width + (j-1)].Rouge;
                resBlue += tab[(i-1) * width + (j-1)].Bleu;
                resGreen += tab[(i-1) * width + (j-1)].Vert;
                nombreCases++;
        }
        if(j > 0) {
                resRed += tab[i * width + (j-1)].Rouge;
                resBlue += tab[i * width + (j-1)].Bleu;
                resGreen += tab[i * width + (j-1)].Vert;
                nombreCases++;
        }
        if(i > 0 && j < width-1) {
                resRed += tab[(i-1) * width + (j+1)].Rouge;
                resBlue += tab[(i-1) * width + (j+1)].Bleu;
                resGreen += tab[(i-1) * width + (j+1)].Vert;
                nombreCases++;
        }
        if(i<height-1) {
                resRed += tab[(i+1) * width + j].Rouge;
                resBlue += tab[(i+1) * width + j].Bleu;
                resGreen += tab[(i+1) * width + j].Vert;
                nombreCases++;
        }
        if(i < height-1 && j < width-1) {
                resRed += tab[(i+1) * width + (j+1)].Rouge;
                resBlue += tab[(i+1) * width + (j+1)].Bleu;
                resGreen += tab[(i+1) * width + (j+1)].Vert;
                nombreCases++;
        }
        if(j > 0 && i < height-2) {
                resRed += tab[(i+1) * width + (j-1)].Rouge;
                resBlue += tab[(i+1) * width + (j-1)].Bleu;
                resGreen += tab[(i+1) * width + (j-1)].Vert;
                nombreCases++;
        }

        res[i * width + j].Rouge = (int) (resRed / nombreCases);
        res[i * width + j].Bleu = (int) (resBlue / nombreCases);
        res[i * width + j].Vert = (int) (resGreen / nombreCases);
}

void sommeFantome(int_bmp_pixel_t * res, int_bmp_pixel_t * tab, int i, int j, int height, int width, int_bmp_pixel_t * fantomeHaut, int_bmp_pixel_t * fantomeBas){
        double resRed = 0;
        double resBlue = 0;
        double resGreen = 0;
        int nombreCases = 0;

        int rank, nproc;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);


        resRed += tab[i * width + j].Rouge;
        resBlue += tab[i * width + j].Bleu;
        resGreen += tab[i * width + j].Vert;
        nombreCases++;

        if(i > 0) {
                resRed += tab[(i-1) * width + j].Rouge;
                resBlue += tab[(i-1) * width + j].Bleu;
                resGreen += tab[(i-1) * width + j].Vert;
                nombreCases++;
        }
        if(i > 0 && j > 0) {
                resRed += tab[(i-1) * width + (j-1)].Rouge;
                resBlue += tab[(i-1) * width + (j-1)].Bleu;
                resGreen += tab[(i-1) * width + (j-1)].Vert;
                nombreCases++;
        }
        if(j > 0) {
                resRed += tab[i * width + (j-1)].Rouge;
                resBlue += tab[i * width + (j-1)].Bleu;
                resGreen += tab[i * width + (j-1)].Vert;
                nombreCases++;
        }
        if(i > 0 && j < width-1) {
                resRed += tab[(i-1) * width + (j+1)].Rouge;
                resBlue += tab[(i-1) * width + (j+1)].Bleu;
                resGreen += tab[(i-1) * width + (j+1)].Vert;
                nombreCases++;
        }
        if(i<height-1) {
                resRed += tab[(i+1) * width + j].Rouge;
                resBlue += tab[(i+1) * width + j].Bleu;
                resGreen += tab[(i+1) * width + j].Vert;
                nombreCases++;
        }
        if(i < height-1 && j < width-1) {
                resRed += tab[(i+1) * width + (j+1)].Rouge;
                resBlue += tab[(i+1) * width + (j+1)].Bleu;
                resGreen += tab[(i+1) * width + (j+1)].Vert;
                nombreCases++;
        }
        if(j > 0 && i < height-2) {
                resRed += tab[(i+1) * width + (j-1)].Rouge;
                resBlue += tab[(i+1) * width + (j-1)].Bleu;
                resGreen += tab[(i+1) * width + (j-1)].Vert;
                nombreCases++;
        }

        if(fantomeHaut != NULL && i == 0) {
                if(j > 0) {
                        resRed += fantomeHaut[j-1].Rouge;
                        resBlue += fantomeHaut[j-1].Bleu;
                        resGreen += fantomeHaut[j-1].Vert;
                        nombreCases++;
                }

                if(j<width-1) {
                        resRed += fantomeHaut[j+1].Rouge;
                        resBlue += fantomeHaut[j+1].Bleu;
                        resGreen += fantomeHaut[j+1].Vert;
                        nombreCases++;
                }

                resRed += fantomeHaut[j].Rouge;
                resBlue += fantomeHaut[j].Bleu;
                resGreen += fantomeHaut[j].Vert;
                nombreCases++;

        }

        if(fantomeBas != NULL && i == height-1) {
                if(j > 0) {
                        resRed += fantomeBas[j-1].Rouge;
                        resBlue += fantomeBas[j-1].Bleu;
                        resGreen += fantomeBas[j-1].Vert;
                        nombreCases++;
                }

                if(j<width-1) {
                        resRed += fantomeBas[j+1].Rouge;
                        resBlue += fantomeBas[j+1].Bleu;
                        resGreen += fantomeBas[j+1].Vert;
                        nombreCases++;
                }

                resRed += fantomeBas[j].Rouge;
                resBlue += fantomeBas[j].Bleu;
                resGreen += fantomeBas[j].Vert;
                nombreCases++;
        }

        res[i * width + j].Rouge = (int) (resRed / nombreCases);
        res[i * width + j].Bleu = (int) (resBlue / nombreCases);
        res[i * width + j].Vert = (int) (resGreen / nombreCases);
}

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

void transformerDeuxDimTransposed(int_bmp_pixel_t ** tab, int_bmp_pixel_t * rbuf, int height, int width){
        int i, j;
        for(i = 0; i<width; i++) {
                for(j = 0; j<height; j++) {
                        tab[j][i] = rbuf[i * width + j];
                }
        }
}

void flou(char * file){
        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int i, j;
        int height, width;
        int_bmp_pixel_t ** tab = NULL;
        int_bmp_pixel_t * tabOneDim = NULL;

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

        int * displs, * ReqCounts;
        ReqCounts = calloc(nproc, sizeof(int));
        displs = calloc(nproc, sizeof(int));
        for(i = 0; i<nproc; i++) {
                displs[i] = i * heightLoc * width;
                ReqCounts[i] = heightLoc * width;
        }
        ReqCounts[i-1] += height%nproc * width;

        int heightSelf = ReqCounts[rank] / width; //Savoir combien on doit en traiter personellement.

        int_bmp_pixel_t * tabLocal = malloc(ReqCounts[rank] * sizeof(int_bmp_pixel_t));
        int_bmp_pixel_t * res;
        MPI_Scatterv(tabOneDim, ReqCounts, displs, mpi_pixel_type, tabLocal, ReqCounts[rank], mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        res = calloc(ReqCounts[rank], sizeof(int_bmp_pixel_t));

        int_bmp_pixel_t * fantomeHaut = NULL;
        int_bmp_pixel_t * fantomeBas = NULL;
        MPI_Request request[4]; int ReqCount;
        if(nproc > 1) {
                if(rank == ROOT) {//Le premier processus
                        fantomeBas = calloc(width, sizeof(int_bmp_pixel_t));
                        MPI_Isend(&(tabLocal[(heightSelf-1) * width]), width, mpi_pixel_type, rank+1, 10, MPI_COMM_WORLD, &(request[0])); // On envoie notre dernière ligne en dessous
                        MPI_Irecv(fantomeBas, width, mpi_pixel_type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &(request[1]));
                        ReqCount = 2;
                }
                else if(rank == (nproc-1)) {//Le dernier
                        fantomeHaut = calloc(width, sizeof(int_bmp_pixel_t));
                        MPI_Irecv(fantomeHaut, width, mpi_pixel_type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &(request[0]));
                        MPI_Isend(tabLocal, width, mpi_pixel_type, rank-1, 10, MPI_COMM_WORLD, &(request[1])); //On envoie notre première ligne au dessus
                        ReqCount = 2;
                }
                else{//Les autres
                        fantomeHaut = calloc(width, sizeof(int_bmp_pixel_t));
                        fantomeBas = calloc(width, sizeof(int_bmp_pixel_t));
                        MPI_Irecv(fantomeHaut, width, mpi_pixel_type, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &(request[0]));
                        MPI_Isend(&(tabLocal[(heightSelf-1) * width]), width, mpi_pixel_type, rank+1, 10, MPI_COMM_WORLD, &(request[1]));
                        MPI_Irecv(fantomeBas, width, mpi_pixel_type, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &(request[2]));
                        MPI_Isend(tabLocal, width, mpi_pixel_type, rank-1, 10, MPI_COMM_WORLD, &(request[3]));
                        ReqCount = 4;
                }
        }


        for(i = 1; i < heightSelf-1; i++) {
                for(j = 0; j < width; j++) {
                        somme(res, tabLocal, i, j, heightSelf, width);
                }
        }

        MPI_Status sta[ReqCount];
        MPI_Waitall(ReqCount, request, sta);

        for(j = 0; j<width; j++) {
                sommeFantome(res, tabLocal, 0, j, heightSelf, width, fantomeHaut, fantomeBas);
                sommeFantome(res, tabLocal, heightSelf-1, j, heightSelf, width, fantomeHaut, fantomeBas);
        }

        int_bmp_pixel_t * rbuf = NULL;

        rbuf = calloc(height * width, sizeof(int_bmp_pixel_t));

        MPI_Gatherv(res, ReqCounts[rank], mpi_pixel_type, rbuf, ReqCounts, displs, mpi_pixel_type, ROOT, MPI_COMM_WORLD);

        if(rank == ROOT) {
                transformerDeuxDim(tab, rbuf, height, width);
                Ecriture_image(tab, "copie.bmp");
                Liberation_image_lue(tab);
                free(rbuf);
                free(tabOneDim);
        }
        free(tabLocal);
        free(ReqCounts);
        free(displs);
        MPI_Type_free(&mpi_pixel_type);
}

int main(int argc, char* argv[])
{
        MPI_Init(&argc, &argv);

        int nproc, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        flou("pingouin.bmp");

        MPI_Finalize();
        return EXIT_SUCCESS;

}
