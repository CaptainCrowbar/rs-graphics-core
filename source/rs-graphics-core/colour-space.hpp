#pragma once

#include "rs-graphics-core/maths.hpp"
#include "rs-graphics-core/matrix.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/enum.hpp"
#include <array>
#include <type_traits>

namespace RS::Graphics::Core {

    class CIELab;
    class CIELuv;
    class CIExyY;
    class CIEXYZ;
    class HSL;
    class HSV;
    class LinearRGB; // Linearized sRGB
    class sRGB;

    RS_DEFINE_ENUM_CLASS(ChannelMode, uint8_t, 0,
        unconstrained,  // Any real value
        non_negative,   // Non-negative real value
        unit,           // Unit interval [0,1]
        circle,         // Circular interval [0,2π)
    )

    struct ChannelSpec {
        char id;
        ChannelMode mode;
    };

    template <typename T>
    constexpr bool is_channel_in_gamut(T t, ChannelMode cm) noexcept {
        static_assert(std::is_floating_point_v<T>);
        switch (cm) {
            case ChannelMode::non_negative:  return t >= 0;
            case ChannelMode::unit:          return t >= 0 && t <= 1;
            case ChannelMode::circle:        return t >= 0 && t < 2 * pi<T>;
            default:                         return true;
        }
    };

    template <typename ColourSpace, typename T, int N>
    constexpr bool is_colour_in_gamut(Vector<T, N> colour) noexcept {
        static_assert(std::is_floating_point_v<T>);
        static_assert(N == ColourSpace::n_channels);
        for (int i = 0; i < N; ++i)
            if (! is_channel_in_gamut(colour[i], ColourSpace::channels[i].mode))
                return false;
        return true;
    }

    template <typename T>
    constexpr void clamp_channel(T& t, ChannelMode cm) noexcept {
        static_assert(std::is_floating_point_v<T>);
        switch (cm) {
            case ChannelMode::non_negative:
                if (t < 0)
                    t = 0;
                break;
            case ChannelMode::unit:
                if (t < 0)
                    t = 0;
                else if (t > 1)
                    t = 1;
                break;
            case ChannelMode::circle:
                t = euclidean_remainder(t, 2 * pi<T>);
                break;
            default:
                break;
        }
    };

    template <typename ColourSpace, typename T, int N>
    constexpr void clamp_colour(Vector<T, N>& colour) noexcept {
        static_assert(std::is_floating_point_v<T>);
        static_assert(N == ColourSpace::n_channels);
        for (int i = 0; i < N; ++i)
            clamp_channel(colour[i], ColourSpace::channels[i].mode);
    }

    // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

    template <typename T>
    constexpr Matrix<T, 3> rgb_to_xyz_matrix() noexcept {
        return Matrix<T, 3, MatrixLayout::row> {
            T(0.4124564), T(0.3575761), T(0.1804375),
            T(0.2126729), T(0.7151522), T(0.0721750),
            T(0.0193339), T(0.1191920), T(0.9503041),
        };
    }

    template <typename T>
    constexpr Matrix<T, 3> xyz_to_rgb_matrix() noexcept {
        return rgb_to_xyz_matrix<T>().inverse();
    }

    // Colour space concept:
    // - using base = CIEXYZ or LinearRGB
    // - static constexpr int n_channels
    // - static constexpr std::array<ChannelSpec, n_channels> channels
    // - static Vector<T, n_channels> from_base(Vector<T, 3> colour)
    // - static Vector<T, 3> to_base(Vector<T, n_channels> colour)

    class CIEXYZ {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'X', ChannelMode::non_negative },
            { 'Y', ChannelMode::non_negative },
            { 'Z', ChannelMode::non_negative },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

    class LinearRGB {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept { return CIEXYZ::to_base(colour); }
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept { return CIEXYZ::from_base(colour); }
    };

        template <typename T>
        Vector<T, 3> CIEXYZ::from_base(Vector<T, 3> colour) noexcept {
            return rgb_to_xyz_matrix<T>() * colour;
        }

        template <typename T>
        Vector<T, 3> CIEXYZ::to_base(Vector<T, 3> colour) noexcept {
            return xyz_to_rgb_matrix<T>() * colour;
        }

    class CIELab {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'L', ChannelMode::unit },
            { 'a', ChannelMode::unconstrained },
            { 'b', ChannelMode::unconstrained },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> CIELab::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> CIELab::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class CIELuv {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'L', ChannelMode::non_negative },
            { 'u', ChannelMode::unconstrained },
            { 'v', ChannelMode::unconstrained },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> CIELuv::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> CIELuv::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class CIExyY {
    public:
        using base = CIEXYZ;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'x', ChannelMode::unit },
            { 'y', ChannelMode::unit },
            { 'Y', ChannelMode::non_negative },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> CIExyY::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            T sum = colour[0] + colour[1] + colour[2];
            out[0] = colour[0] / sum;
            out[1] = colour[1] / sum;
            out[2] = colour[1];
            return out;
        }

        template <typename T>
        Vector<T, 3> CIExyY::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            T scale = colour[2] / colour[1];
            out[0] = scale * colour[0];
            out[1] = colour[2];
            out[2] = scale * (1 - colour[0] - colour[1]);
            return out;
        }

    class HSL {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'H', ChannelMode::unit },
            { 'S', ChannelMode::unit },
            { 'L', ChannelMode::circle },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> HSL::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> HSL::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class HSV {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'H', ChannelMode::unit },
            { 'S', ChannelMode::unit },
            { 'V', ChannelMode::circle },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> HSV::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> HSV::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

    class sRGB {
    public:
        using base = LinearRGB;
        static constexpr int n_channels = 3;
        static constexpr std::array<ChannelSpec, n_channels> channels = {{
            { 'R', ChannelMode::unit },
            { 'G', ChannelMode::unit },
            { 'B', ChannelMode::unit },
        }};
        template <typename T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <typename T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename T>
        Vector<T, 3> sRGB::from_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

        template <typename T>
        Vector<T, 3> sRGB::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            // TODO
            (void)colour;
            return out;
        }

}
