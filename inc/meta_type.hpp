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

#include "meta_counter.hpp"

namespace unconstexpr
{
    template <class StartType, class Id = void>
    class meta_type
    {
        using counter = meta_counter<meta_type<StartType, Id>>;

        template <int N>
        struct flag
        {
            friend constexpr auto adl_flag(flag<N>);
        };

        template <int N, class NewType>
        struct writer
        {
            friend constexpr auto adl_flag(flag<N>)
            {
                return NewType{};
            }
            
            using type = NewType;
        };

    public:
        template <class = typename writer<0, StartType>::type,
                  int index = counter::value()>
        using type = decltype(adl_flag(flag<index>{}));

        template <class NewType, int index = counter::next(),
                  class Ret = typename writer<index, NewType>::type>
        static constexpr int change()
        {
            return 0;
        }
    };
}
