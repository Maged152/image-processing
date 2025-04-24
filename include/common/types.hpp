#pragma once

#include "PixelImage.hpp"
#include "types/TransformationMatrix.hpp"
#include "types/Kernel.hpp"
#include "types/Interpolation.hpp"
#include "types/matrix.hpp"
#include "types/enums.hpp"
#include "types/KDTree.hpp"
#include "types/HuffmanTree.hpp"
#include <vector>
#include <limits>
#include <array>
#include <unordered_map>

namespace qlm
{
	struct Line
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};
	// line in the polar form
	struct LinePolar
	{
		float radius;
		float angle;
	};


	template<class T = int>
	class Point
	{
	public:
		T x;
		T y;
	public:
		Point() : x(0), y(0)
		{}
		Point(T x, T y) : x(x), y(y)
		{}
	public:
		// addition operator
		Point<T> operator+(const Point<T>& other) const
		{
			return Point<T>(x + other.x, y + other.y);
		}

		// assignment operator
		Point<T>& operator=(const Point<T>& other)
		{
			if (this != &other)
			{
				x = other.x;
				y = other.y;
			}
			return *this;
		}

		// equality operator
		bool operator==(const Point<T>& other) const
		{
			return (x == other.x) && (y == other.y);
		}

		// subtraction operator
		Point<T> operator-(const Point<T>& other) const
		{
			return Point<T>(x - other.x, y - other.y);
		}
	};

	struct Rectangle
	{
		Point<int> top_left;
		int width;
		int height;
	};

	template<class T>
	struct Circle
	{
		Point<T> center;
		float radius;
	};

	template<class T = int>
	struct KeyPoint {
		// Member variables
		Point<T> point;
		float scale;
		float angle;
		float response;
		int level;
		int class_id;

		// Constructor
		KeyPoint(Point<T> _pt, float _response = 0, float _scale = 0, float _angle = -1, int _level = 0, int _class_id = -1)
			: point(_pt), scale(_scale), angle(_angle), response(_response), level(_level), class_id(_class_id)
		{}
	};

	template<pixel_t mag = uint8_t, pixel_t drv = int16_t>
	struct SobelDerivatives
	{
	public:
		Image<ImageFormat::GRAY, drv> sobel_x;
		Image<ImageFormat::GRAY, drv> sobel_y;
		Image<ImageFormat::GRAY, mag> magnitude;
		float* angle;
	public:
		SobelDerivatives() : angle(nullptr), sobel_x(), sobel_y(), magnitude()
		{}
		SobelDerivatives(int width, int height) : sobel_x(width, height), sobel_y(width, height), magnitude(width, height)
		{
			angle = new float[width * height];
		}
		// Copy constructor
		SobelDerivatives(const SobelDerivatives& other): sobel_x(other.sobel_x), sobel_y(other.sobel_y), magnitude(other.magnitude), angle(nullptr)
		{
			if (other.angle != nullptr) 
			{
				angle = new float[other.sobel_x.get_width() * other.sobel_x.get_height()];
				std::copy(other.angle, other.angle + other.sobel_x.width * other.sobel_x.height, angle);
			}
		}
		// Move constructor
		SobelDerivatives(SobelDerivatives&& other) noexcept : sobel_x(std::move(other.sobel_x)), sobel_y(std::move(other.sobel_y)), magnitude(std::move(other.magnitude)), angle(other.angle)
		{
			other.angle = nullptr;
		}
		// Copy assignment operator
		SobelDerivatives& operator=(const SobelDerivatives& other)
		{
			if (this != &other) 
			{
				sobel_x = other.sobel_x;
				sobel_y = other.sobel_y;
				magnitude = other.magnitude;

				delete[] angle;
				angle = nullptr;
				if (other.angle != nullptr)
				{
					angle = new float[other.sobel_x.get_width() * other.sobel_x.get_height()];
					std::copy(other.angle, other.angle + other.sobel_x.width * other.sobel_x.height, angle);
				}
			}
			return *this;
		}
		// Move assignment operator
		SobelDerivatives& operator=(SobelDerivatives&& other)
		{
			if (this != &other) 
			{
				sobel_x = std::move(other.sobel_x);
				sobel_y = std::move(other.sobel_y);
				magnitude = std::move(other.magnitude);

				delete[] angle;
				angle = other.angle;
				other.angle = nullptr;
			}
			return *this;
		}
		// Destructor
		~SobelDerivatives() 
		{
			delete[] angle;
		}
	};

	template<ImageFormat frmt, pixel_t T>
	struct Cluster
	{
		std::vector<Point<int>> pixels;
		Pixel<frmt, T> color;
	};

	struct DBSCANResult
    {
        Image<ImageFormat::GRAY, int> labels;
        int num_clusters;
        bool noise_exists;
    };

	template<ImageFormat frmt, typename T>
	struct Histogram_t 
	{
		static constexpr int num_channels = (frmt == ImageFormat::GRAY) ? 1 : 3;
		// Dynamically calculate the number of elements based on T
    	static constexpr int tot_elements = static_cast<int>(std::numeric_limits<T>::max() - std::numeric_limits<T>::lowest()) + 1;

		// Histogram data: dynamically allocated based on channel count
		std::array<std::vector<int>, num_channels> hist;

		// Constructor to initialize the histogram arrays
		Histogram_t()
		{
			for (auto& channel_hist : hist) 
			{
				channel_hist.resize(tot_elements, 0);  // Allocate and initialize to zero
			}
		}

		// Get the cumulative sum of the histogram
		Histogram_t<frmt, T> CumulativeHistogram() const
		{
			Histogram_t<frmt, T> cumsum;

			for (int c = 0; c < num_channels; ++c)
			{
				cumsum.hist[c][0] = hist[c][0];
				for (int i = 1; i < tot_elements; ++i)
				{
					cumsum.hist[c][i] = cumsum.hist[c][i - 1] + hist[c][i];
				}
			}

			return cumsum;
		}

		 // Reset the histogram data
		void Reset()
		{
			for (auto& channel_hist : hist)
			{
				std::fill(channel_hist.begin(), channel_hist.end(), 0);
			}
		}
	};

	template<ImageFormat frmt, pixel_t T>
	struct Pyramid
	{
		std::vector<Image<frmt, T>> layers;
		int num_layers;
		float scale;
		int filter_size;
		float sigma;

		Pyramid(const int num_layers, const float scale, const int filter_size,const float sigma) :
			num_layers(num_layers), scale(scale), filter_size(filter_size), sigma(sigma), layers(num_layers)
			{}
	};

	template<ImageFormat frmt, pixel_t T>
    struct Huffman_t
    {
		static constexpr int num_channels = (frmt == ImageFormat::GRAY) ? 1 : 3;

        std::unordered_map<T, std::string> table[num_channels];
        std::string code[num_channels];
    };
}