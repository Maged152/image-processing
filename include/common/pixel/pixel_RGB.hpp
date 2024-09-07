#pragma once
#include "common/pixel/pixel_common.hpp"

namespace qlm
{
    // Specialization for RGB format
    template<pixel_t T>
    class Pixel<ImageFormat::RGB, T>
    {
    public:
        T r, g, b, a;
    private:
        const T min_value = std::numeric_limits<T>::lowest();
        const T max_value = std::numeric_limits<T>::max();
    public:
        Pixel() : r(0), g(0), b(0), a(std::numeric_limits<T>::max()) {}
        Pixel(T red, T green, T blue) : r(red), g(green), b(blue), a(std::numeric_limits<T>::max()) {}
        Pixel(T v) : r(v), g(v), b(v), a(std::numeric_limits<T>::max()) {}
        Pixel(T red, T green, T blue, T alpha) : r(red), g(green), b(blue), a(alpha) {}
        // Copy constructor
        Pixel(const Pixel& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}
        // Move constructor
        Pixel(Pixel&& other) noexcept : r(std::move(other.r)), g(std::move(other.g)), b(std::move(other.b)), a(std::move(other.a)) {}
    public:
        // Assignment operator
        Pixel& operator=(const Pixel& other)
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
        // Move operator
        Pixel& operator=(Pixel&& other) noexcept
        {
            if (this != &other)
            {
                r = std::move(other.r);
                g = std::move(other.g);
                b = std::move(other.b);
                a = std::move(other.a);
            }
            return *this;
        }
        // Addition operator
        Pixel operator+(const Pixel& other) const
        {
            Pixel result;
            result.r = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(r + other.r, min_value, max_value));
            result.g = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(g + other.g, min_value, max_value));
            result.b = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(b + other.b, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a + other.a, min_value, max_value));
            return result;
        }
        // Subtraction operator
        Pixel operator-(const Pixel& other) const
        {
            Pixel result;
            result.r = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(r - other.r, min_value, max_value));
            result.g = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(g - other.g, min_value, max_value));
            result.b = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(b - other.b, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a - other.a, min_value, max_value));
            return result;
        }
        // Multiplication operator
        Pixel operator*(const Pixel& other) const
        {
            Pixel result;
            result.r = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(r * other.r, min_value, max_value));
            result.g = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(g * other.g, min_value, max_value));
            result.b = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(b * other.b, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a * other.a, min_value, max_value));
            return result;
        }
        // div operator
        Pixel operator/(const Pixel& other) const
        {
            Pixel result;
            result.r = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(r / other.r, min_value, max_value));
            result.g = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(g / other.g, min_value, max_value));
            result.b = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(b / other.b, min_value, max_value));
            result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a / other.a, min_value, max_value));
            return result;
        }
        // < comparison operator
        bool operator<(const Pixel& other) const
        {
            if (r < other.r && g < other.g && b < other.b && a < other.a)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // <= comparison operator
        bool operator<=(const Pixel& other) const
        {
            if (r <= other.r && g <= other.g && b <= other.b && a <= other.a)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // == comparison operator
        bool operator ==(const Pixel& other) const
        {
            if (r == other.r && g == other.g && b == other.b && a == other.a)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // Cast operator
        template<qlm::pixel_t T2>
        operator Pixel<qlm::ImageFormat::RGB, T2>() const
        {
            Pixel<qlm::ImageFormat::RGB, T2> res;

            T2 min_val = std::numeric_limits<T2>::lowest();
            T2 max_val = std::numeric_limits<T2>::max();

            res.r = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(r, min_val, max_val));
            res.g = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(g, min_val, max_val));
            res.b = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(b, min_val, max_val));
            res.a = static_cast<T2>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

            return res;
        }
    public:
        void Set(T red, T green, T blue, T alpha = std::numeric_limits<T>::max())
        {
            r = red;
            g = green;
            b = blue;
            a = alpha;
        }

        void Set(T v, T alpha = std::numeric_limits<T>::max())
        {
            r = v;
            g = v;
            b = v;
            a = alpha;
        }

        template<arithmetic_t T2>
        void MAC(const Pixel& other, const T2 coeff)
        {
            r = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(r + other.r * coeff, min_value, max_value));
            g = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(g + other.g * coeff, min_value, max_value));
            b = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(b + other.b * coeff, min_value, max_value));
            a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a + other.a * coeff, min_value, max_value));
        }

        uint64_t SquaredEuclideanDistance(const Pixel& other)
        {
            int64_t diff_r = (int64_t)r - (int64_t)other.r;
            int64_t diff_g = (int64_t)g - (int64_t)other.g;
            int64_t diff_b = (int64_t)b - (int64_t)other.b;

            return static_cast<uint64_t>(diff_r * diff_r + diff_g * diff_g + diff_b * diff_b);

        }
    };
}