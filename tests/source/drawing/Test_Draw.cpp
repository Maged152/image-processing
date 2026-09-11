#include "test_common.hpp"

TEST(Test_shakhbat_cv, Draw)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Drawing/";

	using RGB8 = qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>;

	// blank canvas (same as examples/run.cpp)
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> canvas;
	canvas.Create(512, 512, RGB8(0, 0, 0));

	const RGB8 red(255, 0, 0);
	const RGB8 green(0, 255, 0);
	const RGB8 blue(0, 0, 255);
	const RGB8 yellow(255, 255, 0);
	const RGB8 cyan(0, 255, 255);
	const RGB8 magenta(255, 0, 255);
	const RGB8 orange(255, 165, 0);
	const RGB8 white(255, 255, 255);

	// 1. DrawCluster (paints on a fresh canvas, so apply it first):
	//    two solid 60x60 blocks that survive under the later drawings
	std::vector<qlm::Cluster<qlm::ImageFormat::RGB, uint8_t>> clusters(2);
	clusters[0].color = orange;
	clusters[1].color = white;
	for (int y = 0; y < 60; y++)
	{
		for (int x = 0; x < 60; x++)
		{
			clusters[0].pixels.push_back(qlm::Point<int>{30 + x, 30 + y});
			clusters[1].pixels.push_back(qlm::Point<int>{422 + x, 30 + y});
		}
	}

	// do the operations
	t.Start();
	auto out = qlm::DrawCluster(canvas, clusters);

	// 2. filled circle (red): center (150, 360), radius 70
	out = qlm::DrawCircle(out, qlm::Circle<int>{qlm::Point<int>{150, 360}, 70.0f}, red, -1);

	// 3. circle outline (green, thick): center (360, 360), radius 70
	out = qlm::DrawCircle(out, qlm::Circle<int>{qlm::Point<int>{360, 360}, 70.0f}, green, 6);

	// 4. rectangle outline (blue): spans most of the canvas
	out = qlm::DrawRectangle(out, qlm::Rectangle<int>{qlm::Point<int>{10, 10}, 491, 491}, blue);

	// 5. plain line (yellow diagonal across the whole canvas)
	out = qlm::DrawLine(out, qlm::Line{0, 511, 511, 0}, yellow);

	// 6. polar line (cyan, 45 degrees through the center region)
	out = qlm::DrawLine(out, qlm::LinePolar{300.0f, 0.7853982f}, cyan);

	// 7. arrowed line (magenta, horizontal with a large head)
	out = qlm::DrawArrowedLine(out, qlm::Line{106, 256, 406, 256}, magenta, 0.2);
	t.End();

	test::PrintTime(t);

	// reread output image (lossless: png round-trip matches the png reference)
	out = test::ReReadImage(out, "png");

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.png");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}
