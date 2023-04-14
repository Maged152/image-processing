#pragma once

#include <limits>
#include <algorithm>
#include <concepts>

namespace qlm
{
    // image formates
    enum class ImageFormat
    {
        GRAY,
        RGB,
        YCrCb,
        HSV,
        HLS,
    };

    //Supported pixel types
    template <class T>
    concept pixel_type = std::same_as<T, uint8_t> ||
                         std::same_as<T, int16_t> ||
                         std::floating_point<T>;

    // Forward declaration of Pixel class
    template<ImageFormat frmt, pixel_type T>
    class Pixel;

}

// Specialization for GRAY format
template<qlm::pixel_type T>
class qlm::Pixel<qlm::ImageFormat::GRAY, T>
{
public:
    T v, a;
public:
    Pixel() : v(0), a(std::numeric_limits<T>::max()) {}
    Pixel(T gray) : v(gray), a(std::numeric_limits<T>::max()) {}
    Pixel(T gray, T alpha) : v(gray), a(alpha) {}
    // Copy constructor
    Pixel(const Pixel& other) : v(other.v), a(other.a) {}
    // Move constructor
    Pixel(Pixel&& other) noexcept : v(std::move(other.v)), a(std::move(other.a)) {}
public:
    // Assignment operator
    Pixel& operator=(const Pixel& other)
    {
        if (this != &other)
        {
            v = other.v;
            a = other.a;
        }
        return *this;
    }
    // Move operator
    Pixel& operator=(Pixel&& other) noexcept
    {
        if (this != &other)
        {
            v = std::move(other.v);
            a = std::move(other.a);
        }
        return *this;
    }
    // Addition operator
    Pixel operator+(const Pixel& other) const
    {
        Pixel result;
        result.v = std::clamp(v + other.v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a + other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.v = std::clamp(v - other.v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a - other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.v = std::clamp(v * other.v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a * other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
public:
    void Set(T value,T alpha = std::numeric_limits<T>::max())
    {
        v = value;
        a = alpha;
    }

    void MAC(const Pixel& c, const float coeff)
    {
        v =  std::clamp(v + c.v * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        a =  std::clamp(a + c.a * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }
};

// Specialization for RGB format
template<qlm::pixel_type T>
class qlm::Pixel<qlm::ImageFormat::RGB, T>
{
public:
    T r, g, b, a;
public:
    Pixel() : r(0), g(0), b(0), a(std::numeric_limits<T>::max()) {}
    Pixel(T red, T green, T blue) : r(red), g(green), b(blue), a(std::numeric_limits<T>::max()) {}
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
        result.r = std::clamp(r + other.r, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.g = std::clamp(g + other.g, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.b = std::clamp(b + other.b, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a + other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.r = std::clamp(r - other.r, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.g = std::clamp(g - other.g, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.b = std::clamp(b - other.b, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a - other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.r = std::clamp(r * other.r, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.g = std::clamp(g * other.g, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.b = std::clamp(b * other.b, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a * other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
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

    void MAC(const Pixel& c, const float coeff)
    {
        r = std::clamp(r + c.r * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        g = std::clamp(g + c.g * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        b = std::clamp(b + c.b * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        a = std::clamp(a + c.a * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }
};

// Specialization for YCrCb format
template<qlm::pixel_type T>
class qlm::Pixel<qlm::ImageFormat::YCrCb, T>
{
private:
    T y, cr, cb, a;
public:
    Pixel() : y(0), cr(0), cb(0), a(std::numeric_limits<T>::max()) {}
    Pixel(T luminance, T chromaR, T chromaB) : y(luminance), cr(chromaR), cb(chromaB), a(std::numeric_limits<T>::max()) {}
    Pixel(T luminance, T chromaR, T chromaB, T alpha) : y(luminance), cr(chromaR), cb(chromaB), a(alpha) {}
    // Copy constructor
    Pixel(const Pixel& other) : y(other.y), cr(other.cr), cb(other.cb), a(other.a) {}
    // Move constructor
    Pixel(Pixel&& other) noexcept : y(std::move(other.y)), cr(std::move(other.cr)), cb(std::move(other.cb)), a(std::move(other.a)) {}
public:
    // Assignment operator
    Pixel& operator=(const Pixel& other)
    {
        if (this != &other)
        {
            y = other.y;
            cr = other.cr;
            cb = other.cb;
            a = other.a;
        }
        return *this;
    }
    // Move operator
    Pixel& operator=(Pixel&& other) noexcept
    {
        if (this != &other)
        {
            y = std::move(other.y);
            cr = std::move(other.cr);
            cb = std::move(other.cb);
            a = std::move(other.a);
        }
        return *this;
    }
    // Addition operator
    Pixel operator+(const Pixel& other) const
    {
        Pixel result;
        // ranges may be changed
        result.y = std::clamp(y + other.y, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.cr = std::clamp(cr + other.cr, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.cb = std::clamp(cb + other.cb, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a + other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.y = std::clamp(y - other.y, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.cr = std::clamp(cr - other.cr, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.cb = std::clamp(cb - other.cb, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a - other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.y = std::clamp(y * other.y, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.cr = std::clamp(cr * other.cr, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.cb = std::clamp(cb * other.cb, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a * other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
public:
    void Set(T Y, T Cr, T Cb, T alpha = std::numeric_limits<T>::max())
    {
        y = Y;
        cr = Cr;
        cb = Cb;
        a = alpha;
    }

    void Set(T val, T alpha = std::numeric_limits<T>::max())
    {
        y = val;
        cr = val;
        cb = val;
        a = alpha;
    }

    void MAC(const Pixel& c, const float coeff)
    {
        y  = std::clamp(y  + c.y  * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        cr = std::clamp(cr + c.cr * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        cb = std::clamp(cb + c.cb * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        a  = std::clamp(a  + c.a  * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }
};

// Specialization for HSV format
template<qlm::pixel_type T>
class qlm::Pixel<qlm::ImageFormat::HSV, T>
{
private:
    T h, s, v, a;
public:
    Pixel() : h(0), s(0), v(0), a(std::numeric_limits<T>::max()) {}
    Pixel(T hue, T saturation, T value) : h(hue), s(saturation), v(value), a(std::numeric_limits<T>::max()) {}
    Pixel(T hue, T saturation, T value, T alpha) : h(hue), s(saturation), v(value), a(alpha) {}
    // Copy constructor
    Pixel(const Pixel& other) : h(other.h), s(other.s), v(other.v), a(other.a) {}
    // Move constructor
    Pixel(Pixel&& other) noexcept : h(std::move(other.h)), s(std::move(other.s)), v(std::move(other.v)), a(std::move(other.a)) {}
public:
    // Assignment operator
    Pixel& operator=(const Pixel& other)
    {
        if (this != &other)
        {
            h = other.h;
            s = other.s;
            v = other.v;
            a = other.a;
        }
        return *this;
    }
    // Move operator
    Pixel& operator=(Pixel&& other) noexcept
    {
        if (this != &other)
        {
            h = std::move(other.h);
            s = std::move(other.s);
            v = std::move(other.v);
            a = std::move(other.a);
        }
        return *this;
    }
    // Addition operator
    Pixel operator+(const Pixel& other) const
    {
        Pixel result;
        result.h = (h + other.h) % 360;
        result.s = std::clamp(s + other.s, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.v = std::clamp(v + other.v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a + other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.h = (h - other.h) % 360;
        result.s = std::clamp(s - other.s, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.v = std::clamp(v - other.v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a - other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.h = (h * other.h) % 360;
        result.s = std::clamp(s * other.s, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.v = std::clamp(v * other.v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a * other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
public:
    void Set(T hue, T sat, T val, T alpha = std::numeric_limits<T>::max())
    {
        h = hue;
        s = sat;
        v = val;
        a = alpha;
    }

    void Set(T val, T alpha = std::numeric_limits<T>::max())
    {
        h = val;
        s = val;
        v = val;
        a = alpha;
    }

    void MAC(const Pixel& c, const float coeff)
    {
        h = (h + c.h * coeff) % 360;
        s = std::clamp(s + c.s * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        v = std::clamp(v + c.v * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        a = std::clamp(a + c.a * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }
};

// Specialization for HLS format
template<qlm::pixel_type T>
class qlm::Pixel<qlm::ImageFormat::HLS, T>
{
private:
    T h, l, s, a;
public:
    Pixel() : h(0), l(0), s(0), a(std::numeric_limits<T>::max()) {}
    Pixel(T hue, T lightness, T saturation) : h(hue), l(lightness), s(saturation), a(std::numeric_limits<T>::max()) {}
    Pixel(T hue, T lightness, T saturation, T alpha) : h(hue), l(lightness), s(saturation), a(alpha) {}
    // Copy constructor
    Pixel(const Pixel& other) : h(other.h), l(other.l), s(other.s), a(other.a) {}
    // Move constructor
    Pixel(Pixel&& other) noexcept : h(std::move(other.h)), l(std::move(other.l)), s(std::move(other.s)), a(std::move(other.a)) {}
public:
    // Assignment operator
    Pixel& operator=(const Pixel& other)
    {
        if (this != &other)
        {
            h = other.h;
            l = other.l;
            s = other.s;
            a = other.a;
        }
        return *this;
    }
    // Move operator
    Pixel& operator=(Pixel&& other) noexcept
    {
        if (this != &other)
        {
            h = std::move(other.h);
            l = std::move(other.l);
            s = std::move(other.s);
            a = std::move(other.a);
        }
        return *this;
    }
    // Addition operator
    Pixel operator+(const Pixel& other) const
    {
        Pixel result;
        result.h = (h + other.h) % 360;
        result.s = std::clamp(s + other.s, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.l = std::clamp(l + other.l, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a + other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.h = (h - other.h) % 360;
        result.s = std::clamp(s - other.s, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.l = std::clamp(l - other.l, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a - other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.h = (h * other.h) % 360;
        result.s = std::clamp(s * other.s, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.l = std::clamp(l * other.l, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        result.a = std::clamp(a * other.a, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        return result;
    }
public:
    void Set(T hue, T lim, T sat, T alpha = std::numeric_limits<T>::max())
    {
        h = hue;
        s = sat;
        l = lim;
        a = alpha;
    }

    void Set(T val, T alpha = std::numeric_limits<T>::max())
    {
        h = val;
        s = val;
        l = val;
        a = alpha;
    }

    void MAC(const Pixel& c, const float coeff)
    {
        h = (h + c.h * coeff) % 360 ;
        s = std::clamp(s + c.s * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        l = std::clamp(l + c.l * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        a = std::clamp(a + c.a * coeff, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }
};
