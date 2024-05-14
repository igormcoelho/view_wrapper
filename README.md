# view_wrapper
A reasonably safe header-only C++20 library for view and range containers in C++

## Do I need this library?

This library is about **views**, which are immutable and lightweight value-types with *reference-type semantics* (behaving *like* a reference),
that allow *remote* manipulation of heavier objects.

Several advances in C++ language, including [C++20 ranges library](https://en.cppreference.com/w/cpp/ranges), 
have drastically improved the efficiency of previous iterative processes.
However, it's tricky for newcomers to understand and to correctly use these
features.

For example:

-  `std::string_view` represents an immutable view to a `std::string`.
Can we write into its elements? No, it's read only. 
Can we build one based on a dangling value and return it on a method? 
Yes, unfortunately, and this will generate issues that are hard to trace.
- `std::span` is a view to many range-based types, including arrays, vectors, etc.
Can we write into its elements? Yes, differently from a `string_view`, although we cannot *insert* new elements in the middle (that's why we developed [subvector.hpp](./include/view_wrapper/subvector.hpp))

These two major classes already cause some confusions, 
and when we think on dozens of range and iterator implementations... 
things get *really confusing*.

We aim at making things like this directly clear to user:

- What is a `View<std::string> v`? Object `*v` behaves as a read-only (const) `std::string_view`.
- What is a `View<std::vector> v`? Object `*v` behaves as a read-only (const) `std::span`.

The proposed library makes all views read-only and it's an extensible library
based on template specialization, so feel free to add any other specialization you like!

It also covers read-write range types. Example:

- What is a `Range<std::vector> v`? Object `*v` is a `subvector` type, that allows remote changes to occur on the original vector (including insertions, differently from `std::span`...)

## Innovations and definitions

This library assumes the following:

- Views are immutable and read-only
- Views are nullable
   * this requirement was put in order to allow them to be compatible with stl container, e.g., `std::movable` and `std::copyable`
- Ranges are read-write and immutable on its *origins* (cannot change the *reference to the pointed/remote object* but can change *content* within the range)
- classes `View<>` and `Range<>` are namespaced on `view_wrapper` and are CamelCase just to prevent name confusions with similar stl classes
- classes do not accept possibly dangling `const T&` types (only `T&`... it can be annoying, but it's better than just crashing due to simple mistakes)
- We created a `subvector` class to represent read-write situations for `std::vector`
    * The `subvector` allows fixed or dynamic bounds to reflect write changes on *origin*
    * Dynamic bounds can be automatically executed before `size()` and push/pop write methods via constructor flags *refreshOnSize* and *refreshBeforePushPop*; or manually invoking `refresh()`
    * `subvector` is C++14 compatible, but works best with C++20 to allow `std::span` and range concepts
- This library is header-only: just copy it into your project!
- This project requires C++20 standard, so for C++17 alternative for simpler situations, see [Optional View project](https://github.com/igormcoelho/optional_view)

## Demo

Just play with `View<std::string>`, `View<std::vector<int>>` and `Range<std::vector<int>>`!

See `src/demo.cpp` and files [include/view_wrapper/View.hpp](./include/view_wrapper/View.hpp) and [include/view_wrapper/Range.hpp](./include/view_wrapper/Range.hpp).

### subvector example
Remember to play with subvector: [include/view_wrapper/subvector.hpp](./include/view_wrapper/subvector.hpp)

In this example, consider a printing function `void printv(subvector<int> v)`:

```
void printv(subvector<int> v) {
  std::cout << "size=" << v.size() << ": ";
  for (auto& x : v) std::cout << x << " ";
  std::cout << std::endl;
}
```

Consider a vector with 6 elements: `std::vector<int> v = {1, 2, -1, 4, 5, 6};`

Then, we demonstrate three views:
- `vv1`: whole view on vector `v`
- `vv2`: first two elements of `v`
- `vv3`: all elements after first `-1` element in `v`


```
subvector<int> vv1(v);
printv(vv1);  // size=6: 1 2 -1 4 5 6
subvector<int> vv2(v, 0, 2);
printv(vv2);  // size=2: 1 2
subvector<int> vv3(v, [](const std::vector<int>& v) {
    auto it1 = std::find(v.begin(), v.end(), -1);
    auto idx1 = std::distance(v.begin(), it1);
    return std::make_pair(idx1 + 1, v.size());
});
printv(vv3);  // size=3: 4 5 6
```

Note how all the views react to the `vv2.push_back(3)`
```
vv2.push_back(3);
printv(vv1);  // size=7: 1 2 3 -1 4 5 6
printv(vv2);  // size=3: 1 2 3
printv(vv3);  // size=3: 4 5 6
```

This demonstrates the fixed and dynamic bounds capabilities of `subvector`.

### building

To build it, just type:

- `make` (for unix systems)
- `cxxbuild . --c++20` (for all systems*)

(*) install [cxxbuild](https://github.com/manydeps/cxxbuild) from `pip install cxxbuild` to automatically run and test with cmake or bazel.

## License

MIT License, 2024

Igor Machado Coelho

