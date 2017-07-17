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

#include <utility>

namespace unconstexpr::detail
{
    template <class... Args>
    class type_list
    {
        template <size_t, class T> struct Select;
        template <class T> struct MergeTool;

        template <size_t Offset, size_t... Indexs>
        struct Select<Offset, std::index_sequence<Indexs...>>
        {
            using type =
                type_list<std::tuple_element_t<Offset + Indexs, std::tuple<Args...>>...>;
        };

        template <class... OtherArgs>
        struct MergeTool<type_list<OtherArgs...>>
        {
            using type = type_list<Args..., OtherArgs...>;
        };

        static constexpr size_t safe_sub(size_t value, size_t n) {
            return (value < n ? 0 : value - n);
        }

        template <size_t toRemove, size_t... Indexs>
        static constexpr auto tool_remove(std::index_sequence<Indexs...>)
        {
            constexpr auto change = [](size_t value) {
                return value + (value >= toRemove ? 1 : 0);
            };

            return std::index_sequence<change(Indexs)...>{};
        }

    public:
        template <size_t From, size_t Len, class Indexs = std::make_index_sequence<Len>>
        using select = typename Select<From, Indexs>::type;

        template <class... NewArgs>
        using push_front = type_list<NewArgs..., Args...>;

        template <class... NewArgs>
        using push_back = type_list<Args..., NewArgs...>;

        template <size_t N>
        using pop_front = select<N, safe_sub(sizeof...(Args), N)>;

        template <size_t N>
        using pop_back = select<0, safe_sub(sizeof...(Args), N)>;

        template <size_t N>
        using item = std::tuple_element_t<N, std::tuple<Args...>>;

        template <template<class...> class Holder>
        using transfer = Holder<Args...>;

        template <class OtherTypeList>
        using merge = typename MergeTool<OtherTypeList>::type;

        template <size_t toRemove>
        using remove = typename
            Select<0,decltype(tool_remove<toRemove>(std::make_index_sequence<safe_sub(sizeof...(Args), 1)>{}))>::type;
    };
}
