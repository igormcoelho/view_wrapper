// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - CrossPP - https://github.com/igormcoelho/crosspp

#include <concepts>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
//
#include <view_wrapper/Range.hpp>
#include <view_wrapper/Subvector.hpp>
#include <view_wrapper/View.hpp>

using view_wrapper::Range;
using view_wrapper::Subvector;
using view_wrapper::View;

int f(View<std::string>& v) {
  // std::cout << v.sv << std::endl;
  auto sv = v.as_view();
  std::cout << "print sv from v=" << &v << std::endl;
  std::cout << sv << std::endl;
  return 1;
}

View<std::string> g(View<std::string>& v) {
  // return View<std::string>(v.as_string());
  // return View<std::string>::from(v.as_string());
  //
  // return View<std::string>(v.as_view());
  return v;  // ERROR, unless std::copyable
}

int main(int argc, char* argv[]) {
  //

  std::string realstr("101");
  View<std::string> v{realstr};
  // auto v = View<std::string>::from(std::string("10"));

  f(v);
  std::cout << "g()" << std::endl;

  std::cout << "will build vg" << std::endl;
  View<std::string> vg{g(v)};
  std::cout << "will print vg=" << &vg << std::endl;
  std::cout << vg.as_view() << std::endl;

  std::vector<View<std::string>> vx;
  std::string str1 = "Hello";
  vx.emplace_back(vg);
  vx.emplace_back(vg);
  vx.emplace_back(vg);
  std::cout << "print list:" << std::endl;
  for (auto& x : vx) std::cout << *x << std::endl;
  //

  std::string_view sv1("abc");
  std::string_view sv2("abc2");
  sv1 = sv2;

  std::cout << sv1 << std::endl;
  std::cout << sv2 << std::endl;
  std::cout << "oi" << std::endl;

  //

  std::vector<float> values = {1.0, 2.0, 3.0, 4.0, 5.2, 6.0, 7.0, 8.0, 9.0};
  auto foo = std::views::filter(values, [](float v) { return v < 5.f; });

  for (auto& x : foo) std::cout << x << std::endl;

  //
  {
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto pos5 = std::ranges::find(v, 5);
    auto countedView = std::views::counted(pos5, 5);
    // outputs 5 6 7 8 9
    for (auto e : countedView) {
      std::cout << e << ' ';
    }
    std::cout << '\n';

    // You can pass the range directly if it supports input_or_output_iterator,
    // in which case, the count starts from the first element
    const char chars[] = {'H', 'i', ' ', 't', 'h', 'e', 'r', 'e'};
    for (char c : std::views::counted(chars, 2)) {
      std::cout << c;  // outputs Hi
    }
  }
  //

  {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::span<int> sp(v.begin() + 1, v.begin() + 3);
    View<std::vector<int>> vx1(v);
    View<std::vector<int>> vx2(sp);
    //

    std::cout << "print list:" << std::endl;
    for (auto& x : *vx1) std::cout << x << std::endl;

    std::cout << "print list:" << std::endl;
    for (auto& x : *vx2) std::cout << x << std::endl;

    sp[1] = 10;

    std::cout << "print list:" << std::endl;
    for (auto& x : *vx1) std::cout << x << std::endl;

    std::cout << "print list:" << std::endl;
    for (auto& x : *vx2) std::cout << x << std::endl;
  }

  std::string_view vv("oi");

  std::cout << vv << std::endl;

  // ============= Subvector

  {
    std::cout << "trying subvector..." << std::endl;

    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::cout << "print list: sz=" << v.size() << std::endl;
    for (auto& x : v) std::cout << x << std::endl;

    Subvector<int> subv1(v);
    assert(subv1.size() == v.size());
    std::cout << "print list: sz=" << subv1.size() << std::endl;
    for (auto& x : subv1) std::cout << x << std::endl;

    Subvector<int> subv2(v, 2, v.size());
    assert(subv2.size() == v.size() - 2);
    std::cout << "print list: sz=" << subv2.size() << std::endl;
    for (auto& x : subv2) std::cout << x << std::endl;

    Subvector<int> subv3(v, 2, 3);
    assert(subv3.size() == 1);
    std::cout << "print list: sz=" << subv3.size() << std::endl;
    for (auto& x : subv3) std::cout << x << std::endl;

    // Subvector<int> subv4(v, 2, v.size() + 1);
    // ERROR!

    subv3.push_back(30);
    assert(subv1.size() == v.size() - 1);

    std::cout << "print list: sz=" << v.size() << std::endl;
    for (auto& x : v) std::cout << x << std::endl;

    std::cout << "print list: sz=" << subv3.size() << std::endl;
    assert(subv3.size() == 2);
    for (auto& x : subv3) std::cout << x << std::endl;

    std::cout << "add -1 on begin" << std::endl;
    subv1.insert(subv1.begin(), -1);
    assert(subv1.size() == v.size() - 1);

    std::cout << "print list: sz=" << v.size() << std::endl;
    for (auto& x : v) std::cout << x << std::endl;

    std::cout << "print list: sz=" << subv1.size() << std::endl;
    for (auto& x : subv1) std::cout << x << std::endl;
    assert(subv1.size() == v.size() - 1);

    std::cout << "print list: sz=" << subv3.size() << std::endl;
    for (auto& x : subv3) std::cout << x << std::endl;
    assert(subv3.size() == 2);
    assert(subv3[0] == 2);
    assert(subv3[1] == 3);

    // remove all from middle
    subv3.pop_back();
    subv3.pop_back();
    assert(subv3.size() == 0);

    std::cout << "print list: sz=" << v.size() << std::endl;
    for (auto& x : v) std::cout << x << std::endl;

    std::cout << "print list: sz=" << subv1.size() << std::endl;
    // subv1 is invalid now!
    std::cout << "v1 is invalid! do not use!" << std::endl;

    std::cout << "print list: sz=" << subv3.size() << std::endl;
    for (auto& x : subv3) std::cout << x << std::endl;
  }

  // test Range

  {
    std::cout << "testing Range" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::cout << "print list: sz=" << v.size() << std::endl;
    for (auto& x : v) std::cout << x << std::endl;

    Range<std::vector<int>> subv1(v);
    assert(subv1->size() == v.size());
    std::cout << "print list: sz=" << subv1->size() << std::endl;
    for (auto& x : *subv1) std::cout << x << std::endl;

    // to_span
    std::cout << "print list: SPAN sz=" << subv1->to_view().size() << std::endl;
    for (auto& x : subv1->to_view()) std::cout << x << std::endl;
  }

  return 0;
}
