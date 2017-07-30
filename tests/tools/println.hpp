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

#include "type_printer.hpp"

namespace log
{
    template <bool started = false>
    struct Joiner
    {
        template <class T>
        Joiner<true> operator<<(T const &arg)
        {
            if constexpr (started) std::cout << ' ';
            std::cout << arg;
            return {};
        }

        Joiner<false> operator<<(std::ostream &(*f)(std::ostream &))
        {
            std::cout << f;
            return {};
        }
    };

    template <class ...Args>
    void Println(Args&&... args)
    {
        (log::Joiner<>() << ... << args) << std::endl;
    }
}

#define println(args...) log::Println(args, "<=", #args)
#define printType(x) log::Println(make_type_printer<x, false>(), "<=", #x)
