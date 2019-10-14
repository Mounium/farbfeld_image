//
// Created by florian on 10/13/19.
//

#include "image.h"
#include "mandelbrot.h"
#include "math.h"

struct Coord {
    int x;
    int y;
};

struct Coord abs_to_rel(size_t i, Image *img) {
    return (struct Coord) {
            .x = i%img->width,
            .y = i/img->width
    };
}

double min(double a, double b) {
    return (a > b) ? b : a;
}

int main(void) {
    int w = 1921;
    int h = 1080;

    Image img = create_image(w, h);

    int const perc = w*h/100*5;

    double const zoom = 1900000;

    int const x0 = w/2;
    int const y0 = h/2;

    double const x1 = -0.7442;
    double const y1 = -.1;

    for (int i = 0; i < img.height * img.width; i+=1) {
        if (i%perc == 0) {
            printf("\r");
            for (int j = 0; j < w*h/perc; j++) {
                printf(j < i/perc ? "." : " ");
            }
            printf(" [%d/100]", i/perc*5);
            fflush(stdout);
        }
        struct Coord point = abs_to_rel(i, &img);
//        double complex c = (point.x-1000)/550.0 + (point.y-550)/550.0*I;
        double complex c = (point.x-x0)/zoom+x1 + (point.y-y0)/zoom*I+y1*I;

        int n = mandelbrot(c);
//        printf("(%f, %f): %d\n", creal(c), cimag(c), n);
        if (!n) {
            img.img[i] = (Pixel) { .r = 0, .a = UINT16_MAX };
        } else {
            uint16_t x = UINT16_MAX-UINT16_MAX/6000.0*(min(log(n)*860, 6000));
            img.img[i] = (Pixel) { .r = x, .g = x, .b = x, .a = UINT16_MAX };
        }
    }

    write_image("/home/florian/Documents/ELTE/c/image/mandelbrot.ff", &img);
}
