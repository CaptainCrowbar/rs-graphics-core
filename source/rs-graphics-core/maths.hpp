#pragma once

#include <limits>
#include <type_traits>
#include <utility>

namespace RS::Graphics::Core {

    // Constants

    template <typename T> constexpr T pi = T(3.14159'26535'89793'23846'26433'83279'50288'41972L);
    constexpr auto pi_f = pi<float>;
    constexpr auto pi_d = pi<double>;
    constexpr auto pi_ld = pi<long double>;

    // Algorithms

    template <typename T>
    constexpr std::pair<T, T> euclidean_divide(T x, T y) noexcept {
        static_assert(std::is_arithmetic_v<T>);
        T q, r;
        if constexpr (std::is_floating_point_v<T>) {
            q = x / y;
            T sq = q < T(0) ? T(-1) : T(1);
            q *= sq;
            if (q <= T(std::numeric_limits<uintmax_t>::max()))
                q = T(uintmax_t(q));
            q *= sq;
            r = x - q * y;
        } else {
            q = x / y;
            r = x % y;
        }
        if constexpr (std::is_signed_v<T>) {
            if (r < T(0)) {
                q += y < T(0) ? T(1) : T(-1);
                r += y < T(0) ? - y : y;
            }
        }
        return {q, r};
    }

    template <typename T>
    constexpr T euclidean_quotient(T x, T y) noexcept {
        return euclidean_divide(x, y).first;
    }

    template <typename T>
    constexpr T euclidean_remainder(T x, T y) noexcept {
        return euclidean_divide(x, y).second;
    }

    template <typename T>
    constexpr std::pair<T, T> symmetric_divide(T x, T y) noexcept {
        static_assert(std::is_arithmetic_v<T>);
        static_assert(std::is_signed_v<T>);
        auto qr = euclidean_divide(x, y);
        T ay = y < T(0) ? - y : y;
        if (T(2) * qr.second > ay) {
            qr.first += y < T(0) ? T(-1) : T(1);
            qr.second -= ay;
        }
        return qr;
    }

    template <typename T>
    constexpr T symmetric_quotient(T x, T y) noexcept {
        return symmetric_divide(x, y).first;
    }

    template <typename T>
    constexpr T symmetric_remainder(T x, T y) noexcept {
        return symmetric_divide(x, y).second;
    }

    template <typename X, typename Y>
    constexpr Y interpolate(X x1, Y y1, X x2, Y y2, X x3) noexcept {
        static_assert(std::is_floating_point_v<X>);
        return y1 + (y2 - y1) * ((x3 - x1) / (x2 - x1));
    }

    template <typename T>
    constexpr T to_degrees(T rad) noexcept {
        return rad * (T(180) / pi<T>);
    }

    template <typename T>
    constexpr T to_radians(T deg) noexcept {
        return deg * (pi<T> / T(180));
    }

    // Literals

    namespace Literals {

        constexpr float operator""_degf(long double x) noexcept { return to_radians(float(x)); }
        constexpr float operator""_degf(unsigned long long x) noexcept { return to_radians(float(x)); }
        constexpr double operator""_degd(long double x) noexcept { return to_radians(double(x)); }
        constexpr double operator""_degd(unsigned long long x) noexcept { return to_radians(double(x)); }
        constexpr long double operator""_degld(long double x) noexcept { return to_radians(x); }
        constexpr long double operator""_degld(unsigned long long x) noexcept { return to_radians(static_cast<long double>(x)); }

    }

}
