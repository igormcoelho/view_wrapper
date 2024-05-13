// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#ifndef VIEW_WRAPPER_SUBVECTOR_HPP_
#define VIEW_WRAPPER_SUBVECTOR_HPP_

// Subvector is a vector-compatible range type in C++
//   => satisfies: std::ranges::contiguous_range
//   => std::ranges::sized_range
//   => std::ranges::random_access_range

#include <cassert>
#include <concepts>
#include <memory>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

namespace view_wrapper {

// What is the advantage of inheriting from:
// std::ranges::view_interface<Subvector<T, A> ?
// nothing special, just to make it 'more range' perhaps...

template <typename T, typename A = std::allocator<T>>
class Subvector : public std::ranges::view_interface<Subvector<T, A>> {
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

 public:
  // full vector
  explicit Subvector(std::vector<T, A>& _remote)
      : remote{&_remote}, idxBegin{0}, idxEnd{_remote.size()} {}

  // range of vector in format [closed, open)
  Subvector(std::vector<T, A>& _remote, size_type _idxBegin, size_type _idxEnd)
      : remote{&_remote}, idxBegin{_idxBegin}, idxEnd{_idxEnd} {
    assert(idxBegin >= 0);
    assert(idxBegin <= idxEnd);
    assert(idxEnd <= remote->size());
  }

  // basic helper: can be removed if necessary...
  std::span<T> to_view() {
    return std::span<T>{remote->begin() + idxBegin, remote->begin() + idxEnd};
  }

  size_type size() const { return idxEnd - idxBegin; }
  bool empty() const { return size() == 0; }

  T& operator[](size_type idx) { return *(remote->begin() + idxBegin + idx); }

  const T& operator[](size_type idx) const {
    return *(remote->begin() + idxBegin + idx);
  }

  // implements iterators from view!!!
  iterator begin() { return remote->begin() + idxBegin; }
  iterator end() { return remote->begin() + idxEnd; }

  void push_back(T&& val) { emplace_back(std::move(val)); }

  void push_back(const T& val) { emplace_back(std::move(val)); }

  template <typename... TArgs>
  auto emplace_back(TArgs&&... args_build) {
    auto it = remote->begin() + idxEnd;
    auto vx = remote->emplace(it, std::forward<TArgs>(args_build)...);
    idxEnd++;
    return vx;
  }

  template <typename... TArgs>
  auto emplace(iterator it, TArgs&&... args_build) {
    idxEnd++;
    it = remote->emplace(it, std::forward<TArgs>(args_build)...);
    return it;
  }

  iterator insert(const_iterator pos, const T& value) {
    auto it = remote->insert(pos, value);
    idxEnd++;
    return it;
  }

  auto erase(iterator it) noexcept {
    it = remote->erase(it);
    idxEnd--;
    return it;
  }

  auto erase(iterator it_start, const iterator& it_end) noexcept {
    const size_t count = std::distance(it_start, it_end);
    idxEnd -= count;
    it_start = remote->erase(it_start, it_end);
    return it_start;
  }

  void pop_back() noexcept {
    remote->erase(remote->begin() + idxEnd);
    idxEnd--;
  }

  // less important

  T& back() noexcept { return operator[](size() - 1); }

  // TODO: cbegin, cend, rbegin, rend, crbegin, crend, ...
};

// basic tests...
static_assert(std::movable<Subvector<int>>);
static_assert(std::copyable<Subvector<int>>);
static_assert(std::ranges::contiguous_range<Subvector<int>>);
static_assert(std::ranges::sized_range<Subvector<int>>);
static_assert(std::ranges::random_access_range<Subvector<int>>);
static_assert(std::ranges::viewable_range<Subvector<int>>);

}  // namespace view_wrapper

#endif  // VIEW_WRAPPER_SUBVECTOR_HPP_