# unconstexpr
#### An ODR violation story
a c++17 header library that implements variant constexpr functions and types.  


__Disclaimer:__  
 * This project only targets G++ and is only for fun.  
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
### compile time variant
```c++
#include "inc/meta_any.hpp"
using namespace unconstexpr;

int main()
{
    meta_any<int> var;
    var = 35;
    println(*var); //35
    var = 3.14;
    println(*var); //3.14
    printType(decltype(*var)); // double&
    var = "word"s;
    println(*var); //word
    printType(decltype(*var)); // std::string&
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
#include "inc/meta_tlist.hpp"
using namespace unconstexpr;

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

## compile time value type list
```c++
#include "inc/meta_vlist.hpp"
using namespace unconstexpr;

int main()
{
    using list = meta_vlist<>;
    
    printType(list::current_type<>); //var_list<>
    list::push_front<42>();
    list::push_front<nullptr>();
    printType(list::current_type<>); //var_list<42, nullptr>
    list::remove<0>();
    printType(list::item<0>); //nullptr
}
```
