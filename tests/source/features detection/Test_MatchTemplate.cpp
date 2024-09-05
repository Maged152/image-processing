#include "test_common.h"

TEST(Test_shakhbat_cv, MatchTemplate)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/MatchTemplate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	// load the template image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> templ;
	const bool load_temp = templ.LoadFromFile(folder_path + "template.jpg");
	EXPECT_EQ(load_temp, true);

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> mask{};
	mask.create(templ.width, templ.height, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> {1});

	t.start();
	auto out = qlm::MatchTemplate(in, templ, qlm::TemplateMatchFlag::SQDIFF, mask);
	t.end();


	// draw clusters
	auto [min_loc, max_loc] = qlm::MinMaxLoc(out);

	qlm::Pixel <qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0 };
	qlm::Rectangle rec{ {0, 0}, templ.width, templ.height };

	for (auto& p : min_loc)
	{
		rec.top_left = p;
		in = qlm::DrawRectangle(in, rec, green);
	}

	test::PrintTime(t);

	// reread output image
	in = test::ReReadImage(in);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(in, ref);
}