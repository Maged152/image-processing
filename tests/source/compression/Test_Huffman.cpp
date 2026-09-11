#include "test_common.hpp"
#include <cmath>

TEST(Test_shakhbat_cv, Huffman)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// encode twice: the codes must be deterministic
	t.Start();
	const auto h1 = qlm::HuffmanEncode(in);
	const auto h2 = qlm::HuffmanEncode(in);
	t.End();
	test::PrintTime(t);

	for (int c = 0; c < 3; c++)
	{
		EXPECT_EQ(h1.code[c], h2.code[c]);
	}

	// per-channel properties: coverage, prefix-freedom, Kraft equality
	const auto hist = qlm::Histogram(in);
	for (int c = 0; c < 3; c++)
	{
		// the table must contain exactly the symbols present in the image
		int used = 0;
		for (int e = 0; e < hist.tot_elements; e++)
		{
			if (hist.hist[c][e] > 0) used++;
		}
		EXPECT_EQ(h1.table[c].size(), (size_t)used);

		double kraft = 0.0;
		for (const auto& [v1, c1] : h1.table[c])
		{
			// non-degenerate image: no empty codes
			EXPECT_NE(c1.size(), 0u);
			kraft += std::pow(0.5, (double)c1.size());

			// no code may be a prefix of another
			for (const auto& [v2, c2] : h1.table[c])
			{
				if (v1 == v2) continue;
				const bool prefix = (c2.size() >= c1.size()) && (c2.compare(0, c1.size(), c1) == 0);
				EXPECT_EQ(prefix, false);
			}
		}
		// a complete Huffman code satisfies Kraft with equality
		EXPECT_NEAR(kraft, 1.0, 1e-9);
	}

	// the encoding must actually compress a natural image
	const long long encoded_bits = (long long)h1.code[0].size() + (long long)h1.code[1].size() + (long long)h1.code[2].size();
	const long long raw_bits = 8LL * in.width * in.height * 3;
	EXPECT_LT(encoded_bits, raw_bits);
}

TEST(Test_shakhbat_cv, Huffman_ConstantImage)
{
	// degenerate case: a single symbol per channel
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
	in.Create(8, 8, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(42));

	const auto h = qlm::HuffmanEncode(in);

	EXPECT_EQ(h.table[0].size(), 1u);
	EXPECT_EQ(h.code[0].size(), 0u);
}