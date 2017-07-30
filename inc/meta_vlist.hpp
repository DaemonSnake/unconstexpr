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

#include "meta_type.hpp"
#include "tools/var_list.hpp"

namespace unconstexpr
{
    namespace detail
    {
        template <auto... Args>
        struct ValueHolder
        {
            constexpr ValueHolder(int) {}

            template <template<class...> class NewHolder>
            static constexpr NewHolder<decltype(Args)...> transfer()
            {
                return {Args...};
            }
        };
    }
    
    template <class Id = void, unsigned = uniq_value::value<> >
    class meta_vlist
    {
        using parent = meta_type<detail::var_list<>, meta_vlist, 0>;

    public:
        template <class Current = typename parent::template type<> >
        using current_type = Current;

        template <auto... NewArgs>
        static constexpr int push_front(int = parent::template change<typename current_type<>::template
                                        push_front<NewArgs...>>()) { return 0; }

        template <auto... NewArgs>
        static constexpr int push_back(int = parent::template change<typename current_type<>::template
                                       push_back<NewArgs...>>()) { return 0; }

        template <size_t N = 1>
        static constexpr int pop_front(int = parent::template change<typename current_type<>::template
                                       pop_front<N>>()) { return 0; }

        template <size_t N = 1>
        static constexpr int pop_back(int = parent::template change<typename current_type<>::template
                                      pop_back<N>>()) { return 0; }

        template <size_t N, auto Ret = current_type<>::template item<N> >
        static constexpr auto item = Ret;

        template <template<auto...> class Holder,
                  class Ret = typename current_type<>::template transfer<Holder> >
        using transfer = Ret;

        template <template<class...> class Holder, int = parent::counter_value()>
        static constexpr auto value_transfer()
        {
            return current_type<>::template transfer<detail::ValueHolder>::
                template transfer<Holder>();
        }
        
        template <class OtherTypeList>
        static constexpr int insert_list(int = parent::template change<typename current_type<>::template
                                         merge<OtherTypeList>>()) { return 0; }

        static constexpr int clear(int = parent::template change<detail::var_list<>>()) { return 0; }

        template <size_t From, size_t Len>
        static constexpr int select(int = parent::template change<typename current_type<>::template
                                    select<From, Len>>()) { return 0; }

        template <size_t Index>
        static constexpr int remove(int = parent::template change<typename current_type<>::template
                                    remove<Index>>()) { return 0; }

        template <int Index = parent::counter_value()>
        static constexpr int counter_value()
        {
            return Index;
        }
    };
}
