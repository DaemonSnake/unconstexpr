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
#include "unconstexpr.hpp"
using namespace unconstexpr;
#include "tools/auto_testing.hpp"

void unit_launcher();

int main ()
{
    static_assert(!std::is_same_v<meta_counter<>, meta_counter<>>);
    static_assert(!std::is_same_v<meta_type<int>, meta_type<int>>);
    static_assert(!std::is_same_v<meta_tlist<>, meta_tlist<>>);
    static_assert(!std::is_same_v<meta_vlist<>, meta_vlist<>>);
    static_assert(!std::is_same_v<meta_any<int>, meta_any<int>>);
    static_assert(!std::is_same_v<meta_value<>, meta_value<>>);

    unit_launcher();
}

new_unit("meta_counter")
{
    using counter = meta_counter<>;

    println(counter::value());
    println(counter::next());
    println(counter::next());
    println(counter::next());
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

new_unit("meta_any")
{
    meta_any<int> v;

    v = 5;
    println(*v, make_type_printer<decltype(*v)>());
    static_assert(std::is_same_v<int&, decltype(*v)>);
    v = 3.14;
    println(*v, make_type_printer<decltype(*v)>());
    static_assert(std::is_same_v<double&, decltype(*v)>);
    v = "Hello"s;
    println(*v, make_type_printer<decltype(*v)>());
    static_assert(std::is_same_v<std::string&, decltype(*v)>);
    std::cout << "\"operator<<\" test: " << v << std::endl;
}

new_unit("meta_value")
{
    using type = unconstexpr::meta_value<>;
    static_assert(type::value<> == false);
    static_assert(std::is_same_v<type::type<>, std::false_type>);

    constexpr type tmp = carg(42);
    static_assert(*tmp == 42);
    tmp += carg(3);
    static_assert(*tmp == 45);
    tmp -= carg(42);
    static_assert(*tmp == 3);
    tmp *= carg(3);
    static_assert(*tmp == 9);
    
    static_assert(tmp.compiles());
    tmp /= carg(0);
    if constexpr(!tmp.compiles()) tmp.undo();
    static_assert(*tmp == 9);

    tmp = carg([]() { return 42; });
    static_assert(tmp() == 42);
    
    tmp = carg(std::array{42, 314});
    static_assert(tmp[1] == 314);
}

void unit_launcher()
{
    run_units_so_far();
}
