#include "shakhbat.h"

int main()
{
	qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.LoadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;


	float scale = 0.5f;
	float angle = 30.0f;
	const int dst_width = in.Width();
	const int dst_height = in.Height();

	const qlm::Point<int> center{ (int)in.Width() / 2, (int)in.Height() / 2 };

	qlm::TransformationMatrix mat = qlm::GetRotationMatrix(center, angle, scale);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;
	
	// do the operation
	t.start();
	auto out = qlm::WarpAffine(in, mat, dst_width, dst_height, qlm::InterpolationFlag::BILINEAR, border_mode);
	t.end();

	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Failed to write \n";
	}
}

