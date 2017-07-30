/*
 * Copyright (C) 2017  Bastien Penavayre
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "meta_type.hpp"

namespace unconstexpr
{
    template <class StartType, class Id = void, unsigned = uniq_value::value<> >
    class meta_any
    {
        using type = meta_type<StartType, meta_any, 0>;

    public:
        template <class T = typename type::template type<> >
        static inline T value;

        template <class T, int = type::template change<T>()>
        static constexpr T change()
        {
            return value<T>;
        }

        template <class T, int = type::template change<T>()>
        static constexpr T change(T const &newValue)
        {
            return (value<T> = newValue);
        }

        template <int = type::counter_value()>
        constexpr auto &operator*() const {
            return value<>;
        }

        template <class T, int = type::template change<T>()>
        constexpr auto &operator=(T const &newValue) const {
            return (value<T> = newValue);
        }

        template <class T, int = type::counter_value()>
        friend T &operator<<(T &stream, meta_any const &) {
            return stream << meta_any::value<>;
        }
    };
}
