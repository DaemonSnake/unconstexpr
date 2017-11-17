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
#include <utility>
#include <algorithm>
#include "meta_type.hpp"

namespace unconstexpr
{
    namespace detail
    {
        static constexpr auto false_type_lambda = []() { return std::false_type{}; };

        template<char type, class Prev, class Next>
        constexpr decltype(auto) lambda_op = []()
        {
            decltype(auto) l = ((Prev*)nullptr)->operator()();
            decltype(auto) r = ((Next*)nullptr)->operator()();
            if constexpr (type == '+') return l + r;
            else if constexpr(type == '-') return l - r;
            else if constexpr (type == '*') return l * r;
            else if constexpr (type == '/') return l / r;
            else return l;
        };
    }

    template<unsigned = uniq_value::value<> >
    struct meta_value
    {
        static constexpr meta_value instance = {};
        
        /*back-end:*/
        using lambda_type = meta_type<decltype(detail::false_type_lambda), meta_value, 0>;

        template<class R = typename lambda_type::template type<>>
        static constexpr auto value = ((R*)nullptr)->operator()();

        template<class R = typename lambda_type::template type<>>
        using type = std::decay_t<decltype(value<R>)>;

        /*Tool for undo() & undo_g() */
        template<int C = lambda_type::template counter_value()>
        static constexpr int index_safe_sub(int I) { return std::max(0, C - I); }

        /*Tool for is_valid & compiles()*/
        template<class R = typename lambda_type::template type<>,
                 bool = (((R*)nullptr)->operator()(), true)>
        static constexpr bool ok_tool(int) { return true; }
        static constexpr bool ok_tool(float) { return false; }

        /*front-end:*/
        constexpr meta_value() = default;

        template<class T, int = lambda_type::template change<T>()>
        constexpr meta_value(const T &) {}

        template<class T, int = lambda_type::template change<T>()>
        constexpr const auto &operator=(const T &) const { return *this; }

        template<int I = 1,
                 class R = typename lambda_type::template type<void, index_safe_sub(I)>,
                 int = lambda_type::template change<R>()>
        static constexpr int undo_g() { return 0; }

        template<int I = 1, int = undo_g<I>()>
        constexpr int undo() const { return 0; }

        template<bool R = ok_tool(0)>
        static constexpr bool compiles_g() { return R; }

        template<bool R = ok_tool(0)>
        constexpr bool compiles() const { return R; }

        template<int I = lambda_type::template counter_value(),
            class R = typename lambda_type::template type<void, I> >
        constexpr auto &operator*() const { return value<R>; }

        /*normal operators*/
        template<class T,
                 int I = lambda_type::template counter_value(),
                 class R = typename lambda_type::template type<void, I> >
        constexpr operator T() const { return static_cast<T>(value<R>); }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I> >
        constexpr decltype(auto) operator+(T&& arg) const { return value<R> + arg; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I> >
        constexpr decltype(auto) operator-(T&& arg) const { return value<R> - arg; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I> >
        constexpr decltype(auto) operator*(T&& arg) const { return value<R> * arg; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I> >
        constexpr decltype(auto) operator/(T&& arg) const { return value<R> / arg; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I> >
        constexpr decltype(auto) operator[](T&& arg) const { return value<R>[std::forward<T>(arg)]; }

        template<class... Args, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I> >
        constexpr decltype(auto) operator()(Args&&... args) const { return value<R>(std::forward<Args>(args)...); }

        /*assigment operators*/
        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I>,
                 int = lambda_type::template change<decltype(detail::lambda_op<'+', R, T>)>()>
        constexpr auto &operator+=(T const &) const { return *this; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I>,
                 int = lambda_type::template change<decltype(detail::lambda_op<'-', R, T>)>()>
        constexpr auto &operator-=(T const &) const { return *this; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I>,
                 int = lambda_type::template change<decltype(detail::lambda_op<'*', R, T>)>()>
        constexpr auto &operator*=(T const &) const { return *this; }

        template<class T, int I = lambda_type::template counter_value(), class R = typename lambda_type::template type<void, I>,
                 int = lambda_type::template change<decltype(detail::lambda_op<'/', R, T>)>()>
        constexpr auto &operator/=(T const &) const { return *this; }
    };
}

#define carg(...) ([]{return __VA_ARGS__; })
