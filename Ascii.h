/* author: samtenka
 * create: 2018-03-29 
 * change: 2018-03-29 
 * descrp: interfaces for ASCII Art translation, building on Bitmap 
 * usage : call `translate` and `stretched` 
*/

#ifndef ASCII_H
#define ASCII_H

#include "Bitmap.h"
#include <fstream>
#include <algorithm>
#include <iostream>

//const int types = 9;
//char asciis[] = " .!itmITM"; // 9 types
//int thresholds[] = {0, 32, 64, 96, 128, 160, 192, 224, 256};

const int types = 95;
char asciis[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; 
int thresholds[95];

void init_thresholds(char const* in_name, int spacing) {
    Bitmap alpha;
    alpha.read_from(in_name);
    for (int i=0; i!=types; ++i) {
        thresholds[i]=0; 
    } 
    for (int c=0; c!=alpha.dims.width; ++c) {
        int i = c/spacing;
        for (int r=0; r!=alpha.dims.height; ++r) {
            thresholds[i] += 255 - alpha.data[r][c].R;
        }
    } 

    std::sort(asciis, asciis+types, [](int a, int b) {
        return thresholds[a-' '] < thresholds[b-' '];
    });

    int max = 0;
    for (int i=0; i!=types; ++i) {
        max = max < thresholds[i] ? thresholds[i] : max; 
    } 
    for (int i=0; i!=types; ++i) {
        thresholds[i] = (255 * thresholds[i])/max; 
    } 

    for (int i=0; i!=types; ++i) {
        std::cout << asciis[i] << "\t";
    } std::cout << std::endl; 
    for (int i=0; i!=types; ++i) {
        std::cout << thresholds[asciis[i]-' '] << "\t";
    } 

}

static int index(double value) {
    for(int i=0; i<types; ++i) {
        if(thresholds[asciis[i]-' ']>value) {
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
            val_err -= thresholds[asciis[ind]-' '];
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

void contrast(const Bitmap& bmp, Bitmap& bmp2, float a, float b) {
    bmp2.allocate({bmp.dims.height, bmp.dims.width});
    for(int r=0; r!=bmp.dims.height; ++r) {
        for(int c=0; c!=bmp.dims.width; ++c) {
            bmp2.data[r][c].R = 0;
            bmp2.data[r][c].G = 0;
            bmp2.data[r][c].B = 0;
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
            bmp2.data[r][c].R = a*(bmp.data[r][c].R-255/2.0) + b*(bmp.data[r][c].R+bmp2.data[r][c].R/sum)/2 + 255/2.0;
            bmp2.data[r][c].G = a*(bmp.data[r][c].G-255/2.0) + b*(bmp.data[r][c].G+bmp2.data[r][c].G/sum)/2 + 255/2.0;
            bmp2.data[r][c].B = a*(bmp.data[r][c].B-255/2.0) + b*(bmp.data[r][c].B+bmp2.data[r][c].B/sum)/2 + 255/2.0;

            bmp2.data[r][c].R = bmp2.data[r][c].R < 0 ? 0 : bmp2.data[r][c].R > 255 ? 255 : bmp2.data[r][c].R;
            bmp2.data[r][c].G = bmp2.data[r][c].G < 0 ? 0 : bmp2.data[r][c].G > 255 ? 255 : bmp2.data[r][c].G;
            bmp2.data[r][c].B = bmp2.data[r][c].B < 0 ? 0 : bmp2.data[r][c].B > 255 ? 255 : bmp2.data[r][c].B;
        }
    }
}

#endif // ASCII_H

