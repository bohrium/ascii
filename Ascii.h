/* author: samtenka
 * create: 2018-03-29 
 * change: 2018-03-29 
 * descrp: interfaces for ASCII Art translation, building on Bitmap 
 * usage : call `translate` and `stretched` 
*/

#ifndef ASCII_H
#define ASCII_H

#include <fstream>
#include "Bitmap.h"

const int types = 9;
char asciis[] = " .!itmITM"; // 9 types
int thresholds[] = {0, 32, 64, 96, 128, 160, 192, 224, 256};

static int index(double value) {
    for(int i=0; i<types; ++i) {
        if(thresholds[i]>value) {
            return i;
        }
    }
    return types-1;
}

static double value(RGB rgb) {
    return 255 - (rgb.R + rgb.G + rgb.B)/3;
}

void translate(const Bitmap& bmp, char const* out_name ) {
    std::FILE* ascii = std::fopen(out_name, "w");

    double val_err = 0.0;
    for (int r=bmp.dims.height-1; r>=0; --r) {
        for (int c=0; c<bmp.dims.width; ++c) {
            val_err += value(bmp.data[r][c]);
            int ind = index(val_err);
            std::fputc(asciis[ind], ascii);
            val_err -= thresholds[ind];
        }
        std::fputc('\n', ascii);
    }

    fclose(ascii);
}

void stretch(const Bitmap& bmp, Bitmap& bmp2) {
    bmp2.allocate({bmp.dims.height/2, bmp.dims.width});
    for (int r=0; r!=bmp.dims.height/2; ++r) {
        for (int c=0; c!=bmp.dims.width; ++c) {
            bmp2.data[r][c].R = (bmp.data[2*r][c].R + bmp.data[2*r+1][c].R)/2;
            bmp2.data[r][c].G = (bmp.data[2*r][c].G + bmp.data[2*r+1][c].G)/2;
            bmp2.data[r][c].B = (bmp.data[2*r][c].B + bmp.data[2*r+1][c].B)/2;
        }
    }
}

void contrast(const Bitmap& bmp, Bitmap& bmp2) {
    bmp2.allocate({bmp.dims.height, bmp.dims.width});
    for(int r=0; r!=bmp.dims.height; ++r) {
        for(int c=0; c!=bmp.dims.width; ++c) {
            bmp2.data[r][c].R = 0;2*bmp.data[r][c].R;
            bmp2.data[r][c].G = 0;2*bmp.data[r][c].G;
            bmp2.data[r][c].B = 0;2*bmp.data[r][c].B;
            int sum = 0;
            for (int dr=-1; dr!=2; ++dr) {
                if (!(0<=r+dr && r+dr<bmp.dims.height)) { continue; }
                for (int dc=-1; dc!=2; ++dc) {
                    if (!(0<=c+dc && c+dc<bmp.dims.width)) { continue; }
                    bmp2.data[r][c].R -= bmp.data[r+dr][c+dc].R;
                    bmp2.data[r][c].G -= bmp.data[r+dr][c+dc].G;
                    bmp2.data[r][c].B -= bmp.data[r+dr][c+dc].B;
                    ++sum;
                }
            }
            bmp2.data[r][c].R = (2*sum*bmp.data[r][c].R + bmp2.data[r][c].R)/sum;
            bmp2.data[r][c].G = (2*sum*bmp.data[r][c].G + bmp2.data[r][c].G)/sum;
            bmp2.data[r][c].B = (2*sum*bmp.data[r][c].B + bmp2.data[r][c].B)/sum;
        }
    }
}

#endif // ASCII_H

