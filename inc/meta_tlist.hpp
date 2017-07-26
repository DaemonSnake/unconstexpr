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
#include "tools/type_list.hpp"

namespace unconstexpr
{
    template <class Id = void, unsigned = uniq_value::value<> >
    class meta_tlist
    {
        using parent = meta_type<detail::type_list<>, meta_tlist, 0>;

    public:
        template <class current = typename parent::template type<>>
            using current_type = current;

        template <class... NewArgs> static constexpr void
        push_front(int = parent::template change<typename current_type<>::template
                   push_front<NewArgs...>>()) {}

        template <class... NewArgs> static constexpr void
        push_back(int = parent::template change<typename current_type<>::template
                  push_back<NewArgs...>>()) {}

        template <size_t N = 1> static constexpr void
        pop_front(int = parent::template change<typename current_type<>::template
                  pop_front<N>>()) {}

        template <size_t N = 1> static constexpr void
        pop_back(int = parent::template change<typename current_type<>::template
                 pop_back<N>>()) {}

        template <size_t N, class Ret = typename current_type<>::template item<N>>
            using item = Ret;

        template <template<class...> class Holder,
                  class Ret = typename current_type<>::template transfer<Holder>>
            using transfer = Ret;

        template <class OtherTypeList> static constexpr void
        insert_list(int = parent::template change<typename current_type<>::template
                    merge<OtherTypeList>>()) {}

        static constexpr void clear(int = parent::template change<detail::type_list<>>()) {}

        template <size_t From, size_t Len> static constexpr void
        select(int = parent::template change<typename current_type<>::template
               select<From, Len>>()) {}

        template <size_t Index> static constexpr void
        remove(int = parent::template change<typename current_type<>::template
               remove<Index>>()) {}
    };
}
