//
// Created by florian on 10/11/19.
//

#ifndef IMAGE_IMAGE_H
#define IMAGE_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t a;
} Pixel;

typedef struct {
    uint32_t width;
    uint32_t height;
    Pixel* img;
} Image;

void channel_manipulation(Pixel* p, unsigned char mask, uint16_t func(uint16_t)) {
    uint16_t* val[] = { &p->r, &p->g, &p->b, &p->a };
    for (size_t i = 0; i < 4; i++) {
        if ((1U << i) & mask) {
            *val[i] = func(*val[i]);
        }
    }
}

Image
create_image(int w, int h) {
    Image img = (Image) {
        .width = w,
        .height = h,
        .img = NULL
    };
    img.img = malloc(img.width*img.height*sizeof(Pixel));

    return img;
}

Image
open_image(char const* filename) {
    FILE *file = fopen(filename, "r");

    Image img = { 0 };

    char magic[8];
    uint32_t  widthBE;
    uint32_t  heightBE;

    fread(magic, 8, 1, file);
    fread(&widthBE, sizeof(widthBE), 1, file);
    fread(&heightBE, sizeof(heightBE), 1, file);

    img.width = ntohl(widthBE);
    img.height = ntohl(heightBE);

    img.img = malloc(img.width*img.height*sizeof(Pixel));

    fread(img.img, sizeof(Pixel), img.width * img.height, file);

    for (int i = 0; i < img.width*img.height; i++) {
        channel_manipulation(&img.img[i], 255, ntohs);
    }

    fclose(file);

    return img;
}

void
write_image(char const* filename, Image* img) {
    FILE *file = fopen(filename, "w");

    for (int i = 0; i < img->width * img->height; i++) {
        channel_manipulation(&img->img[i], 255, htons);
    }

    uint32_t widthBE = htonl(img->width);
    uint32_t heightBE = htonl(img->height);

    fwrite("farbfeld", 8, 1, file);
    fwrite(&widthBE, sizeof(widthBE), 1, file);
    fwrite(&heightBE, sizeof(heightBE), 1, file);

    fwrite(img->img, sizeof(Pixel), img->width * img->height, file);

    free(img->img);

    fclose(file);
}

#endif //IMAGE_IMAGE_H
