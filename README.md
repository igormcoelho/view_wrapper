# view_wrapper
A reasonably safe C++20 library for view and range containers in C++

## What do I need this library?

This library is about **views**, which are **immutable** and lightweight types,
that allow *remote* manipulation of heavier objects.

Several advances in C++ language, including C++20 ranges library, 
have drastically improved the efficiency of previous iterative processes.
However, it's tricky for newcomers to understand and to correctly use these
features.

For example:

-  `std::string_view` represents an immutable view to a `std::string`.
Can we write into its elements? No, it's read only. 
Can we build one based on a dangling value and return it on a method? 
Yes, unfortunately, and this will generate issues that are hard to trace.
- `std::span` is a view to many range-based types, including arrays, vectors, etc.
Can we write into its elements? Yes, differently from a `string_view`.

These two major classes already cause some confusions, 
and when we think on dozens of range and iterator implementations... 
things get *really confusing*.

We aim at making things like this directly clear to user:

- What is a `View<std::string> v`? Object `*v` behaves as a read-only (const) `std::string_view`.
- What is a `View<std::vector> v`? Object `*v` behaves as a read-only (const) `std::span`.

The proposed library makes all views read-only and it's an extensible library
based on template specialization, so feel free to add any other specialization you like!

It also covers read-write range types. Example:

- What is a `Range<std::vector> v`? Object `*v` is a `subvector` type, that allows remote changes to occur on the original vector.

## Innovations and definitions

This library assumes the following:

- Views are immutable and read-only
- Views are nullable
   * this requirement was put in order to allow them to be compatible with stl container, e.g., `std::movable` and `std::copyable`
- Ranges are read-write and immutable on its *origins* (cannot change the *reference to the pointed/remote object*)
- classes `View<>`, `Range<>` and `Subvector<>` are namespaced on `view_wrapper` and are CamelCase just to prevent name confusions with similar stl classes
- We created a `subvector` class to represent read-write situations for `std::vector`
- This project requires C++20 standard, so for C++17 alternative for simpler situations, see [Optional View project](https://github.com/igormcoelho/optional_view)

## Demo

See `src/demo.cpp` and files `include/view_wrapper/View.hpp` and `include/view_wrapper/Subvector.hpp` 

To build it, just type:

- `make` (for unix systems)
- `cxxbuild . --c++20` (for all systems*)

(*) install [cxxbuild](https://github.com/manydeps/cxxbuild) from `pip install cxxbuild` to automatically run and test with cmake or bazel.

## License

MIT License, 2024

Igor Machado Coelho

