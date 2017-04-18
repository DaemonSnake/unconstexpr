/*
 * Copyright (C) 2016  Bastien Penavayre
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

#include <cxxabi.h>
#include <iostream>
#include <type_traits>
#include <string>
#include <regex>

namespace printer
{
    class NoPrint {};

    inline std::string find_class_name(std::string const &name)
    {
        std::size_t b = name.size();
        std::size_t open = 0;

        for (std::size_t i = 0; i < name.size(); i++)
            if (name[i] == '>')
                --open;
            else if (name[i] == '<')
                if (open++ == 0)
                    b = i;
        return name.substr(0, b);
    }

    template <class T = NoPrint>
    void type_printer()
    {
        int status = -4;
        const char *name = typeid(T).name();
        std::string true_name = abi::__cxa_demangle(name, 0, 0, &status);
        std::string class_name = find_class_name(true_name);
        std::string namespe = [&]() -> std::string {
            std::size_t i = class_name.rfind("::");
            if (i != std::string::npos)
                return class_name.substr(0, i);
            else
                return "";
        }();
        if (!namespe.empty()) {
            std::string::size_type n = 0;
            while ( ( n = true_name.find( namespe, n ) ) != std::string::npos )
            {
                true_name.replace( n, namespe.size(), "" );
                n += 0;
            }
        }
        std::cout <<  true_name;
        if (!namespe.empty())
            std::cout << std::endl << "(:: ==> " + namespe + ")";
        std::cout << std::endl;
    }

    template <>
    inline void type_printer<NoPrint>() {
        printf("\n");
    }

    template <class T>
    void make_type_printer(T const &) {
        type_printer<T>();
    }
}
