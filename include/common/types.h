#pragma once
#include "image.h"
#include "WebCam.h"
#include "types/TransformationMatrix.h"
#include "types/Kernel.h"
#include "types/Interpolation.h"
#include "types/matrix.h"
#include <vector>
#include <limits>
#include <array>
namespace qlm
{
	enum class RotateFlag
	{
		ROTATE_90_CLOCKWISE,
		ROTATE_180,
		ROTATE_90_COUNTERCLOCKWISE,
	};

	enum class FlipFlag
	{
		FLIP_VERTICAL_AXIS,
		FLIP_HORIZONTAL_AXIS,
		FLIP_VERTICAL_HORIZONTAL_AXIS,
	};

	enum class KMeansInit
	{
		RANDOM,
		KMEANS_PLUS_PLUS
	};

	enum class TemplateMatchFlag
	{
		SQDIFF,
		SQDIFF_NORMED,
		CCORR,
		CCORR_NORMED
		//CCOEFF,
		//CCOEFF_NORMED
	};

	enum class OrderFlag
	{
		WIDTH_FIRST,
		HEIGHT_FIRST
	};

	enum class EnergyFlag
	{
		FORWARD,
		BACKWARD
	};
}

/***************************************************************************/

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
		size_t width;
		size_t height;
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
		SobelDerivatives(size_t width, size_t height) : sobel_x(width, height), sobel_y(width, height), magnitude(width, height)
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

	template<ImageFormat frmt, typename T>
	struct Histogram_t 
	{
		static constexpr int num_channels = (frmt == ImageFormat::GRAY) ? 1 : 3;
		static constexpr size_t tot_elements = std::numeric_limits<T>::max() - std::numeric_limits<T>::lowest() + 1;

		std::array<std::array<size_t, tot_elements>, num_channels> hist;

		// Constructor initializes the histograms to zero
		Histogram_t() 
		{
			for (auto& channel : hist) 
			{
				channel.fill(0);
			}
		}
	};
}