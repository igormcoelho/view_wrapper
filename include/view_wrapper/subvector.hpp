// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#ifndef VIEW_WRAPPER_SUBVECTOR_HPP_
#define VIEW_WRAPPER_SUBVECTOR_HPP_

// Subvector is a C++14 vector-compatible range type in C++
//
// It works even better with C++20 concepts and ranges, satisfying:
//   => std::ranges::contiguous_range
//   => std::ranges::sized_range
//   => std::ranges::random_access_range
//   => std::ranges::viewable_range

// #include <cassert>
//
#include <concepts>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)
#include <ranges>
#endif

#if defined(__cpp_lib_span) && (__cpp_lib_span >= 202002L)
#include <span>
#endif

namespace view_wrapper {

// What is the advantage of inheriting from:
// std::ranges::view_interface<Subvector<T, A> ?
// nothing special, just to make it 'more range' perhaps...

// #if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)
// class subvector : public std::ranges::view_interface<subvector<T, A>> {
// #endif

template <typename T, typename A = std::allocator<T>>
class subvector {
 public:
  using value_type = T;
  using allocator_type = A;
  using size_type = typename std::vector<T, A>::size_type;
  using iterator = typename std::vector<T, A>::iterator;
  using const_iterator = typename std::vector<T, A>::const_iterator;

 private:
  // immutable, but nullable
  std::vector<T, A>* remote{nullptr};
  size_type idxBegin, idxEnd;
  // cached bounds
  // std::pair<size_type, size_type> bounds;
  // dynamic bounds
  using fBoundsType =
      std::function<std::pair<size_type, size_type>(const std::vector<T, A>&)>;
  fBoundsType fBounds;
  // === refresh bounds strategies ===
  // 0. must refresh on size() call
  bool refreshOnSize{false};
  // 1. must refresh before push/pop write operation
  //   => middle iterator-based 'emplace', 'insert', 'erase', not included...
  //      otherwise, they would be doubly executed on push_back, pop_front!
  //      another reason is that users can manually invoke 'refresh' if needed!
  bool refreshBeforePushPop{false};

 public:
  // full vector: dynamic bounds [0, size)
  explicit subvector(std::vector<T, A>& _remote)
      : remote{&_remote}, refreshOnSize{true}, refreshBeforePushPop{true} {
    fBounds =
        [](const std::vector<T, A>& vr) -> std::pair<size_type, size_type> {
      return std::make_pair(0, vr.size());
    };
    // invoke dynamic bounds function
    refresh();
  }

  // fixed-range of vector in format [closed, open)
  subvector(std::vector<T, A>& _remote, size_type _idxBegin, size_type _idxEnd)
      : remote{&_remote}, idxBegin{_idxBegin}, idxEnd{_idxEnd} {
    // assert(idxBegin >= 0);
    // assert(idxBegin <= idxEnd);
    // assert(idxEnd <= remote->size());
  }

  // dynamic-range of vector
  subvector(std::vector<T, A>& _remote, fBoundsType _fBounds,
            bool _refreshOnSize = true, bool _refreshBeforePushPop = true)
      : remote{&_remote},
        fBounds{_fBounds},
        refreshOnSize{_refreshOnSize},
        refreshBeforePushPop{_refreshBeforePushPop} {
    // invoke dynamic bounds function
    refresh();
    // assert(idxBegin >= 0);
    // assert(idxBegin <= idxEnd);
    // assert(idxEnd <= remote->size());
  }

  void refresh() const {
    auto p = fBounds(*remote);
    auto& thisConstless = const_cast<subvector<T, A>&>(*this);
    thisConstless.idxBegin = p.first;
    thisConstless.idxEnd = p.second;
  }

#if defined(__cpp_lib_span) && (__cpp_lib_span >= 202002L)
  // basic helper: can be removed if necessary...
  std::span<T> as_span() {
    return std::span<T>{remote->begin() + idxBegin, remote->begin() + idxEnd};
  }
#endif

  std::vector<T, A> as_copy() {
    return std::vector<T, A>(remote->begin() + idxBegin,
                             remote->begin() + idxEnd);
  }

  // slice subvector into [a,b)
  subvector<T, A> slice(size_type a, size_type b) const {
    if (refreshOnSize) refresh();  // just to be extra careful
    subvector<T, A> v2(*remote, idxBegin + a, idxBegin + b);
    return v2;
  }

  size_type size() const {
    if (refreshOnSize) refresh();
    return idxEnd - idxBegin;
  }
  bool empty() const { return size() == 0; }

  T& operator[](size_type idx) { return *(remote->begin() + idxBegin + idx); }

  const T& operator[](size_type idx) const {
    return *(remote->begin() + idxBegin + idx);
  }

  // implements iterators from view!!!
  iterator begin() { return remote->begin() + idxBegin; }
  iterator end() { return remote->begin() + idxEnd; }
  const_iterator begin() const { return remote->begin() + idxBegin; }
  const_iterator end() const { return remote->begin() + idxEnd; }

  void push_back(T&& val) { emplace_back(std::move(val)); }

  void push_back(const T& val) { emplace_back(std::move(val)); }

  template <typename... XArgs>
  auto emplace_back(XArgs&&... args_build) {
    if (refreshBeforePushPop) refresh();
    auto it = remote->begin() + idxEnd;
    idxEnd++;
    return remote->emplace(it, std::forward<XArgs>(args_build)...);
  }

  template <typename... XArgs>
  auto emplace(iterator it, XArgs&&... args_build) {
    idxEnd++;
    return remote->emplace(it, std::forward<XArgs>(args_build)...);
  }

  iterator insert(const_iterator pos, const T& value) {
    idxEnd++;
    return remote->insert(pos, value);
  }

  auto erase(iterator it) noexcept {
    idxEnd--;
    return remote->erase(it);
  }

  auto erase(iterator it_start, const iterator& it_end) noexcept {
    const size_t count = std::distance(it_start, it_end);
    idxEnd -= count;
    return remote->erase(it_start, it_end);
  }

  void pop_back() noexcept {
    if (refreshBeforePushPop) refresh();
    remote->erase(remote->begin() + idxEnd);
    idxEnd--;
  }

  // less important

  T& back() noexcept { return operator[](size() - 1); }

  // TODO: cbegin, cend, rbegin, rend, crbegin, crend, ...
};

// Check if C++20 Concepts is supported
#if defined(__cpp_concepts) && (__cpp_concepts >= 201907L)
static_assert(std::movable<subvector<int>>);
static_assert(std::copyable<subvector<int>>);
static_assert(std::ranges::contiguous_range<subvector<int>>);
static_assert(std::ranges::sized_range<subvector<int>>);
static_assert(std::ranges::random_access_range<subvector<int>>);
static_assert(std::ranges::viewable_range<subvector<int>>);
#endif

}  // namespace view_wrapper

#endif  // VIEW_WRAPPER_SUBVECTOR_HPP_
