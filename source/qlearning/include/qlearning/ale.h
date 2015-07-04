#ifndef ALE_H
#define ALE_H

#include <qlearning/dqn.h>

namespace dqn {

class ALE {
private:
	const std::array<int, 3> PixelToRGB(const pixel_t& pixel);
	uint8_t RGBToGrayscale(const std::array<int, 3>& rgb);
	uint8_t PixelToGrayscale(const pixel_t pixel);
public:
	FDataP PreprocessScreen(const ALEScreen& tmp);
	std::string DrawFrame(const FData& frame);
};

}

#endif
