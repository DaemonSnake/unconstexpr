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

#include <utility>

namespace unconstexpr
{
    namespace tools
    {
        template <class T, class... Args,
                  std::invoke_result_t<T, Args...> (T::*Func)(Args...) const = &T::operator()>
        static constexpr auto invoke_func_operator(Args... args)
        {
            static_assert(sizeof(T) <= 1, "Type given holds require instance");
            return (static_cast<T *>(nullptr)->*Func)(args...);
        }

        template <class T, class... Args>
        static constexpr auto invoke_lambda_type(T, Args... args)
        {
            static_assert(sizeof(T) <= 1, "Lambda given captures values");
            return invoke_func_operator<T>(std::forward<Args...>(args...));
        }
    }
}
