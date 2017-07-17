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

namespace unconstexpr
{
    template <class Tag = void, class Type = int, Type Start = 0, Type Step = 1>
    class meta_counter
    {
        template<Type N>
        struct flag
        {
            friend constexpr Type adl_flag (flag<N>);
        };

        template<Type N>
        struct writer
        {
            friend constexpr Type adl_flag (flag<N>)
            {
                return N;
            }

            static constexpr Type value = N;
        };

        template<Type N, Type ThisR = adl_flag (flag<N> {})>
        static constexpr Type reader (int, flag<N>, Type R = reader(0, flag<N + Step>{}))
        {
            return (R != Start ? R : ThisR);
        }

        template <Type N>
        static constexpr Type reader(float, flag<N>)
        {
            return Start;
        }

    public:
        template <Type = writer<Start>::value>
        static constexpr Type value(Type R = reader(0, flag<Start>{}))
        {
            return R;
        }

        template <Type Value = value()>
        static constexpr Type next(Type R = writer<Value + Step>::value)
        {
            return R;
        }
    };
}
