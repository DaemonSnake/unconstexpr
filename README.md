# unconstexpr

#### An ODR violation story
a c++17 header library that implements variant constexpr functions and types.

### Test it online now with Coliru online g++ compiler
<a href="http://coliru.stacked-crooked.com/a/1fc479c704674632">
<img src="https://img4.hostingpics.net/pics/733552layout.png"/>
</a>

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
#include "unconstexpr/meta_counter.hpp"
using namespace unconstexpr;

int main()
{
    using counter = meta_counter<>;
    constexpr int i = counter::value(); // 0
    counter::next(); // 1
    static_assert(i != counter::value(), "Will not fire");
}
```
### compile time type-safe any (value not constexpr)
```c++
#include "unconstexpr/meta_any.hpp"
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
### constexpr typesafe any : meta_value
```c++
#include "unconstexpr/meta_value.hpp"
using namespace unconstexpr;

//carg(x) is a macro and is required for every assignation operation
int main()
{
    using type = unconstexpr::meta_value<>;
    static_assert(type::value<> == false);
    static_assert(std::is_same_v<type::type<>, std::false_type>);

    constexpr type tmp = carg(42);
    static_assert(*tmp == 42);
    tmp += carg(3);
    static_assert(*tmp == 45);
    tmp -= carg(42);
    static_assert(*tmp == 3);
    tmp *= carg(3);
    static_assert(*tmp == 9);
    
    static_assert(tmp.compiles());
    tmp /= carg(0);
    if constexpr(!tmp.compiles())
      tmp.undo();
    static_assert(*tmp == 9);

    tmp = carg([]() { return 42; });
    static_assert(tmp() == 42);
    
    tmp = carg(std::array{42, 314});
    static_assert(tmp[1] == 314);
}
```

### partial class definition
```c++
#include "unconstexpr/meta_partial.hpp"
using namespace unconstexpr;

using A = partial_it<>;

template<>
struct partial<A>
{
   int i;
};

template<>
struct partial<A>
{
   int j;
};

int main()
{
   partial_t<A> tmp;
   (void)tmp.i;
   (void)tmp.j;
}

template<>
struct partial<A>
{
   int k;
};

void func()
{
   partial_t<A> tmp;
   (void)tmp.i;
   (void)tmp.j;
   (void)tmp.k;
}
```

## compile time variant type list
```c++
#include "unconstexpr/meta_tlist.hpp"
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
#include "unconstexpr/meta_vlist.hpp"
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
