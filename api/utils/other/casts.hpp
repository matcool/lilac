#pragma once

#include <inttypes.h>

namespace lilac::cast {
    /** 
     * Alias for reinterpret_cast
     */
    template <typename T, typename R>
    static constexpr T as(R const v) {
        return reinterpret_cast<T>(v);
    }

    /**
     * Cast from anything to anything else,
     * provided they are the same size
     */
    template<typename R, typename T>
    static constexpr R union_cast(T v) {
        static_assert(sizeof T == sizeof R, "union_cast: R and T don't match in size!");
        union {
            R r;
            T t;
        } x;
        x.t = v;
        return x.r;
    }

    #ifndef LILAC_NO_CRINGE

    /**
     * Cast from any type to an enum,
     * provided it is the same size as
     * `int`
     */
    template<typename E, typename T>
    static constexpr E enum_cast(T type) {
        return static_cast<E>(reinterpret_cast<int>(type));
    }

    /**
     * Cast from any type to bool
     * without compiler warnings
     */
    template <typename T>
    static constexpr bool bool_cast(T const v) {
        return static_cast<bool>(reinterpret_cast<int>(v));
    }

    #endif
}
