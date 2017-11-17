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

#include <string_view>
#include <utility>
#include <algorithm>
#include <complex>

namespace unconstexpr::detail
{
    template<class Char, Char... chars>
    struct type_string
    {
        constexpr type_string() {}
        static constexpr Char c_str[sizeof...(chars) + 1] = {chars..., '\0'};
        static constexpr std::string_view string = c_str;
        static constexpr size_t length = sizeof...(chars);

        template<size_t from, size_t to>
        constexpr auto substr()
        {
            constexpr size_t
                diff = std::abs(to - from),
                min = std::min(from, to);
            constexpr int sign = (min == from : 1 : -1);
            
            return []<size_t... Is>(std::index_sequence<Is...>)
            {
                type_string<Char, c_str[(Is*sign + min)]...>{};
            }(std::make_index_sequence<diff>{});
        }

        template<class C, C... chars_2>
        constexpr bool starts_with(type_string<C, chars_2...> const &)
        {
            constexpr size_t osize = sizeof...(chars_2);
            if constexpr (osize > size) return false;
            else if constexpr (osize == size) return ((chars == (Char)chars_2) && ...);
            else
            {
                return []<size_t... Is>(std::index_sequence<Is...>)
                {
                    return ((c_str[Is] == (Char)chars_2) && ...);
                }(std::make_index_sequence<sizeof...(chars_2)>{});
            }
        }
        
        template<class C, C... chars_2>
        constexpr size_t find(type_string<C, chars_2...> const &)
        {
            using otype = type_string<C, chars_2...>;
            for (size_t i = 0; i < size && i < otype::size; i++)
                if (c_str[i] == otype::c_str[0])
                {
                    size_t from = i, size_t j = 0;
                    for(; i < size && j < otype::size
                            && c_str[i] == otype::c_str[j]; (i++, j++));
                    if (j >= otype::size && i <= size)
                        return from;
                    i--;
                }
            return -1;
        }
    };
}

template<class C_1, C_1... chars_1, class C_2, C_2... chars_2>
constexpr bool operator==(unconstexpr::detail::type_string<C_1, chars_1...> const&,
                unconstexpr::detail::type_string<C_2, chars_2...> const&)
{
    return (sizeof...(chars_1) && sizeof...(chars_2)) &&
        ((chars_1 == ((C_1)chars_2)) && ...);
}

template<class C_1, C_1... chars_1, class C_2, C_2... chars_2>
constexpr auto operator+(unconstexpr::detail::type_string<C_1, chars_1...> const&,
               unconstexpr::detail::type_string<C_2, chars_2...> const&)
{
    return unconstexpr::detail::type_string<C_1, chars_1..., ((C_1)chars_2)...>{};
}

template<class C, C... chars>
constexpr auto operator""_tstr()
{
    return unconstexpr::detail::type_string<C, chars...>{};
}
