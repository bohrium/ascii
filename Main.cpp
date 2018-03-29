/* author: samtenka
 * create: 2018-03-29 
 * change: 2018-03-29 
 * descrp: convert a given bitmap to an ASCII Art textfile
 * usage : compile (if needed) with `g++ std=c++11 Main.cpp -o main`,
           then run via `./main images/face.bmp out/ascii.txt` 
 */

#include "Bitmap.h"
#include "Ascii.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) { std::cout << "Argument Error!"; return 0; }
    init_thresholds("alpha.bmp", 8);

    Bitmap BMP, BMP2, BMP3;
    BMP.read_from(argv[1]);
    contrast(BMP, BMP2, 1.0, 2.0);
    //contrast(BMP, BMP2, 1.0, 0.5);
    BMP2.write_to("images/_contrasted.bmp");
    stretch(BMP2, BMP3);
    BMP3.write_to("images/_stretched.bmp");
    translate(BMP3, argv[2]);
    return 0;
}
