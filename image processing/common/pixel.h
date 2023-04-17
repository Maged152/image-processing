#pragma once

#include <limits>
#include <type_traits>
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
        HLS
    };

    // Supported pixel types
    template <class T>
    concept pixel_t = std::same_as<T, uint8_t> ||
                      std::same_as<T, int16_t> ||
                      std::floating_point<T>;
    // Arithmetic data type
    template<typename T>
    concept arithmetic_t = std::is_arithmetic_v<T>;
    // Next larger data type
    template <typename T>
    using wider_t = 
        typename std::conditional<std::is_same_v<T, std::uint8_t> , std::uint16_t,
        typename std::conditional<std::is_same_v<T, std::int8_t>  , std::int16_t,
        typename std::conditional<std::is_same_v<T, std::uint16_t>, std::uint32_t,
        typename std::conditional<std::is_same_v<T, std::int16_t> , std::int32_t,
        typename std::conditional<std::is_same_v<T, std::uint32_t>, std::uint64_t,
        typename std::conditional<std::is_same_v<T, std::int32_t> , std::int64_t,
        typename std::conditional<std::is_same_v<T, float>, double, double
        >::type
        >::type
        >::type
        >::type
        >::type
        >::type
    >::type;
    // Make a type signed
    template<typename T>
    using signed_t =
        typename std::conditional<std::is_integral<T>::value, std::make_signed<T> ,T>::type;
    // upgrade type
    template <pixel_t T, arithmetic_t T2>
    using cast_t =
        typename std::conditional<(std::is_floating_point_v<T2> || std::is_floating_point_v<T>), double,
        typename std::conditional<((sizeof(T) > sizeof(T2)) && (std::is_signed_v<T2> || std::is_signed_v<T>)), wider_t<signed_t<T>>,
        typename std::conditional<((sizeof(T) < sizeof(T2)) && (std::is_signed_v<T2> || std::is_signed_v<T>)), wider_t<signed_t<T2>>,
        typename std::conditional<(sizeof(T) > sizeof(T2)), T, T2
        >::type
        >::type
        >::type
    >::type;
    // Forward declaration of Pixel class
    template<ImageFormat frmt, pixel_t T>
    class Pixel;

}

// Specialization for GRAY format
template<qlm::pixel_t T>
class qlm::Pixel<qlm::ImageFormat::GRAY, T>
{
public:
    T v, a;
private:
    const T min_value = std::numeric_limits<T>::lowest();
    const T max_value = std::numeric_limits<T>::max();
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
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(v + other.v, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a + other.a, min_value, max_value));
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(v - other.r, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a - other.a, min_value, max_value));
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(v * other.r, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a * other.a, min_value, max_value));
        return result;
    }
    // Cast operator
    template<qlm::pixel_t T2>
    operator Pixel<qlm::ImageFormat::GRAY, T2>() const
    {
        Pixel<qlm::ImageFormat::GRAY, T2> res;

        T2 min_val = std::numeric_limits<T2>::lowest();
        T2 max_val = std::numeric_limits<T2>::max();

        res.v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(v, min_val, max_val));
        res.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

        return res;
    }
public:
    void Set(T value,T alpha = std::numeric_limits<T>::max())
    {
        v = value;
        a = alpha;
    }

    template<arithmetic_t T2>
    void MAC(const Pixel& other, const T2 coeff)
    {
        v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(v + other.v * coeff, min_value, max_value));
        a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a + other.a * coeff, min_value, max_value));
    }
};

// Specialization for RGB format
template<qlm::pixel_t T>
class qlm::Pixel<qlm::ImageFormat::RGB, T>
{
public:
    T r, g, b, a;
private:
    const T min_value = std::numeric_limits<T>::lowest();
    const T max_value = std::numeric_limits<T>::max();
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
    // Cast operator
    template<qlm::pixel_t T2>
    operator Pixel<qlm::ImageFormat::RGB, T2>() const
    {
        Pixel<qlm::ImageFormat::RGB, T2> res;
    
        T2 min_val = std::numeric_limits<T2>::lowest();
        T2 max_val = std::numeric_limits<T2>::max();

        res.r = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(r, min_val, max_val));
        res.g = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(g, min_val, max_val));
        res.b = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(b, min_val, max_val));
        res.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

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
};

// Specialization for YCrCb format
template<qlm::pixel_t T>
class qlm::Pixel<qlm::ImageFormat::YCrCb, T>
{
private:
    T y, cr, cb, a;
private:
    const T min_value = std::numeric_limits<T>::lowest();
    const T max_value = std::numeric_limits<T>::max();
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
        result.y  = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(y  + other.y , min_value, max_value));
        result.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cr + other.cr, min_value, max_value));
        result.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cb + other.cb, min_value, max_value));
        result.a  = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a  + other.a , min_value, max_value));
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.y  = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(y  - other.y , min_value, max_value));
        result.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cr - other.cr, min_value, max_value));
        result.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cb - other.cb, min_value, max_value));
        result.a  = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a  - other.a , min_value, max_value));
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.y  = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(y  * other.y , min_value, max_value));
        result.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cr * other.cr, min_value, max_value));
        result.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(cb * other.cb, min_value, max_value));
        result.a  = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a  * other.a , min_value, max_value));
        return result;
    }
    // Cast operator
    template<qlm::pixel_t T2>
    operator Pixel<qlm::ImageFormat::YCrCb, T2>() const
    {
        Pixel<qlm::ImageFormat::YCrCb, T2> res;

        T2 min_val = std::numeric_limits<T2>::lowest();
        T2 max_val = std::numeric_limits<T2>::max();

        res.y  = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(y , min_val, max_val));
        res.cr = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(cr, min_val, max_val));
        res.cb = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(cb, min_val, max_val));
        res.a  = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a , min_val, max_val));

        return res;
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

    template<arithmetic_t T2>
    void MAC(const Pixel& other, const T2 coeff)
    {
        y  = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(y  + other.y  * coeff, min_value, max_value));
        cr = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(cr + other.cr * coeff, min_value, max_value));
        cb = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(cb + other.cb * coeff, min_value, max_value));
        a  = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a  + other.a  * coeff, min_value, max_value));
    }
};

// Specialization for HSV format
template<qlm::pixel_t T>
class qlm::Pixel<qlm::ImageFormat::HSV, T>
{
private:
    T h, s, v, a;
private:
    const T min_value = std::numeric_limits<T>::lowest();
    const T max_value = std::numeric_limits<T>::max();
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
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s + other.s, min_value, max_value));
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(v + other.v, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a + other.a, min_value, max_value));
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.h = (h - other.h) % 360;
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s - other.s, min_value, max_value));
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(v - other.v, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a - other.a, min_value, max_value));
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.h = ((qlm::cast_t<T, T>)h * (qlm::cast_t<T, T>)other.h) % 360;
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s * other.s, min_value, max_value));
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(v * other.v, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a * other.a, min_value, max_value));
        return result;
    }
    // Cast operator
    template<qlm::pixel_t T2>
    operator Pixel<qlm::ImageFormat::HSV, T2>() const
    {
        Pixel<qlm::ImageFormat::HSV, T2> res;

        T2 min_val = std::numeric_limits<T2>::lowest();
        T2 max_val = std::numeric_limits<T2>::max();

        res.h = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(h, min_val, max_val));
        res.s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(s, min_val, max_val));
        res.v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(v, min_val, max_val));
        res.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

        return res;
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

    template<arithmetic_t T2>
    void MAC(const Pixel& other, const T2 coeff)
    {
        h = ((qlm::cast_t<T, T2>)h + (qlm::cast_t<T, T2>)other.h * (qlm::cast_t<T, T2>)coeff) % 360;
        s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(s + other.s * coeff, min_value, max_value));
        v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(v + other.v * coeff, min_value, max_value));
        a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a + other.a * coeff, min_value, max_value));
    }
};

// Specialization for HLS format
template<qlm::pixel_t T>
class qlm::Pixel<qlm::ImageFormat::HLS, T>
{
private:
    T h, l, s, a;
private:
    const T min_value = std::numeric_limits<T>::lowest();
    const T max_value = std::numeric_limits<T>::max();
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
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s + other.s, min_value, max_value));
        result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(l + other.l, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a + other.a, min_value, max_value));
        return result;
    }
    // Subtraction operator
    Pixel operator-(const Pixel& other) const
    {
        Pixel result;
        result.h = (h - other.h) % 360;
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s - other.s, min_value, max_value));
        result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(l - other.l, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a - other.a, min_value, max_value));
        return result;
    }
    // Multiplication operator
    Pixel operator*(const Pixel& other) const
    {
        Pixel result;
        result.h = ((qlm::cast_t < T, T>) h * (qlm::cast_t<T, T>)other.h) % 360;
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(s * other.s, min_value, max_value));
        result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(l * other.l, min_value, max_value));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T>>(a * other.a, min_value, max_value));
        return result;
    }
    // Cast operator
    template<qlm::pixel_t T2>
    operator Pixel<qlm::ImageFormat::HLS, T2>() const
    {
        Pixel<qlm::ImageFormat::HLS, T2> res;

        T2 min_val = std::numeric_limits<T2>::lowest();
        T2 max_val = std::numeric_limits<T2>::max();

        res.h = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(h, min_val, max_val));
        res.s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(s, min_val, max_val));
        res.l = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(l, min_val, max_val));
        res.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a, min_val, max_val));

        return res;
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

    template<arithmetic_t T2>
    void MAC(const Pixel& other, const T2 coeff)
    {
        h = ((qlm::cast_t<T, T2>)h + (qlm::cast_t<T, T2>)other.h * (qlm::cast_t<T, T2>)coeff) % 360;
        s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(s + other.s * coeff, min_value, max_value));
        l = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(l + other.l * coeff, min_value, max_value));
        a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(a + other.a * coeff, min_value, max_value));
    }
};


// pixels operations
// Pixel * num 
template<qlm::ImageFormat frmt, qlm::pixel_t T, qlm::arithmetic_t T2>
qlm::Pixel<frmt, T> operator*(const qlm::Pixel<frmt, T>& c, T2 num)
{
    qlm::Pixel<frmt, T> result;
    
    const T min_val = std::numeric_limits<T>::lowest();
    const T max_val = std::numeric_limits<T>::max();

    if constexpr (frmt == qlm::ImageFormat::GRAY)
    {
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.v * num, min_val, max_val));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
    }
    else if constexpr (frmt == qlm::ImageFormat::RGB)
    {
        result.r = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.r * num, min_val, max_val));
        result.g = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.g * num, min_val, max_val));
        result.b = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.b * num, min_val, max_val));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
    }
    else if constexpr (frmt == qlm::ImageFormat::HLS)
    {
        result.h = ((qlm::cast_t<T, T2>)c.h * (qlm::cast_t<T, T2>)num) % 360;
        result.l = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.l * num, min_val, max_val));
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.s * num, min_val, max_val));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
    }
    else
    {
        result.h = ((qlm::cast_t<T, T2>)c.h * (qlm::cast_t<T, T2>)num) % 360;
        result.v = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.v * num, min_val, max_val));
        result.s = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.s * num, min_val, max_val));
        result.a = static_cast<T>(std::clamp<qlm::cast_t<T, T2>>(c.a * num, min_val, max_val));
    }

    return result;
}

