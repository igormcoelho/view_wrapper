// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#ifndef VIEW_WRAPPER_RANGE_HPP_
#define VIEW_WRAPPER_RANGE_HPP_

// Range<> is a wrapper for safer use of range types in C++

#include <concepts>
#include <iostream>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
//
#include "./subvector.hpp"

// TODO: inherit from https://en.cppreference.com/w/cpp/ranges/view_interface

namespace view_wrapper {

template <typename T>
class Range;

template <typename X>
class Range<std::vector<X>> {
 public:
  std::optional<subvector<X>> sv;

  // no copy (perhaps?)
  // View(const View& v) = delete;

  Range(const Range& v) : sv{v.sv} {}

  // move needed for std::movable
  Range(Range&& v) : sv{std::move(v.sv)} {}

  // private:
  // explicit View(const std::string& s) : std::string_view{s} {
  // DO NOT ACCEPT 'const string&' HERE! IT MAY DANGLE!
  explicit Range(std::vector<X>& s) : sv{s} {
    // std::cout << "CONSTRUCT View" << std::endl;
  }

  explicit Range(subvector<X>& s) : sv{s} {
    // std::cout << "CONSTRUCT View" << std::endl;
  }

  void show() { std::cout << "Range for vector\n"; }

  subvector<X>& as_range() {
    std::cout << "printing as_range from " << this << std::endl;

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
  Range& operator=(const Range& other) {
    if (this == &other) return *this;
    sv = other.sv;
    return *this;
  }

  // move needed for std::movable
  Range& operator=(Range&& other) {
    this->sv = std::move(other.sv);
    return *this;
  }

  subvector<X>& operator*() { return *sv; }
  subvector<X>* operator->() { return &(*sv); }
};

}  // namespace view_wrapper

#endif  // VIEW_WRAPPER_RANGE_HPP_
