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
#include "tools/type_map.hpp"
#include "tools.hpp"

#define META_KEY(x) decltype(unconstexpr::tools::template_holder<x>())

namespace unconstexpr
{
    template <class Id = void, unsigned = uniq_value::value<>>
    class meta_map
    {
        using parent = meta_type<detail::map<>, meta_map, 0>;

    public:
        template <class current = typename parent::template type<> >
        using current_type = current;

        template <class Key>
        static constexpr bool has_key(bool r = current_type<>::template has_key<Key >)
        {
            return r;
        }

#define BUILD_SET(KeyT, ValueT)                                                          \
        template <KeyT Key, ValueT Value>                                                \
        static constexpr int set(int = parent::template change<typename current_type<>:: \
                                 template set<                                           \
                                 decltype(tools::template_holder<Key>()), \
                                 decltype(tools::template_holder<Value>())> >()) { return 0; }

        BUILD_SET(auto, auto);
        BUILD_SET(class, class);
        BUILD_SET(auto, class);
        BUILD_SET(class, auto);

#undef BUILD_SET

        static constexpr int clear(int = parent::template change<typename current_type<>::clear>()) { return 0; }

        template <class Key, class Result = typename current_type<>::template get< tools::type_holder<Key> > >
        using get_by_class = Result;

        template <auto Key, class Result = typename current_type<>::template get< tools::value_holder<Key> > >
        using get_by_value = Result;

        template <class KeyTypeOrValue, class Result = typename current_type<>::template get<KeyTypeOrValue> >
        using get = Result;
    };
}
