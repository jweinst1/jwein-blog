# Using Templates and Generics in C

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

The main type of templates that can be implemented in C are static templates. Static templates are created at compile time and
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
element in `arr`. The `size_t` type can be used to access any array element. We don't need to also pass in the type of
`arr`, because we don't need it, we can let `fn` handle it.

### Functions

Static templates can also be implemented in C to embody entire functions. This approach to generics and templating is needed
when the desired behavior is more complex than what can be captured in a loop-oriented template. Typically in C, 
parameters taking the form of multiple different types will be given the type `void*`, which another `size_t` parameter
representing the size of the memory the `void*` points to. This style of function definiton is used in the standard library,
such as with the functions `memcpy` and `memset`.

An issue with using `void*` is the lack of type checking. Any address of any type in C can be stored in `void*`, such as

```c
int s = 3;
char mes[50];
long b = 6754333;

void* ps = &s;
void* pmes = mes;
void* pb = &b;
```

Those are all valid definitions. There is no way for the compiler to interpret an incorrect type is being passed to a `void*`
intended for a different type. Therefore, functional templates provide a way to limit the scope of types
accepted into generic functions. For a start, let's look at an example of comparing two aggregate types by a particular
field's value for equality:

```c

#define FIELD_EQ(type, field) \
         int FIELD_EQ_##type(const type* _obj1, const type* _obj2) \
         {  \
            return _obj1->field == _obj2->field; \
         }
```

The above template is a macro that produces a function definition everytime it is called. This function takes two 
`const` pointers of the specified type, and returns the result of comparing `field`. The name of the function produced
has the name of the type appended to the end of it. We need each definition of this equality template function to have it's own name, 
because C does not allow functions to have the same name yet conflicting types. This means that, with this template, doing

```c
FIELD_EQ(point_t, x)
FIELD_EQ(foo_t, x)
```

Would actually produce `FIELD_EQ_point_t` and `FIELD_EQ_foo_t` respectively. The advantage here is, we can create the same function
for multiple types without explicitly writing out those functions multiple times. Here is an example of bothing using the functional 
template and calling the produced function:

```c
typedef struct {
  int x;
  int y;
} point_t;

FIELD_EQ(point_t, x)

int main(void) {
  point_t f1 = {3, 5};
  point_t f2 = {3, 3};
  printf("result is %d\n", FIELD_EQ_point_t(&f1, &f2));
  return 0;
}
```

This pattern of function production can be used to create identical C APIs between different types.

### Structures

In C, structures, called "structs" for short, are aggregate types that can contain heterogenous, named fields of data.
In comparison to other languages, a `struct` is like an object with no methods, no constructor, and no destructor. Structs are chunks
of formatted data composed of multiple scalar types. Structs are different than objects in other languages because there is no
builtin way of achieving polymorphism. Nor is there a native way to achieve inheritance, such as in the desired case of a 
parent struct and child struct.

Interpreted programming languages, specifically Python, use an object "head" approach. This means that, any struct that is considered
an "object" of the language, has a special collection of fields in the beginning of it's definition, that hold information
related to the structs idenity. This allows all of the structs to be casted to a common base `struct` type, as long as
they carry these fields before any other specific fields are defined. The Python C API accomplishes this with a macro called
`PyObject_HEAD`. This macro carries fields that identify the type of the struct, it's size, and more.

A more basic example would look like the following:

```c
typedef unsigned obj_type_t
#define OBJECT_TYPE_INT 1

#define OBJECT_HEAD obj_type_t type;
// Base struct 
typedef struct { 
   OBJECT_HEAD;
} obj_t;

// child struct
typedef struct {
   OBJECT_HEAD;
   int val;
} obj_int_t;

int obj_int_check(const obj_t* obj)
{
    return obj->type == OBJECT_TYPE_INT;
}

int main(void) {
   obj_int_t i = { OBJECT_TYPE_INT, 4};
   return !obj_int_check((obj_t*)(&i));
}
``` 

In the above, `obj_t` is the base `struct` of all other `obj_*_t` related structs. It is defined purely with the macro
`OBJECT_HEAD`, holding the type information about the struct. This macro is a template, any other struct with `OBJECT_HEAD`
as part of it's definition will be able to be cast to `obj_t`. This can be seen in the `struct` type `obj_int_t`. The creation of struct
types which can be cast to `obj_t` can be simplfied with a higher order macro template:

```c
#define OBJECT_START(name) \
            struct name { \
            OBJECT_HEAD;
#define OBJECT_END(name) }; \
             typedef struct name name
```

These two macros, `OBJECT_START` and `OBJECT_END` allow more standard type definitions of template structs. They guarantee any
defining statements between the start and end macro will form a `obj_t` compatible type. More so, it ensures the name of the type will exist
under the `struct <name>` and `<name>` namespaces.

## Conclusion

Overall, implementing templates in C can make C code more readable, less redundant, and less prone to errors. It allows effecient development
without the need to incorporate or switch to C++ or languages with builtin template systems, if desired. Using generics and templates in C can also make programs
more type safe, and prevent improper access of memory.
