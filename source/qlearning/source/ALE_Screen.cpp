#include <qlearning/dqn.h>
#include <qlearning/ale.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>

namespace qlearning {

// convert ALEScreen to FDataP
FDataP ALE::PreprocessScreen(const ALEScreen& tmp) {

    // get pixels info in the screen
    const auto pixels = tmp.getArray();
    // compression ratio to fit screen in CSize x CSize
    const double ratiox = FWidth / double(CSize);
    const double ratioy = FHeight / double(CSize);
    // destination FDataP
    FDataP screen = boost::make_shared<FData>();

    for (int i = 0; i < CSize; i++) {
        for (int j = 0; j < CSize; j++) {
            
            // get corresponding pixel in ALEScreen
            const int firstx = int(std::floor(j * ratiox));
            const int lastx = int(std::floor((j + 1) * ratiox));
            const int firsty = int(std::floor(i * ratioy));
            const int lasty = int(std::floor((i + 1) * ratioy));

            // calculate color of every pixel of FDataP
            uint8_t color = 0.0;

            for (int x = firstx; x <= lastx; x++) {
                double pixelx = 1.0;

                if (x == firstx) {
                    pixelx = x + 1 - j * ratiox;
                } else if (x == lastx) {
                    pixelx = ratiox * (j + 1) - x;
                }

                for(int y = firsty; y <= lasty; y++) {
                    double pixely = 1.0;

                    if (y == firsty) {
                        pixely = y + 1 - i * ratioy;
                    } else if (y == lasty) {
                        pixely = ratioy * (i + 1) - y;
                    }
                    
                    // get grayscale
                    const uint8_t grayscale =
                        PixelToGrayscale(pixels[int(y * FWidth + x)]);

                    // and mul size as weight
                    color += (pixelx / ratiox) * (pixely / ratioy) * grayscale;
                }
            }

            // set color to the FDataP pixel
            (*screen)[i * CSize + j] = color;
        }
    }
    
    return screen;
}

}
