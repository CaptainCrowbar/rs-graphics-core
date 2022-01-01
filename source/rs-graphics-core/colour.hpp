#pragma once

#include "rs-graphics-core/colour-space.hpp"
#include "rs-graphics-core/maths.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-format/enum.hpp"
#include "rs-format/format.hpp"
#include <functional>
#include <string>
#include <type_traits>

namespace RS::Graphics::Core {

    RS_DEFINE_ENUM_CLASS(ColourLayout, int, 0,
        std,
        std_alpha,
        alpha_std,
        reverse,
        reverse_alpha,
        alpha_reverse
    )

    namespace Detail {

        template <typename CS, char CH, int Offset = 0>
        struct ColourSpaceChannelIndex {
            static constexpr int cs_channels = int(CS::channels.size());
            static constexpr int get() noexcept {
                if constexpr (Offset >= cs_channels)
                    return -1;
                else if constexpr (CS::channels[Offset] == CH)
                    return Offset;
                else
                    return ColourSpaceChannelIndex<CS, CH, Offset + 1>::get();
            }
        };

        template <typename CS, char CH, ColourLayout CL>
        struct ColourChannelIndex {
            static constexpr int cs_channels = int(CS::channels.size());
            static constexpr int cs_index = ColourSpaceChannelIndex<CS, CH>::get();
            static constexpr int get() noexcept {
                if constexpr (cs_index == -1)
                    return -1;
                else if constexpr (CL == ColourLayout::std || CL == ColourLayout::std_alpha)
                    return cs_index;
                else if constexpr (CL == ColourLayout::alpha_std)
                    return cs_index + 1;
                else if constexpr (CL == ColourLayout::reverse || CL == ColourLayout::reverse_alpha)
                    return cs_channels - cs_index - 1;
                else
                    return cs_channels - cs_index;
            }
        };

        template <typename CS, char CH, ColourLayout CL>
        constexpr int colour_channel_index = ColourChannelIndex<CS, CH, CL>::get();

    }

    // Don't use single letter template parameters here

    template <typename VT, typename CS = LinearRGB, ColourLayout CL = ColourLayout::std_alpha>
    class Colour {

    public:

        static_assert(std::is_arithmetic_v<VT>);

        static constexpr int colour_channels = int(CS::channels.size());
        static constexpr bool has_alpha = CL != ColourLayout::std && CL != ColourLayout::reverse;
        static constexpr int channels = colour_channels + int(has_alpha);
        static constexpr bool is_hdr = std::is_floating_point_v<VT>;
        static constexpr ColourLayout layout = CL;
        static constexpr VT scale = is_hdr ? VT(1) : std::numeric_limits<VT>::max();

        using colour_space = CS;
        using iterator = VT*;
        using const_iterator = const VT*;
        using value_type = VT;
        using vector_type = Vector<VT, channels>;
        using colour_vector_type = Vector<VT, colour_channels>;

        constexpr Colour() noexcept {}
        explicit constexpr Colour(VT x) noexcept: vec_(x) { if constexpr (has_alpha) alpha() = scale; }
        template <typename VT2 = VT> constexpr Colour(VT2 x, std::enable_if_t<has_alpha, VT2> a) noexcept: vec_(x) { alpha() = a; }
        explicit constexpr Colour(vector_type v) noexcept: vec_(v) {}

        template <typename... Args>
        constexpr Colour(VT x,
            std::enable_if_t<sizeof...(Args) + 2 == channels && (std::is_convertible_v<Args, VT> && ...), VT> y,
            Args... args) noexcept:
            vec_(x, y, args...) {}

        template <typename... Args>
        constexpr Colour(VT x,
            std::enable_if_t<has_alpha && sizeof...(Args) + 2 == colour_channels && (std::is_convertible_v<Args, VT> && ...), VT> y,
            Args... args) noexcept:
            vec_(x, y, args..., scale) {}

        constexpr VT& alpha(std::enable_if<has_alpha>* = nullptr) noexcept {
            if constexpr (CL == ColourLayout::alpha_std || CL == ColourLayout::alpha_reverse)
                return vec_[0];
            else
                return vec_[colour_channels];
        }

        constexpr const VT& alpha() const noexcept {
            if constexpr (CL == ColourLayout::alpha_std || CL == ColourLayout::alpha_reverse)
                return vec_[0];
            else if constexpr (CL == ColourLayout::std_alpha || CL == ColourLayout::reverse_alpha)
                return vec_[colour_channels];
            else
                return scale;
        }

        constexpr VT& α(std::enable_if<has_alpha>* = nullptr) noexcept { return alpha(); }
        constexpr const VT& α() const noexcept { return alpha(); }

        #define RS_GRAPHICS_COLOUR_CHANNEL(Ch, Lit) \
            constexpr VT& Ch(std::enable_if<Detail::colour_channel_index<CS, Lit, CL> != -1>* = nullptr) noexcept { \
                return vec_[Detail::colour_channel_index<CS, Lit, CL>]; \
            } \
            constexpr VT Ch(std::enable_if<Detail::colour_channel_index<CS, Lit, CL> != -1>* = nullptr) const noexcept { \
                return vec_[Detail::colour_channel_index<CS, Lit, CL>]; \
            }

        RS_GRAPHICS_COLOUR_CHANNEL(a, 'a')  RS_GRAPHICS_COLOUR_CHANNEL(A, 'A')
        RS_GRAPHICS_COLOUR_CHANNEL(b, 'b')  RS_GRAPHICS_COLOUR_CHANNEL(B, 'B')
        RS_GRAPHICS_COLOUR_CHANNEL(c, 'c')  RS_GRAPHICS_COLOUR_CHANNEL(C, 'C')
        RS_GRAPHICS_COLOUR_CHANNEL(d, 'd')  RS_GRAPHICS_COLOUR_CHANNEL(D, 'D')
        RS_GRAPHICS_COLOUR_CHANNEL(e, 'e')  RS_GRAPHICS_COLOUR_CHANNEL(E, 'E')
        RS_GRAPHICS_COLOUR_CHANNEL(f, 'f')  RS_GRAPHICS_COLOUR_CHANNEL(F, 'F')
        RS_GRAPHICS_COLOUR_CHANNEL(g, 'g')  RS_GRAPHICS_COLOUR_CHANNEL(G, 'G')
        RS_GRAPHICS_COLOUR_CHANNEL(h, 'h')  RS_GRAPHICS_COLOUR_CHANNEL(H, 'H')
        RS_GRAPHICS_COLOUR_CHANNEL(i, 'i')  RS_GRAPHICS_COLOUR_CHANNEL(I, 'I')
        RS_GRAPHICS_COLOUR_CHANNEL(j, 'j')  RS_GRAPHICS_COLOUR_CHANNEL(J, 'J')
        RS_GRAPHICS_COLOUR_CHANNEL(k, 'k')  RS_GRAPHICS_COLOUR_CHANNEL(K, 'K')
        RS_GRAPHICS_COLOUR_CHANNEL(l, 'l')  RS_GRAPHICS_COLOUR_CHANNEL(L, 'L')
        RS_GRAPHICS_COLOUR_CHANNEL(m, 'm')  RS_GRAPHICS_COLOUR_CHANNEL(M, 'M')
        RS_GRAPHICS_COLOUR_CHANNEL(n, 'n')  RS_GRAPHICS_COLOUR_CHANNEL(N, 'N')
        RS_GRAPHICS_COLOUR_CHANNEL(o, 'o')  RS_GRAPHICS_COLOUR_CHANNEL(O, 'O')
        RS_GRAPHICS_COLOUR_CHANNEL(p, 'p')  RS_GRAPHICS_COLOUR_CHANNEL(P, 'P')
        RS_GRAPHICS_COLOUR_CHANNEL(q, 'q')  RS_GRAPHICS_COLOUR_CHANNEL(Q, 'Q')
        RS_GRAPHICS_COLOUR_CHANNEL(r, 'r')  RS_GRAPHICS_COLOUR_CHANNEL(R, 'R')
        RS_GRAPHICS_COLOUR_CHANNEL(s, 's')  RS_GRAPHICS_COLOUR_CHANNEL(S, 'S')
        RS_GRAPHICS_COLOUR_CHANNEL(t, 't')  RS_GRAPHICS_COLOUR_CHANNEL(T, 'T')
        RS_GRAPHICS_COLOUR_CHANNEL(u, 'u')  RS_GRAPHICS_COLOUR_CHANNEL(U, 'U')
        RS_GRAPHICS_COLOUR_CHANNEL(v, 'v')  RS_GRAPHICS_COLOUR_CHANNEL(V, 'V')
        RS_GRAPHICS_COLOUR_CHANNEL(w, 'w')  RS_GRAPHICS_COLOUR_CHANNEL(W, 'W')
        RS_GRAPHICS_COLOUR_CHANNEL(x, 'x')  RS_GRAPHICS_COLOUR_CHANNEL(X, 'X')
        RS_GRAPHICS_COLOUR_CHANNEL(y, 'y')  RS_GRAPHICS_COLOUR_CHANNEL(Y, 'Y')
        RS_GRAPHICS_COLOUR_CHANNEL(z, 'z')  RS_GRAPHICS_COLOUR_CHANNEL(Z, 'Z')

        VT& operator[](int i) noexcept { return vec_[i]; }
        const VT& operator[](int i) const noexcept { return vec_[i]; }

        constexpr vector_type as_vector() const noexcept { return vec_; }
        constexpr colour_vector_type colour_vector() const noexcept { return colour_vector_type(vec_.begin()); }

        constexpr VT* begin() noexcept { return vec_.begin(); }
        constexpr const VT* begin() const noexcept { return vec_.begin(); }
        constexpr VT* end() noexcept { return vec_.end(); }
        constexpr const VT* end() const noexcept { return vec_.end(); }

        constexpr bool empty() const noexcept { return false; }
        constexpr size_t size() const noexcept { return size_t(channels); }

        size_t hash() const noexcept { return vec_.hash(); }
        std::string str(RS::Format::FormatSpec spec = {}) const { return vec_.str(spec); }

        friend constexpr bool operator==(Colour a, Colour b) noexcept { return a.vec_ == b.vec_; }
        friend constexpr bool operator!=(Colour a, Colour b) noexcept { return ! (a == b); }

    private:

        vector_type vec_;

    };

    using Rgb8 = Colour<uint8_t, LinearRGB, ColourLayout::std>;
    using Rgba8 = Colour<uint8_t, LinearRGB, ColourLayout::std_alpha>;
    using sRgb8 = Colour<uint8_t, sRGB, ColourLayout::std>;
    using sRgba8 = Colour<uint8_t, sRGB, ColourLayout::std_alpha>;
    using Rgb16 = Colour<uint16_t, LinearRGB, ColourLayout::std>;
    using Rgba16 = Colour<uint16_t, LinearRGB, ColourLayout::std_alpha>;
    using sRgb16 = Colour<uint16_t, sRGB, ColourLayout::std>;
    using sRgba16 = Colour<uint16_t, sRGB, ColourLayout::std_alpha>;
    using Rgbf = Colour<float, LinearRGB, ColourLayout::std>;
    using Rgbaf = Colour<float, LinearRGB, ColourLayout::std_alpha>;
    using sRgbf = Colour<float, sRGB, ColourLayout::std>;
    using sRgbaf = Colour<float, sRGB, ColourLayout::std_alpha>;
    using Rgbd = Colour<double, LinearRGB, ColourLayout::std>;
    using Rgbad = Colour<double, LinearRGB, ColourLayout::std_alpha>;
    using sRgbd = Colour<double, sRGB, ColourLayout::std>;
    using sRgbad = Colour<double, sRGB, ColourLayout::std_alpha>;

}

namespace std {

    template <typename VT, typename CS, RS::Graphics::Core::ColourLayout CL>
    struct hash<RS::Graphics::Core::Colour<VT, CS, CL>> {
        size_t operator()(const RS::Graphics::Core::Colour<VT, CS, CL>& c) const noexcept { return c.hash(); }
    };

}
