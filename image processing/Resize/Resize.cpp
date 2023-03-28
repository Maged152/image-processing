#include "Resize.h"
#include "../Scale/Scale.h"


void qlm::Resize(const sf::Image& in, sf::Image& out, const unsigned int width, const unsigned int height, qlm::ScaleMethod method)
{
	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;
	// calculate the scale factors
	float scale_x = static_cast<float>(width) / img_width;
	float scale_y = static_cast<float>(height) / img_height;

	qlm::Scale(in, out, method, scale_x, scale_y);
}
