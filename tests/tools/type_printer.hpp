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
#include <algorithm>

using namespace std::literals;

namespace type_repr
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

    inline bool char_to_string(char c, std::string &res)
    {
        if ((c >= 1 && c <= 6) || (c >= 14 && c <= 31) || c == 127)
            return false;
        if (c >= ' ')
            res = "'"s + c + "'";
        else
            res = "'\\"s + std::map<char, char>({
                    {'\0', '0'}, {'\a', 'a'}, {'\b', 'b'}, {'\t', 't'},
                    {'\n', 'n'}, {'\v', 'v'}, {'\f', 'f'}, {'\r', 'r'}
                })[c] + '\'';
        return true;
    }

    inline void charcast_replace(std::string &str)
    {
        size_t index = 0;
        const std::string motif = "(char)";
        while ((index = str.find(motif, index)) != std::string::npos)
        {
            size_t end = 0;
            std::string with;
            if (char_to_string((char)std::stoi(&str[index + motif.size()], &end), with)) {
                str.replace(index, motif.size() + end, with);
                index -= ((motif.size() + end) - with.size());
            }
        }
    }

    template <class T = NoPrint, bool new_line = true>
    struct TypePrinter {};

    template <class T, bool new_line>
    std::ostream& operator<<(std::ostream &stream, TypePrinter<T, new_line>)
    {
        if constexpr (!std::is_same_v<T, NoPrint>)
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
            charcast_replace(true_name);
            stream <<  true_name;
            if (!namespe.empty())
                stream << " (:: ==> " + namespe + ")";
        }
        if constexpr (new_line) stream << std::endl;
        return stream;
    }

    template <auto T, bool new_line = true>
    struct ValuePrinter {};

    template <auto T, bool new_line>
    std::ostream& operator<<(std::ostream &stream, ValuePrinter<T, new_line>)
    {
        if constexpr (new_line) return stream << T << std::endl;
        else return stream << T;
    }

    template <class T = NoPrint, bool new_line = true>
    void type_printer()
    {
        std::cout << TypePrinter<T, new_line>();
    }

    template <auto T, bool new_line = true>
    void type_printer()
    {
        std::cout << ValuePrinter<T, new_line>();
    }

    template <class T, bool new_line = true>
    void make_type_printer(T const &) {
        type_printer<T, new_line>();
    }

    template <class T, bool new_line = false>
    auto make_type_printer()
    {
        return TypePrinter<T, new_line>();
    }

    template <auto T, bool new_line = false>
    auto make_type_printer()
    {
        return ValuePrinter<T, new_line>();
    }
}

using type_repr::type_printer;
using type_repr::make_type_printer;
