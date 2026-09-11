# Huffman Encoding

## Description
Builds a Huffman code for each channel of the input image. The function returns the symbol table (`pixel value -> bit string`) and the concatenated encoded bit stream of the image. The bit stream is built in ascending symbol order, so it is deterministic.

Note: the function is encode-only (no decoder is provided).

You can check the implementation [here](../../../../source/Huffman.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	struct Huffman_t
	{
		static constexpr int num_channels = (frmt == ImageFormat::GRAY) ? 1 : 3;

		std::unordered_map<T, std::string> table[num_channels];  // symbol -> code
		std::string code[num_channels];                          // encoded bit stream
	};

	template<ImageFormat frmt, pixel_t T>
	Huffman_t<frmt, T> HuffmanEncode(
		const Image<frmt, T>& in);
}
```

## Parameters

| Name  | Type   | Description       |
|-------|--------|-------------------|
| `in`  | `Image`| The input image.  |

## Return Value
The function returns a `Huffman_t<frmt, T>` with one symbol table and one encoded bit stream per channel.

Note: a channel that contains a single unique value produces a degenerate empty code for that symbol.

## Example

```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg";
    // load the image
    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the image\n";
        return -1;
    }

    t.Start();
    auto huffman = qlm::HuffmanEncode(in);
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";

    // bits in the encoded stream vs the raw size
    std::cout << "encoded bits = " << huffman.code[0].size() << "\n";
    std::cout << "raw bits     = " << 8 * in.width * in.height << "\n";
```