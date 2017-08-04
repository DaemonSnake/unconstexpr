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

namespace unconstexpr
{
    class uniq_value
    {
        struct Detail
        {
            template <unsigned N>
            struct Flag
            {
                friend constexpr bool adl_flag(Flag<N>);
            };
        
            template <unsigned N>
            struct Writer
            {
                friend constexpr bool adl_flag(Flag<N>)
                {
                    return true;
                }

                static constexpr unsigned value = N;
            };

            template <unsigned N = 0, bool = adl_flag(Flag<N>{})>
            static constexpr unsigned reader(int, unsigned r = reader<N+1>(0))
            {
                return r;
            }

            template <unsigned N = 0>
            static constexpr unsigned reader(float)
            {
                return Writer<N>::value;
            }
        };

    public:
        template <unsigned N = Detail::reader(0)>
        static constexpr unsigned value = N;
    };
}
