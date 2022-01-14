#pragma once

#include "rs-graphics-core/maths.hpp"
#include "rs-format/format.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <numeric>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace RS::Graphics::Core {

    namespace Detail {

        template <typename T> constexpr bool dependent_false = false;

        template <typename Range>
        size_t hash_range(const Range& r) noexcept {
            size_t h = 0, hx = 0;
            for (auto& x: r) {
                using T = std::decay_t<decltype(x)>;
                hx = std::hash<T>()(x);
                h ^= hx + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }

    }

    template <typename T, int N>
    class Vector {

    public:

        static_assert(std::is_arithmetic_v<T>);
        static_assert(N >= 1);

        using scalar_type = T;
        using iterator = T*;
        using const_iterator = const T*;

        static constexpr int dim = N;

        constexpr Vector() noexcept: array_{} {}
        constexpr explicit Vector(T x) noexcept: array_{} { for (auto& y: *this) y = x; }
        template <typename... Args, typename U = T>
            constexpr Vector(T x, std::enable_if_t<Detail::SfinaeBoolean<U, sizeof...(Args) + 2 == N
                && (std::is_convertible_v<Args, T> && ...)>::value, T> y,
                Args... args) noexcept:
            array_{{T(x), T(y), T(args)...}} {}
        constexpr explicit Vector(const T* ptr) noexcept: array_{} { for (int i = 0; i < N; ++i) array_[i] = ptr[i]; }
        template <typename U> constexpr explicit Vector(const Vector<U, N>& v) noexcept:
            array_{} { for (int i = 0; i < N; ++i) array_[i] = T(v[i]); }

        constexpr Vector(const Vector& v) noexcept = default;
        constexpr Vector(Vector&& v) noexcept = default;
        constexpr Vector& operator=(const Vector& v) noexcept = default;
        constexpr Vector& operator=(Vector&& v) noexcept = default;

        constexpr T& operator[](int i) noexcept { return array_[i]; }
        constexpr const T& operator[](int i) const noexcept { return array_[i]; }
        constexpr T& x() noexcept { return (*this)[0]; }
        constexpr T x() const noexcept { return (*this)[0]; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, (N >= 2)>::value, T&> y() noexcept { return (*this)[1]; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, (N >= 2)>::value, T> y() const noexcept { return (*this)[1]; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, (N >= 3)>::value, T&> z() noexcept { return (*this)[2]; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, (N >= 3)>::value, T> z() const noexcept { return (*this)[2]; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, (N >= 4)>::value, T&> w() noexcept { return (*this)[3]; }
        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, (N >= 4)>::value, T> w() const noexcept { return (*this)[3]; }

        constexpr T* begin() noexcept { return array_.data(); }
        constexpr const T* begin() const noexcept { return array_.data(); }
        constexpr T* end() noexcept { return begin() + N; }
        constexpr const T* end() const noexcept { return begin() + N; }

        constexpr Vector operator+() const noexcept { return *this; }
        constexpr Vector operator-() const noexcept { auto v = *this; for (auto& x: v) x = - x; return v; }
        constexpr Vector& operator+=(const Vector& v) noexcept { for (int i = 0; i < N; ++i) (*this)[i] += v[i]; return *this; }
        constexpr Vector& operator-=(const Vector& v) noexcept { for (int i = 0; i < N; ++i) (*this)[i] -= v[i]; return *this; }
        constexpr Vector& operator*=(const Vector& v) noexcept { for (int i = 0; i < N; ++i) (*this)[i] *= v[i]; return *this; }
        constexpr Vector& operator/=(const Vector& v) noexcept { for (int i = 0; i < N; ++i) (*this)[i] /= v[i]; return *this; }
        constexpr Vector& operator*=(T x) noexcept { for (auto& y: *this) y *= x; return *this; }
        constexpr Vector& operator/=(T x) noexcept { for (auto& y: *this) y /= x; return *this; }
        constexpr friend Vector operator+(const Vector& a, const Vector& b) noexcept { auto c = a; c += b; return c; }
        constexpr friend Vector operator-(const Vector& a, const Vector& b) noexcept { auto c = a; c -= b; return c; }
        constexpr friend Vector operator*(const Vector& a, const Vector& b) noexcept { auto c = a; c *= b; return c; }
        constexpr friend Vector operator*(const Vector& a, T b) noexcept { auto c = a; c *= b; return c; }
        constexpr friend Vector operator*(T a, const Vector& b) noexcept { auto c = b; c *= a; return c; }
        constexpr friend Vector operator/(const Vector& a, const Vector& b) noexcept { auto c = a; c /= b; return c; }
        constexpr friend Vector operator/(const Vector& a, T b) noexcept { auto c = a; c /= b; return c; }

        constexpr T dot(const Vector& v) const noexcept { return std::inner_product(begin(), end(), v.begin(), T(0)); }
        constexpr friend T operator%(const Vector& a, const Vector& b) noexcept { return a.dot(b); }

        template <typename U = T>
            constexpr std::enable_if_t<Detail::SfinaeBoolean<U, N == 3>::value, Vector> cross(const Vector& v) const noexcept {
                auto& u = *this;
                return {u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
            }

        constexpr friend bool operator==(const Vector& a, const Vector& b) noexcept
            { for (int i = 0; i < N; ++i) if (a[i] != b[i]) return false; return true; }
        constexpr friend bool operator!=(const Vector& a, const Vector& b) noexcept { return ! (a == b); }

        constexpr T angle(const Vector& v) const noexcept
            { return is_null() || v.is_null() ? T(0) : std::acos(*this % v / std::sqrt(r2() * v.r2())); }
        constexpr Vector dir() const noexcept { return is_null() ? null() : *this / r(); }
        constexpr bool empty() noexcept { return false; }
        constexpr bool is_null() const noexcept { return *this == null(); }
        constexpr Vector project(const Vector& v) const noexcept { return v.is_null() ? null() : v * ((*this % v) / (v % v)); }
        constexpr Vector reject(const Vector& v) const noexcept { return *this - project(v); }
        T r() const noexcept { return std::sqrt(r2()); }
        constexpr T r2() const noexcept { T sum = 0; for (auto x: *this) sum += x * x; return sum; }
        constexpr size_t size() const noexcept { return N; }

        size_t hash() const noexcept { return Detail::hash_range(*this); }
        std::string str(const RS::Format::FormatSpec& spec = {}) const;
        friend std::ostream& operator<<(std::ostream& out, const Vector& v) { return out << v.str(); }

        constexpr static Vector null() noexcept { return Vector(T{0}); }
        constexpr static Vector unit(int i) noexcept { Vector v(T{0}); v[i] = T(1); return v; }

    private:

        std::array<T, N> array_;

    };

    using Int2 = Vector<int, 2>;
    using Int3 = Vector<int, 3>;
    using Int4 = Vector<int, 4>;
    using Float2 = Vector<float, 2>;
    using Float3 = Vector<float, 3>;
    using Float4 = Vector<float, 4>;
    using Double2 = Vector<double, 2>;
    using Double3 = Vector<double, 3>;
    using Double4 = Vector<double, 4>;
    using Ldouble2 = Vector<long double, 2>;
    using Ldouble3 = Vector<long double, 3>;
    using Ldouble4 = Vector<long double, 4>;

    template <typename T>
    constexpr Vector<T, 3>& operator^=(Vector<T, 3>& a, const Vector<T, 3>& b) noexcept {
        return a = a.cross(b);
    }

    template <typename T>
    constexpr Vector<T, 3> operator^(const Vector<T, 3>& a, const Vector<T, 3>& b) noexcept {
        return a.cross(b);
    }

    template <typename T, int N>
    std::string Vector<T, N>::str(const RS::Format::FormatSpec& spec) const {
        return RS::Format::format_range(*this, spec);
    }

    template <typename T, int N>
    constexpr Vector<T, N> clampv(const Vector<T, N>& x, const Vector<T, N>& min, const Vector<T, N>& max) noexcept {
        Vector<T, N> y;
        for (int i = 0; i < N; ++i)
            y[i] = std::clamp(x[i], min[i], max[i]);
        return y;
    }

    template <typename T, int N>
    constexpr Vector<T, N> minv(const Vector<T, N>& x, const Vector<T, N>& y) noexcept {
        Vector<T, N> z;
        for (int i = 0; i < N; ++i)
            z[i] = std::min(x[i], y[i]);
        return z;
    }

    template <typename T, int N>
    constexpr Vector<T, N> maxv(const Vector<T, N>& x, const Vector<T, N>& y) noexcept {
        Vector<T, N> z;
        for (int i = 0; i < N; ++i)
            z[i] = std::max(x[i], y[i]);
        return z;
    }

    template <typename T, int N>
    constexpr std::pair<Vector<T, N>, Vector<T, N>> minmaxv(const Vector<T, N>& x, const Vector<T, N>& y) noexcept {
        std::pair<Vector<T, N>, Vector<T, N>> pair;
        for (int i = 0; i < N; ++i)
            std::tie(pair.first[i], pair.second[i]) = std::minmax(x[i], y[i]);
        return pair;
    }

}

namespace std {

    template <typename T, int N>
    struct hash<RS::Graphics::Core::Vector<T, N>> {
        size_t operator()(const RS::Graphics::Core::Vector<T, N>& v) const noexcept { return v.hash(); }
    };

    template <typename T, int N>
    struct greater<RS::Graphics::Core::Vector<T, N>> {
        bool operator()(const RS::Graphics::Core::Vector<T, N>& x, const RS::Graphics::Core::Vector<T, N>& y) const noexcept {
            return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end(), std::greater<T>());
        }
    };

    template <typename T, int N>
    struct less<RS::Graphics::Core::Vector<T, N>> {
        bool operator()(const RS::Graphics::Core::Vector<T, N>& x, const RS::Graphics::Core::Vector<T, N>& y) const noexcept {
            return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end(), std::less<T>());
        }
    };

}
