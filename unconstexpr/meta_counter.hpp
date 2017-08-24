/*
 * Copyright (C) 2017  Bastien Penavayre
 *                     Filip Roséen <filip.roseen@gmail.com>
 *                              http://b.atch.se/posts/constexpr-meta-container
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

#include "uniq_value.hpp"

namespace unconstexpr
{
    namespace detail
    {
        template <class Tag = void, class Type = int, Type Start = 0, Type Step = 1>
        class meta_counter
        {
            template<Type N>
            struct Flag
            {
                friend constexpr bool adl_flag (Flag<N>);
            };

            template<Type N>
            struct Writer
            {
                friend constexpr bool adl_flag (Flag<N>) { return true; }
                static constexpr Type value = N;
            };

            template<Type N, bool = adl_flag(Flag<N>{})>
            static constexpr Type reader (int, Flag<N>, Type r = reader(0, Flag<N + Step>{}))
            {
                return r;
            }

            template <Type N>
            static constexpr Type reader(float, Flag<N>)
            {
                return N;
            }

        public:
            static constexpr Type value(Type r = reader(0, Flag<Start>{}))
            {
                return r;
            }

            template <Type Value = value()>
            static constexpr Type next(Type r = Writer<Value>::value)
            {
                return r + Step;
            }
        };

        template <unsigned> struct unique_type {};
    }

    template <class Type = int, Type Start = 0, Type It = 1, unsigned I = uniq_value::value<> >
    using meta_counter = detail::meta_counter<detail::unique_type<I>, Type, Start, It>;
}
