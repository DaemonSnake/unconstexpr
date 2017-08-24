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

#include "meta_counter.hpp"
#include <type_traits>

namespace unconstexpr
{
    template <class Type, Type Start = Type{0},
              class Id = void, unsigned = uniq_value::value<> >
    class meta_var
    {
        using counter = detail::meta_counter<meta_var>;

        template <int N>
        struct Flag
        {
            friend constexpr Type adl_flag(Flag<N>);
        };

        template <Type V, int N>
        struct Writer
        {
            friend constexpr Type adl_flag(Flag<N>)
            {
                return V;
            }

            static constexpr Type value = V;
        };

        template <int Index, Type R = adl_flag(Flag<Index>{})>
        static constexpr Type reader(int) { return R; }

        template <int Index>
        static constexpr Type reader(float) { return Start; }

    public:

        template <int Index = counter::value()>
        static constexpr Type value(Type r = reader<Index>(0))
        {
            return r;
        }

        template <Type Value, int Index = counter::next()>
        static constexpr Type set(Type r = Writer<Value, Index>::value)
        {
            return r;
        }

    private:

        template <char Op, Type NewValue, Type Value>
        static constexpr Type calc()
        {
            static_assert(Op == '+' || Op == '-' || Op == '*' || Op == '/',
                          "operator not supported");
            static_assert((Op == '/' && NewValue != 0) || Op != '/',
                          "floating point exception");

            Type tmp = Value;
            if (Op == '+')
                tmp += NewValue;
            else if (Op == '-')
                tmp -= NewValue;
            else if (Op == '*')
                tmp *= NewValue;
            else /* if (Op == '/')*/
                tmp /= NewValue;
            return tmp;
        }

        template <class T,
                  std::invoke_result_t<T, Type> (T::*Func)(Type) const = &T::operator()>
        static constexpr Type invoke_func_operator(Type arg)
        {
            static_assert(sizeof(T) <= 1, "Lambda given captures values");
            return (static_cast<T *>(nullptr)->*Func)(arg);
        }

        template <class T,
                  std::invoke_result_t<T, Type &> (T::*Func)(Type &) const = &T::operator()>
        static constexpr Type invoke_func_operator(Type arg)
        {
            static_assert(sizeof(T) <= 1, "Lambda given captures values");
            (static_cast<T *>(nullptr)->*Func)(arg);
            return arg;
        }

    public:

        template <char Op, Type NewValue, Type Value = value()>
        static constexpr Type op(Type r = set<calc<Op, NewValue, Value>()>())
        {
            return r;
        }

        template <class T, Type Value = value()>
        static constexpr Type apply(const T &,
                                    Type r = set<invoke_func_operator<T>(Value)>())
        {
            return r;
        }

        template <int Index = counter::value()>
        static constexpr int counter_value()
        {
            return Index;
        }
    };
}
