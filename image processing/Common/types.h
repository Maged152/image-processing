#pragma once
#include "Image.h"

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
	
	enum class ColorCvtCode
	{
		RGB2GRAY,
		GRAY2RGB,
		RGB2YCrCb,
		YCrCb2RGB,
		RGB2HSV,
		HSV2RGB,
		RGB2HLS,
		HLS2RGB,
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

	class Kernel1D : protected Kernel
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
		Kernel1D(Kernel1D&& other) : Kernel(std::move(other)), length(other.length)
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

	struct SobelDerivatives
	{
		sf::Image sobel_x;
		sf::Image sobel_y;
		sf::Image magnitude;
		float* angle;
		// TODO : constructor
	};
}