#pragma once

namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	};

	enum class BORDER
	{
		BORDER_CONSTANT,
		BORDER_REPLICATE,
		BORDER_REFLECT,
	};

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

	class Pixel
	{
	public:
		float r, g, b, a;
	public:
		Pixel(float value):r(value), g(value), b(value), a(value)
		{}

		Pixel():Pixel(0)
		{}

		Pixel(float r, float g, float b, float a = 255):r(r), g(g), b(b), a(a)
		{}
	public:
		void Set(float value)
		{
			r = value;
			g = value;
			b = value;
			a = value;
		}

		void Set(float red, float green, float blue,float alpha = 255)
		{
			r = red;
			g = blue;
			b = green;
			a = alpha;
		}

		void ToColor(sf::Color& c) const
		{
			c.r = std::clamp(static_cast<int>(std::round(r)), 0, 255);
			c.g = std::clamp(static_cast<int>(std::round(g)), 0, 255);
			c.b = std::clamp(static_cast<int>(std::round(b)), 0, 255);
			c.a = std::clamp(static_cast<int>(std::round(a)), 0, 255);

		}

		void MAC(const sf::Color& c, const float coeff)
		{
			r += static_cast<float>(c.r) * coeff;
			g += static_cast<float>(c.g) * coeff;
			b += static_cast<float>(c.b) * coeff;
			a += static_cast<float>(c.a) * coeff;
		}

		void MAC(const Pixel& c, const float coeff)
		{
			r += c.r * coeff;
			g += c.g * coeff;
			b += c.b * coeff;
			a += c.a * coeff;
		}

		Pixel& operator=(Pixel&& other) noexcept
		{
			if (this != &other)
			{
				r = std::exchange(other.r, 0);
				g = std::exchange(other.g, 0);
				b = std::exchange(other.b, 0);
				a = std::exchange(other.a, 0);
			}

			return *this;
		}

		Pixel& operator=(Pixel& other) noexcept
		{
			if (this != &other)
			{
				r = other.r;
				g = other.g;
				b = other.b;
				a = other.a;
			}

			return *this;
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
		Kernel(Kernel&& other) : width(other.width), height(other.height), data(other.data)
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
		Kernel& operator=(Kernel&& other) 
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
}