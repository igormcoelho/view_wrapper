// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#ifndef VIEW_WRAPPER_VIEW_HPP_
#define VIEW_WRAPPER_VIEW_HPP_

// View<> is a wrapper for safer use of view types in C++

#include <concepts>
#include <iostream>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// TODO: inherit from https://en.cppreference.com/w/cpp/ranges/view_interface

namespace view_wrapper {

// By default: View is immutable and not nullable
// (thus, not movable or copyable)
// Maybe a NullableView versions is required for move constructors
// (thus, with Partial immutability) => useful in stl containers!

// =================================================
// View is a partially-immutable view type, as it
// allows nullability.
//
// To allow compability with containers, it implements
// std::movable and std::copiable
// =================================================

template <typename T>
class View;

template <>
class View<std::string> {
 public:
  std::optional<std::string_view> sv;

  // no copy (perhaps?)
  // View(const View& v) = delete;

  View(const View& v) : sv{v.sv} {}

  // move needed for std::movable
  View(View&& v) : sv{std::move(v.sv)} {}

  // private:
  // explicit View(const std::string& s) : std::string_view{s} {
  // DO NOT ACCEPT 'const string&' HERE! IT MAY DANGLE!
  explicit View(std::string& s) : sv{s} {
    // std::cout << "CONSTRUCT View" << std::endl;
  }

  explicit View(std::string_view& s) : sv{s} {
    // std::cout << "CONSTRUCT View" << std::endl;
  }

  void show() { std::cout << "View for strings\n"; }

  const std::string_view& as_view() {
    std::cout << "printing as_string_view from " << this << std::endl;

    // return std::string_view(*this);
    // return std::string_view(sv);
    return *sv;
  }

  std::string as_copy() {
    // return std::string(*this);
    return std::string(*sv);
  }

  // no assign (perhaps?)
  // View<std::string>& operator=(const View<std::string>& other) = delete;
  View& operator=(const View& other) {
    if (this == &other) return *this;
    sv = other.sv;
    return *this;
  }

  // move needed for std::movable
  View& operator=(View&& other) {
    this->sv = std::move(other.sv);
    return *this;
  }

  const std::string_view& operator*() { return as_view(); }
  const std::string_view* operator->() { return &as_view(); }
};

// View should support movable, otherwise it's useless in containers!
// https://en.cppreference.com/w/cpp/memory/construct_at
// complaining here...
// https://en.cppreference.com/w/cpp/container/vector/emplace_back
static_assert(std::movable<View<std::string>>);
static_assert(std::copyable<View<std::string>>);
// static_assert(std::semiregular<View<std::string>>);
// movable => copyable => semiregular => ...
// Not semiregular, as it requires default construction.
// It could support, but is there any case for it? Empty view?
//    => todo: verify if this helps nullability somehow!
// If copyable is too dangerous (TODO investigate!), then
// it could be removed, so we rest with movable only.
//
// Example: string_view is copyable.
static_assert(std::copyable<std::string_view>);

// VECTOR PART!

template <typename X>
class View<std::vector<X>> {
 public:
  std::optional<std::span<X>> sv;

  // no copy (perhaps?)
  // View(const View& v) = delete;

  View(const View& v) : sv{v.sv} {}

  // move needed for std::movable
  View(View&& v) : sv{std::move(v.sv)} {}

  // private:
  // explicit View(const std::string& s) : std::string_view{s} {
  // DO NOT ACCEPT 'const string&' HERE! IT MAY DANGLE!
  explicit View(std::vector<X>& s) : sv{s} {
    // std::cout << "CONSTRUCT View" << std::endl;
  }

  explicit View(std::span<X>& s) : sv{s} {
    // std::cout << "CONSTRUCT View" << std::endl;
  }

  void show() { std::cout << "View for vector\n"; }

  std::span<X>& as_view() {
    std::cout << "printing as_string_view from " << this << std::endl;

    // return std::string_view(*this);
    // return std::string_view(sv);
    return *sv;
  }

  std::vector<X> as_copy() {
    // return std::string(*this);
    return std::vector<X>(*sv);
  }

  // no assign (perhaps?)
  // View<std::string>& operator=(const View<std::string>& other) = delete;
  View& operator=(const View& other) {
    if (this == &other) return *this;
    sv = other.sv;
    return *this;
  }

  // move needed for std::movable
  View& operator=(View&& other) {
    this->sv = std::move(other.sv);
    return *this;
  }

  const std::span<X>& operator*() { return *sv; }
  const std::span<X>* operator->() { return &(*sv); }
};

}  // namespace view_wrapper

#endif  // VIEW_WRAPPER_VIEW_HPP_
