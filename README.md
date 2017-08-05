# unconstexpr

#### An ODR violation story
a c++17 header library that implements variant constexpr functions and types.

### Test online now with Coliru online g++ compiler
http://coliru.stacked-crooked.com/a/1fc479c704674632

__Disclaimer:__  
 * This project only targets __g++(7+)__ and is only for fun.  
 * It is inspired by Filip Roséen's blog http://b.atch.se/.  
 * His blog is great go check it out for a c++ standart compilant implementation of some of the features of this library.  

### template instanciation uniqueness
All meta_X templated types provided by this library have unique template instanciation.
```c++
static_assert(!std::is_same_v<meta_X<>, meta_X<>>, "Will not fire");
```

### compile time counter
```c++
#include "inc/meta_counter.hpp"
using namespace unconstexpr;

int main()
{
    using counter = meta_counter<>;
    constexpr int i = counter::value(); // 0
    counter::next(); // 1
    static_assert(i != counter::value(), "Will not fire");
}
```
### compile time type-safe any
```c++
#include "inc/meta_any.hpp"
using namespace unconstexpr;

int main()
{
    meta_any<int> var;
    var = 35;
    println(*var); //35
    var = 3.14;
    type_printer<decltype(*var)>(); //double&
    println(*var); //3.14
    var = "word"s;
    type_printer<decltype(*var)>(); //std::string&
    println(*var); //word
    std::cout << var << std::endl; //provides an operator<<
}
```
### compile time variant variable
```c++
#include "inc/meta_var.hpp"
using namespace unconstexpr;

int main()
{
    using var = meta_var<int>;
    
    static_assert(var::value() == 0);
    var::set<5>();
    static_assert(var::value() == 5);
    var::op<'+', 5>();
    static_assert(var::value() == 10);
    var::op<'*', 2>();
    static_assert(var::value() == 20);
    var::apply([](int i) {
            return i * i;
        });
    static_assert(var::value() == 20*20);
}
```

## compile time variant type list
```c++
#include "inc/meta_tlist.hpp"
using namespace unconstexpr;

int main()
{
    using list = meta_tlist<>;

    type_printer<list::current_type<>>(); //type_list<>
    list::push_front<int>();
    list::push_front<double>();
    type_printer<list::current_type<>>(); //type_list<int, double>
    list::remove<0>();
    type_printer<list::item<0>>(); //double
}
```

## compile time value type list
```c++
#include "inc/meta_vlist.hpp"
using namespace unconstexpr;

int main()
{
    using list = meta_vlist<>;
    
    type_printer<list::current_type<>>(); //var_list<>
    list::push_front<42>();
    list::push_front<nullptr>();
    type_printer<list::current_type<>>(); //var_list<42, nullptr>
    list::remove<0>();
    static_assert(list::item<0> == nullptr);
}
```
