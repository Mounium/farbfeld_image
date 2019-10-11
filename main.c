#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "image.h"


void algo(Pixel* img, uint32_t w, uint32_t h) {
    for (int i = 0; i < w*h; i++) {
        img[i].r = img[i].r / 10000 * 10000;
        img[i].g = img[i].g / 10000 * 10000;
        img[i].b = img[i].b / 10000 * 10000;
    }
}

int main(void) {
    Image img = open_image("/home/florian/Documents/ELTE/c/image/dog.ff");

    algo(img.img, img.width, img.height);

    for (int i = 0; i < img.width*img.height; i++) {
        img.img[i].r = htons(img.img[i].r);
        img.img[i].g = htons(img.img[i].g);
        img.img[i].b = htons(img.img[i].b);
        img.img[i].a = htons(img.img[i].a);
    }

    write_image("/home/florian/Documents/ELTE/c/image/res.ff", &img);
}
