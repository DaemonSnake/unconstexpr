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
#include "println.hpp"
#include "meta_tlist.hpp"

template <class T> void unit(T);

namespace auto_testing
{
    using unit_list = unconstexpr::meta_tlist<>;

    template <class Char, Char... chars>
    struct TestLogger final
    {
        template <int = unit_list::push_back<TestLogger>()>
            using pusher = TestLogger;

        static void run() { unit(TestLogger{}); }
        static constexpr Char repr[] = {chars..., ':', '\0'};

        TestLogger() { log::Println("TESTING", repr); }
        ~TestLogger() { log::Println(); }
    };

    template <class... Args>
    constexpr void run(detail::type_list<Args...>)
    {
        (Args::run(), ...);
    }
};

template <class Char, Char... chars>
constexpr auto operator""_logger_string() {
    return (typename auto_testing::TestLogger<Char, chars...>::template pusher<>){};
}

#define new_unit(x) template <> void unit(decltype(x ## _logger_string))
#define run_units_so_far() auto_testing::run(auto_testing::unit_list::current_type<>{})
