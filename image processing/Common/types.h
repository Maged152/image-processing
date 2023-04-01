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
	
		Kernel(unsigned int M, unsigned int N) :width(M), height(N)
		{
			data = new float[height * width];
		}
		~Kernel()
		{
			if (data != nullptr)
				delete[] data;
		}
	
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
		Kernel1D(unsigned int length): Kernel(1u, length), length(length)
		{}
	public:
		const unsigned int length;

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