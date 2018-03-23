#ifndef __BMP_READER_H__
#define __BMP_READER_H__

struct int_bmp_pixel_s
{
    int Rouge;
    int Bleu;
    int Vert;
};
typedef struct int_bmp_pixel_s int_bmp_pixel_t;

/* Accessor header info: Width */
int get_img_width(void);

/* Accessor header info: Height */
int get_img_heigh(void);

/* Accessor header info: Dump infos */
void print_all_header_infos(void);

/* Read BMP img */
int_bmp_pixel_t ** Lecture_image( char *);

/* Write BMP img */
int Ecriture_image( int_bmp_pixel_t **, char*);

void Liberation_image_lue(int_bmp_pixel_t ** img);

void Liberation_finale(void);
#endif /* __BMP_READER_H__ */
