#include "Resize.h"
#include "../Scale/Scale.h"


sf::Image qlm::Resize(const sf::Image& in, const unsigned int width, const unsigned int height, qlm::ScaleMethod method)
{
	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;
	// calculate the scale factors
	float scale_x = static_cast<float>(width) / img_width;
	float scale_y = static_cast<float>(height) / img_height;

	return std::move(qlm::Scale(in, method, scale_x, scale_y));
}
