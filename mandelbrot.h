//
// Created by florian on 10/13/19.
//

#ifndef IMAGE_MANDELBROT_H
#define IMAGE_MANDELBROT_H

#include "complex.h"

int const MAX_REC = 3000;

int mandelbrot(double complex c) {
    int n = 0;
    double complex z = 0;
    while(n < MAX_REC && creal(z)*creal(z)+cimag(z)*cimag(z) <= 4) {
        z = z*z + c;
        n++;
    }
    return (n < MAX_REC) ? n : 0;
}

#endif //IMAGE_MANDELBROT_H
