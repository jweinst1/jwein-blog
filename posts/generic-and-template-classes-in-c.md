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

## Static Templates

The first type of template that will be discussed are static templates. Static templates are created at compile time and
do not perform runtime checks on sizes, because they shift that responsibility to the compiler.

