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

#include <type_traits>

namespace unconstexpr
{
    namespace detail
    {
        template <class...> struct map;

        template <class Key, class Value>
        struct map_item
        {
            using key = Key;
            using value = Value;
        };

        template <class Key, class Value, class T>
        using map_replace_tool = std::conditional_t<std::is_same_v<Key, typename T::key>,
                                                    map_item<Key, Value>,
                                                    T >;

        struct no_such_key {};

        template <class Key, class T, class... Args>
        constexpr auto map_get_tool()
        {
            if constexpr (std::is_same_v<Key, typename T::key>) return typename T::value{};
            else if constexpr (sizeof...(Args) == 0) return no_such_key{};
            else return map_get_tool<Key, Args...>();
        };

        template <class... Args>
        struct map
        {
            template <class Key>
            static constexpr bool has_key = (std::is_same_v<Key, typename Args::key> || ...);

            template <class Key, class Value>
            using set = std::conditional_t<has_key<Key>,
                                           map<map_replace_tool<Key, Value, Args>...>,
                                           map<Args..., map_item<Key, Value> > >;

            template <class Key>
            using get = std::conditional_t<has_key<Key>,
                                           decltype(map_get_tool<Key, Args...>()),
                                           no_such_key>;

            using clear = map<>;
        };
    }
}
