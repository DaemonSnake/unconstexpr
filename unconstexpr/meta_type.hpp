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
    namespace detail
    {
        template<class T>
        struct ReturnTool
        {
            using type = T;
        };
    }
    
    template <class StartType, class = void, unsigned = uniq_value::value<> >
    class meta_type
    {
        using counter = detail::meta_counter<meta_type>;

        template <int N>
        struct Flag
        {
            friend constexpr auto adl_flag(Flag<N>);
        };

        template <int N, class NewType>
        struct Writer
        {
            friend constexpr auto adl_flag(Flag<N>)
            {
                return detail::ReturnTool<NewType>{};
            }

            using type = NewType;
        };

        template <class NewType, int>
        struct Changer
        {
            template <int Index = counter::next(),
                      class Ret = typename Writer<Index, NewType>::type>
            static constexpr int change() { return 0; }
        };

    public:
        template <class = typename Writer<0, StartType>::type,
                  int Index = counter::value()>
        using type = typename decltype(adl_flag(Flag<Index>{}))::type;

        template <class NewType, int Index = counter::value(),
                  int = Changer<NewType, Index>::change()>
        static constexpr int change()
        {
            return 0;
        }

        template <int Index = counter::value()>
        static constexpr int counter_value()
        {
            return Index;
        }

    private:
        template <int I>
        struct Changer<type<>, I>
        {
            static constexpr int change() { return 0; }
        };
    };
}
