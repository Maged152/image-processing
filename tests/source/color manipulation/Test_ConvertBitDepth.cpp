#include "test_common.hpp"

TEST(Test_shakhbat_cv, ConvertBitDepth)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/FloodFill/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	// do the operation
	// U8 to S16
	t.Start();
	auto out = qlm::ConvertBitDepth<qlm::ImageFormat::RGB, uint8_t, int16_t>(in);
	t.End();

	test::PrintTime(t);

	// S16 to U8
	auto out_u8 = qlm::ConvertBitDepth<qlm::ImageFormat::RGB, int16_t, uint8_t>(out);

	test::CompareImages(out_u8, in);
}

TEST(Test_shakhbat_cv, ConvertBitDepth_KnownValues)
{
	// pixel-level known values (GRAY)
	const qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> in0{ 0 };
	const qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> in255{ 255 };
	const qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> in128{ 128 };

	auto s16_0 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint8_t, int16_t>(in0);
	auto s16_255 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint8_t, int16_t>(in255);
	auto s16_128 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint8_t, int16_t>(in128);

	EXPECT_EQ(s16_0.v, -32768);
	EXPECT_EQ(s16_255.v, 32767);
	// 128 -> (128/255)*65535 = 128*257 = 32896 -> 32896 - 32768 = 128 (exact)
	EXPECT_EQ(s16_128.v, 128);

	// round-trip back to u8
	const auto back0 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, int16_t, uint8_t>(s16_0);
	const auto back255 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, int16_t, uint8_t>(s16_255);
	const auto back128 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, int16_t, uint8_t>(s16_128);
	EXPECT_EQ(back0.v, 0);
	EXPECT_EQ(back255.v, 255);
	EXPECT_EQ(back128.v, 128);

	// u16 -> u8 (the Magnitude viewing path)
	qlm::Image<qlm::ImageFormat::GRAY, uint16_t> in16(2, 1);
	in16.SetPixel(0, qlm::Pixel<qlm::ImageFormat::GRAY, uint16_t>(0));
	in16.SetPixel(1, qlm::Pixel<qlm::ImageFormat::GRAY, uint16_t>(65535));
	auto u8 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint16_t, uint8_t>(in16);
	EXPECT_EQ(u8.GetPixel(0).v, 0);
	EXPECT_EQ(u8.GetPixel(1).v, 255);
}