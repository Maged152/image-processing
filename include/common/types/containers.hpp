#pragma once

#include "PixelImage.hpp"

namespace qlm
{
    struct Size
    {
        int width;
        int height;
    };
    
    /**********************************************************/
    struct Line
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};

    /**********************************************************/
    struct LinePolar
	{
		float radius;
		float angle;
	};

    /**********************************************************/
    template<class T = int>
	class Point
	{
	public:
		T x;
		T y;
	public:
		constexpr Point() : x(0), y(0)
		{}
		constexpr Point(T x, T y) : x(x), y(y)
		{}
	public:
		// addition operator
		Point<T> operator+(const Point<T>& other) const
		{
			return Point<T>(x + other.x, y + other.y);
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

		Point<T> operator/(const float scalar) const
		{
			return Point<T>(x / scalar, y / scalar);
		}

		Point<T> operator*(const float scalar) const
		{
			return Point<T>(x * scalar, y * scalar);
		}
	};

    /**********************************************************/
    struct Rectangle
	{
		Point<int> top_left;
		int width;
		int height;
	};

    /**********************************************************/
    template<class T>
	struct Circle
	{
		Point<T> center;
		float radius;
	};

    /**********************************************************/
    template<ImageFormat frmt, pixel_t T>
	struct Cluster
	{
		std::vector<Point<int>> pixels;
		Pixel<frmt, T> color;
	};
    
    /**********************************************************/
    template<class T = int>
	struct KeyPoint 
	{
		Point<T> point;
		float scale;
		float angle;
		float response;
		float error;
		KPStatusFlag status;
		int level;
		int class_id;

		// Constructor
		KeyPoint(Point<T> _pt, float _response = 0, float _scale = 0, float _angle = -1, int _level = 0, int _class_id = -1, float _error = -1, KPStatusFlag _status = KPStatusFlag::TRACKED)
			: point(_pt), scale(_scale), angle(_angle), response(_response), error(_error), status(_status), level(_level), class_id(_class_id)
		{}

		KeyPoint()
		{}

		// cast operator
		template<typename T2>
        operator KeyPoint<T2>() const
		{
			KeyPoint<T2> result {Point<T2>{(T2)point.x, (T2)point.y}, response, scale, angle, level, class_id, error, status};

			return result;
		}
	};

    /**********************************************************/
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
}