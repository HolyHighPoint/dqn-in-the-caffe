#ifndef ALE_H
#define ALE_H

#include <qlearning/dqn.h>

namespace qlearning {

class ALE {
private:
	// convert pixel_t to RGB 
	const std::array<int, 3> PixelToRGB(const pixel_t& pixel);
	
	// convert RGB to a grayscale
	uint8_t RGBToGrayscale(const std::array<int, 3>& rgb);
	uint8_t PixelToGrayscale(const pixel_t pixel);
	
public:
	// convert ALEScreen to framedata
	FDataP PreprocessScreen(const ALEScreen& tmp);
};

}

#endif
