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
#include <utility>
#include "meta_counter.hpp"

namespace unconstexpr
{
    template<class T, size_t> struct partial;

    namespace detail
    {
        template<class T, size_t... Is>
        struct partial_class : partial<T, Is>... {};

        template<class T, int = sizeof(partial<T, T::value()>)>
        constexpr size_t safe_next(int) { return T::next(); }

        template<class T, size_t R = T::value()>
        constexpr size_t safe_next(float)
        {
            if constexpr(R == 0) return T::next();
            else return R;
        }

        template<class Id, size_t... Is>
        auto partial_tool_impl(std::index_sequence<Is...>) { return partial_class<Id, (Is+1)...>{}; }
    }

    template<class T, size_t = detail::safe_next<T>(0)>
    struct partial;

    template<class T, size_t C = T::value()>
    using partial_t = decltype(detail::partial_tool_impl<T>(std::make_index_sequence<C>{}));

    template<size_t C = uniq_value::value<> >
    struct partial_it : meta_counter<size_t, 0, 1, C> {};
}
