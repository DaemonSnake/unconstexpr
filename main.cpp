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
#include "meta_counter.hpp"
#include "meta_var.hpp"
#include "meta_type.hpp"
#include "meta_tlist.hpp"
#include "meta_vlist.hpp"
#include "meta_variant.hpp"
using namespace unconstexpr;

#include "tools/log.hpp"

new_unit("meta_counter")
{
    using counter = meta_counter<>;
    
    println(counter::value());
    println(counter::next());
    println(counter::next());
    println(counter::next());
}

new_unit("meta_var")
{
    using var = meta_var<int>;
    
    println(var::value());
    var::set<5>();
    static_assert(var::value() == 5);
    println(var::value());
    var::op<'+', 5>();
    println(var::value());
    static_assert(var::value() == 10);
    var::op<'*', 2>();
    println(var::value());
    static_assert(var::value() == 20);
    var::apply([](int i) {
            return i * i;
        });
    println(var::value());
    static_assert(var::value() == 400);
}

new_unit("meta_tlist")
{
    using list = meta_tlist<>;

    printType(list::current_type<>);
    list::push_front<int>();
    list::push_front<double>();
    printType(list::current_type<>);
    list::remove<-1>();
    printType(list::item<0>);
}

static constexpr int i = 0, j = 25;

new_unit("meta_vlist")
{
    using list = meta_vlist<>;

    printType(list::current_type<>);
    list::push_back<&i, 52>();
    printType(list::current_type<>);
    list::push_back<&i, 52>();
    printType(list::current_type<>);
    println(list::item<0>, list::item<1>);
    constexpr auto v = list::value_transfer<std::tuple>();
    printType(decltype(v));
    list::clear();
    printType(decltype(list::value_transfer<std::tuple>()));
    list::push_front<42>();
    println(list::item<0>);
    // list::foreach;
    // std::apply;
}

new_unit("meta_variant")
{
    struct tag {};
    using v = meta_variant<int, tag>;

    v::value<> = 5;
    println(v::value<>);
    printType(decltype(v::value<>));
    
    v::change<double>(3.14);
    static_assert(std::is_same_v<double, decltype(v::value<>)>);
    println(v::value<>);
    printType(decltype(v::value<>));

    println(v::change<int>(), make_type_printer<decltype(v::value<>)>());
}

int main ()
{
    for (const auto &tests: units)
        tests();
}
