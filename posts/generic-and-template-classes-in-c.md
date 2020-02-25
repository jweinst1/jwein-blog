# Generic and Template Classes in C

Generics are syntax components of a programming language that can be reused for different types of objects.
Typically, generics take the form classes or functions, which take type(s) as a parameter. Generics are also
commonly referred to as `templates`, and in C++ are officially called templates. Typically, programming 
languages which lack generics or templates have received criticism. Having the ability to share procedural
code between types greatly improves quality and development. This article describes a few
techniques and methods to achieve template-like functionality in pure `C`.

## What is a type?

Generics and templates allow types to use the same code. But, what really constitutes a "type" in a language 
like C? Unlike other object-oriented programming languages, C does not have a native concept of construction and 
destruction of objects. There is also no native concept of inheritance built into the language. C views types 
as sections of memory with a particular known format of their data. Officially, types are broken down into two 
categories, *scalar* and *aggregate* types.

A scalar type holds only one data item, such as `int` or `char*`. These are most often integer or pointer types. Aggregate
types hold one or more data values, such as any array or struct. The type `void*` represents a special
pointer type that can point to any type, scalar or aggregate. In fact, `void*` can sometimes be used as a form of
generic code. Quite a few functions in `string.h` use `void*` for reading and writing memory chunks of different
sizes.

Therefore, a type in C must have:

* A known size
* A known format

Two types may have the same size, but not the same format. Such as, on most platforms, `float` and `int`. They usually
have the same size, and as such, `sizeof(int) == sizeof(float)` holds true. However, they have an entirely different format,
and thus will behave differently when called with particular functions.

## Static Templates

The first type of template that will be discussed are static templates. Static templates are created at compile time and
do not perform runtime checks on sizes, because they shift that responsibility to the compiler. Static templates used in C
are similar to the templates from the C++ language, because they depend on the actual type members, such as in the case of a
`struct`. In C, the only native means of creating static templates are through the use of macros.

*Note: The use of the word 'static' only means 'at compile time'. It is not related to the C keyword `static`, which means a function has internal linkage*

To begin, let's observe the simplest form of a static template, a definition macro:

```c
#define DEF(type, name, val) type name = val
DEF(int, foo, 5);
// Then used as normal
printf("%d", foo);
```

The `DEF` macro described above can work with any type which can be initialized via the `=` operator. It could also work
by doing:

```c
DEF(const char*, txt, "foo");
```

This template allows the code needed to define variables with basic type to be *generalized* and *abstracted*. The goal is
to make code sharable and similar between types. However, this example is trivial in the sense it provides very little simplification
over just writing out the statement. The real power comes from code that performs more complex tasks.

### Foreach

In C, `for` loops can result in a lot of code being very type specific, leading to redundant code, and huge source files. With the help of
static templates in the form of macros, we can make for looping much smoother in C. First, consider a standard for loop:

```c
for (int i = 0; i < 10; i++) {
    printf("%d", i);
    // ... //
}
```

There are four distinct syntax components. The initialization of the variable, the termination condition, the
incremental step, and finally the actual block of code to be executed upon each increment in the loop. Given those four elements,
we can construct a macro which acts as a `foreach` loop.

```c
#define FOREACH(type, start, end, fn) \
                for (type _foreach_var = start; _foreach_var != end; _foreach_var++) { \
                      fn(_foreach_var); \
                }
```

This template works for primitive type *ranges*, where there is a known type, a start value, an end value, and a desired
function to be applied for each member of the range. The name `_foreach_var` is chosen to purposefully decrease the possibility of
collision with one of the macro's arguments. Do note, `fn` *does not* have to be a function. You could, in this case, pass in
a macro for `fn`, which is also called a higher order macro:

```c
#define PRINT_INT(n) printf("%d", n)
// use FOREACH
FOREACH(int, 0, 5, PRINT_INT)
```

For loop templates can also be used on loops that *iterate* over the contents of an aggregate type, such as an array. Using
aggregate types can sometimes be even more straight forward with macros, because we can derive more information from an aggregate type
than a scalar type. Using the `sizeof` operator, we can determine the number of elements in an array. 

```c
long nums[4];
const size_t nums_count = sizeof(nums) / sizeof(nums[0]);
printf("%zu\n", nums_count);
// 4
```

The `sizeof` operator always takes the size of a `value` in bytes. This means passing an array will evaluate to the
total size of that array. This does not work with pointer types, as the size of any pointer will always be the same as 
`sizeof(void*)`. This information that can be inferred from an array can be used to make a powerful "foreach" template:

```c
#define FOREACH(arr, fn) \
            for (size_t _ind = 0; _ind < sizeof(arr) / sizeof(arr[0]); _ind++) { \
                fn(_ind[i]); \
            }
```

In the `FOREACH` macro above, the only two arguments required are the array itself, `arr`, and some callable to apply for each
element in `arr`.



