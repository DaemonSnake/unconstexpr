# unconstexpr
a c++17 header library that implements variant constexpr functions and types.

### compile time counter
```c++
#include "inc/meta_counter.hpp"
using namespace unconstexpr;

int main()
{
    using counter = meta_counter<void>;
    constexpr int i = counter::value(); // 0
    counter::next(); // 1
    static_assert(i != counter::value(), "Will not fire");
}
```
### compile time variant variable
```c++
#include "inc/meta_var.hpp"
using namespace unconstexpr;
//println see main.cpp

int main()
{
    using var = meta_var<int>;
    
    println(var::value()); //0
    var::set<5>();
    println(var::value()); //5
    var::op<'+', 5>();
    println(var::value()); //10
    var::op<'*', 2>();
    println(var::value()); //20
    var::apply([](int i) {
            return i * i;
        });
    println(var::value()); //20²
}
```

## compile time variant type list
```c++
#include "inc/meta_list.hpp"
using namespace unconstexpr;
//printType see main.cpp

int main()
{
    using list = meta_tlist<>;

    printType(list::current_type<>); //type_list<>
    list::push_front<int>();
    list::push_front<double>();
    printType(list::current_type<>); //type_list<int, double>
    list::remove<0>();
    printType(list::item<0>); //double
}
```
