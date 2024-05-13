// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#ifndef VIEW_WRAPPER_RANGE_HPP_
#define VIEW_WRAPPER_RANGE_HPP_

// Range<> is a C++20 wrapper for safer use of range types in C++

#include <optional>
#include <utility>
#include <vector>
//
#include "./subvector.hpp"

namespace view_wrapper {

template <typename Self>
concept IsRange = requires(Self s) {
  { s.as_copy() };
  { s.as_range() };
  typename Self::value_type;
  typename Self::range_type;
};

template <typename T>
class Range;

template <typename X>
class Range<std::vector<X>>
    : public std::ranges::view_interface<Range<std::vector<X>>> {
 private:
  std::optional<subvector<X>> sv;

 public:
  using value_type = std::vector<X>;
  using range_type = subvector<X>;

  // no copy (perhaps?)
  // View(const View& v) = delete;

  Range(const Range& v) : sv{v.sv} {}

  // move needed for std::movable
  Range(Range&& v) : sv{std::move(v.sv)} {}

  // private:
  // explicit View(const std::string& s) : std::string_view{s} {
  // DO NOT ACCEPT 'const string&' HERE! IT MAY DANGLE!
  explicit Range(std::vector<X>& s) : sv{s} {}

  explicit Range(subvector<X>& s) : sv{s} {}

  auto begin() const { return sv->begin(); }
  auto end() const { return sv->end(); }

  subvector<X>& as_range() { return *sv; }

  std::vector<X> as_copy() { return std::vector<X>(*sv); }

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

static_assert(IsRange<Range<std::vector<int>>>);
static_assert(std::ranges::viewable_range<Range<std::vector<int>>>);

}  // namespace view_wrapper

#endif  // VIEW_WRAPPER_RANGE_HPP_
