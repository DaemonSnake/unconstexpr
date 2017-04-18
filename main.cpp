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
#include <iostream>
#include "type_printer.hpp"
#include "meta_counter.hpp"
#include "meta_var.hpp"
#include "meta_type.hpp"
#include "meta_list.hpp"

using namespace unconstexpr;
using namespace printer;

#define printType(x) type_printer<typename x>()
#define println(x) std::cout << x << std::endl

void test_counter()
{
    using counter = meta_counter<>;
    
    println(counter::value());
    println(counter::next());
    println(counter::next());
    println(counter::next());
}

void test_var()
{
    using var = meta_var<int>;
    
    println(var::value());
    var::set<5>();
    println(var::value());
    var::op<'+', 5>();
    println(var::value());
    var::op<'*', 2>();
    println(var::value());
    var::apply([](int i) {
            return i * i;
        });
    println(var::value());
}

void test_list()
{
    using list = meta_list<>;

    printType(list::current_type<>);
    list::push_front<int>();
    list::push_front<double>();
    printType(list::current_type<>);
    list::remove<-1>();
    printType(list::item<0>);
}

int main ()
{
    test_counter();
    test_var();
    test_list();
}
