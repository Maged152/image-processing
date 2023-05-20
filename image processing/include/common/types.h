#pragma once
#include "image.h"

/*************************************************************************/
namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	};

	enum class Border
	{
		BORDER_CONSTANT,
		BORDER_REPLICATE,
		BORDER_REFLECT,
	};

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
		Point operator+(const Point& other) const
		{
			return Point(x + other.x, y + other.y);
		}

		// assignment operator
		Point& operator=(const Point& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}

		// equality operator
		bool operator==(const Point& other) const
		{
			return (x == other.x) && (y == other.y);
		}

		// subtraction operator
		Point operator-(const Point& other) const
		{
			return Point(x - other.x, y - other.y);
		}
	};

	class Kernel
	{
	private:
		float* data;
	public:
		const unsigned int width;
		const unsigned int height;
		// Constructor
		Kernel(unsigned int M, unsigned int N) :width(M), height(N)
		{
			data = new float[height * width];
		}
		// Move Constructor
		Kernel(Kernel&& other) noexcept : width(other.width), height(other.height), data(other.data)
		{
			other.data = nullptr;
		}
		// Copy Constructor
		Kernel(const Kernel& other) : width(other.width), height(other.height)
		{
			data = new float[height * width];
			std::memcpy(data, other.data, height * width * sizeof(float));
		}
		// Move Assignment Operator
		Kernel& operator=(Kernel&& other) noexcept
		{
			if (this != &other)
			{
				const_cast<unsigned int&>(width) = other.width;
				const_cast<unsigned int&>(height) = other.height;
				data = other.data;
				other.data = nullptr;
			}
			return *this;
		}
		// Copy Assignment Operator
		Kernel& operator=(const Kernel& other)
		{
			if (this != &other)
			{
				const_cast<unsigned int&>(width) = other.width;
				const_cast<unsigned int&>(height) = other.height;
				if (data != nullptr)
					delete[] data;
				data = new float[height * width];
				std::memcpy(data, other.data, height * width * sizeof(float));
			}
			return *this;
		}
		// Destructor
		~Kernel()
		{
			if (data != nullptr)
				delete[] data;
		}
	public:
		float Get(int x, int y) const
		{
			return data[y * width + x];
		}

		void Set(int x, int y, float value)
		{
			data[y * width + x] = value;
		}
	};

	class Kernel1D : private Kernel
	{
	public:
		const unsigned int length;
		// Constructor
		Kernel1D(unsigned int length): Kernel(1u, length), length(length)
		{}
		// Copy Constructor
		Kernel1D(const Kernel1D& other) : Kernel(other), length(other.length)
		{
		}
		// Move Constructor
		Kernel1D(Kernel1D&& other) noexcept: Kernel(std::move(other)), length(other.length)
		{
		}
		// Copy Assignment Operator
		Kernel1D& operator=(const Kernel1D& other)
		{
			if (this != &other)
			{
				Kernel::operator=(other);
				const_cast<unsigned int&>(length) = other.length;
			}
			return *this;
		}
		// Move Assignment Operator
		Kernel1D& operator=(Kernel1D&& other) noexcept
		{
			if (this != &other)
			{
				Kernel::operator=(std::move(other));
				const_cast<unsigned int&>(length) = other.length;
			}
			return *this;
		}

	public:

		float Get(int idx)  const
		{
			return Kernel::Get(idx, 0);
		}

		void Set(int idx, float value) 
		{
			Kernel::Set(idx, 0, value);
		}
	};

	class SepKernel
	{
	public:
		Kernel1D x_ker;
		Kernel1D y_ker;
	public:
		SepKernel(unsigned int len) : x_ker(len), y_ker(len)
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
		SobelDerivatives(unsigned int width, unsigned int height) : sobel_x(width, height), sobel_y(width, height), magnitude(width, height)
		{
			angle = new float[width * height];
		}
		// Copy constructor
		SobelDerivatives(const SobelDerivatives& other): sobel_x(other.sobel_x), sobel_y(other.sobel_y), magnitude(other.magnitude), angle(nullptr)
		{
			if (other.angle != nullptr) 
			{
				angle = new float[other.sobel_x.get_width() * other.sobel_x.get_height()];
				std::copy(other.angle, other.angle + other.sobel_x.Width() * other.sobel_x.Height(), angle);
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
					std::copy(other.angle, other.angle + other.sobel_x.Width() * other.sobel_x.Height(), angle);
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
}