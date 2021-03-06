#pragma once

#include "rs-graphics-core/maths.hpp"
#include <initializer_list>
#include <iterator>
#include <map>
#include <type_traits>

namespace RS::Graphics::Core {

    template <typename X, typename Y = X>
    class LinearMap {

    private:

        static_assert(std::is_floating_point_v<X>);

        struct init_type {
            X x0;
            Y y1, y2, y3;
            init_type(X x, Y y) noexcept: x0(x), y1(y), y2(y), y3(y) {}
            init_type(X x, Y yl, Y yr) noexcept: x0(x), y1(yl), y2(midpoint(yl, yr)), y3(yr) {}
            init_type(X x, Y yl, Y y, Y yr) noexcept: x0(x), y1(yl), y2(y), y3(yr) {}
        };

    public:

        using key_type = X;
        using mapped_type = Y;

        LinearMap() = default;
        LinearMap(std::initializer_list<init_type> list);

        Y operator[](X x) const;
        void clear() noexcept { map_.clear(); }
        bool empty() const noexcept { return map_.empty(); }
        void insert(X x, Y y) { map_[x] = {y, y, y}; }
        void insert(X x, Y yl, Y yr) { map_[x] = {yl, midpoint(yl, yr), yr}; }
        void insert(X x, Y yl, Y y, Y yr) { map_[x] = {yl, y, yr}; }
        void erase(X x) noexcept { map_.erase(x); }
        void erase(X x1, X x2) noexcept;
        X min() const noexcept { return map_.empty() ? X(0) : map_.begin()->first; }
        X max() const noexcept { return map_.empty() ? X(0) : std::prev(map_.end())->first; }

    private:

        struct mapped_values {
            Y left, mid, right;
        };

        using map_type = std::map<X, mapped_values>;

        map_type map_;

        inline static Y midpoint(Y y1, Y y2) noexcept { return y1 + X(0.5) * (y2 - y1); }

    };

        template <typename X, typename Y>
        LinearMap<X, Y>::LinearMap(std::initializer_list<init_type> list) {
            for (auto& in: list)
                map_.insert({in.x0, {in.y1, in.y2, in.y3}});
        }

        template <typename X, typename Y>
        Y LinearMap<X, Y>::operator[](X x) const {
            if (map_.empty())
                return Y();
            auto i = map_.lower_bound(x);
            if (i == map_.end())
                return std::prev(i)->second.right;
            if (i->first == x)
                return i->second.mid;
            if (i == map_.begin())
                return i->second.left;
            auto j = std::prev(i);
            return interpolate(j->first, j->second.right, i->first, i->second.left, x);
        }

        template <typename X, typename Y>
        void LinearMap<X, Y>::erase(X x1, X x2) noexcept {
            auto i = map_.lower_bound(x1);
            auto j = map_.upper_bound(x2);
            map_.erase(i, j);
        }

}
