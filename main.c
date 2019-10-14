#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "image.h"
#include "complex.h"

#include "mandelbrot.h"

typedef struct Set Set;
struct Set {
    Pixel *val;
    Set *parent;
};

struct Coord {
    int x;
    int y;
};

Set *set_find(Set *s) {
    while (s->parent) {
        s = s->parent;
    }
    return s;
}

void set_find_replace(Set *s, Set *val) {
    Set *next = s;
    while (next) {
        next = s->parent;
        s->parent = val;
        s = next;
    }
}

void set_union(Set *a, Set *b) {
//    Set *root = set_find(a);
//    set_find_replace(b, root);

    Set *root = set_find(b);
    root->parent = a;
}

void average(Image *img) {
    for (int i = 0; i < img->width * img->height; i++) {
        Pixel *p = &img->img[i];
//        uint16_t avg = (p->r + p->g + p->b) / 3;
        p->r = UINT16_MAX;
        p->g = UINT16_MAX;
        p->b = UINT16_MAX;
    }
}

size_t rel_to_abs(size_t h, size_t w, Image *img) {
    return img->width * h + w;
}

struct Coord abs_to_rel(size_t i, Image *img) {
    return (struct Coord) {
        .x = i%img->width,
        .y = i/img->width
    };
}

int main(void) {
    Image img = open_image("/home/florian/Documents/ELTE/c/image/dog.ff");

    average(&img);

/*
//    Set* sets = malloc(img.width*img.height*sizeof(Set));
    Set sets[img.width * img.height];

    for (size_t i = 0; i < img.width * img.height; i++) {
        sets[i] = (Set) {.val = &img.img[i], .parent = NULL};
    }

    for (size_t i = 0; i < img.width * img.height; i++) {
        size_t h = i / img.width;
        size_t w = i % img.width;
        for (int j = -1; j <= 1; j += 2) {
            for (int k = -1; k <= 1; k += 2) {
                size_t z = rel_to_abs(h + j, w + k, &img);
                if (z && z < img.width*img.height && abs(img.img[z].r - img.img[i].r) < 10) {
                    set_union(&sets[i], &sets[z]);
                }
            }
        }
    }

    for (size_t i = 0; i < img.width * img.height; i++) {
        uint16_t v = set_find(&sets[i])->val->r;
        img.img[i].r = v;
        img.img[i].g = v;
        img.img[i].b = v;
    }*/

    write_image("/home/florian/Documents/ELTE/c/image/res.ff", &img);
}
