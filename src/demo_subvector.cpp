// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#include <algorithm>
#include <iostream>
//
#include <iterator>
#include <view_wrapper/subvector.hpp>

using view_wrapper::subvector;

void printv(subvector<int> v) {
  std::cout << "size=" << v.size() << ": ";
  for (auto& x : v) std::cout << x << " ";
  std::cout << std::endl;
}

void simple_test() {
  std::vector<int> v = {1, 2, -1, 4, 5, 6};
  //
  // subvector<int> vv(v);
  // vv.push_back(-2);
  // printv(vv);  // size=7: 1 2 -1 4 5 6 -2
  //
  subvector<int> vv0(v, 0, 2);
  printv(vv0);  // size=6: 1 2 -1 4 5 6
  subvector<int> vv1(v);
  printv(vv1);  // sz=6: 1 2 -1 4 5 6
  subvector<int> vv2(v, 0, 2);
  printv(vv2);  // sz=2: 1 2
  subvector<int> vv3(v, [](const std::vector<int>& v) {
    auto it1 = std::find(v.begin(), v.end(), -1);
    auto idx1 = std::distance(v.begin(), it1);
    return std::make_pair(idx1 + 1, v.size());
  });
  printv(vv3);  // sz=3: 4 5 6
  subvector<int> vv4(v, 1, 5);
  printv(vv4);  // size=4: 2 -1 4 5
  //
  vv2.push_back(3);
  printv(vv1);  // sz=7: 1 2 3 -1 4 5 6
  printv(vv2);  // sz=3: 1 2 3
  printv(vv3);  // sz=3: 4 5 6
  printv(vv4);  // size=4: 2 3 -1 4
}

int main() {
  simple_test();
  // ======= vector<subvector> =======

  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<std::vector<int>> vv;
  for (int i = 0; i < 5; i++) vv.push_back(v);  // copy 5 times

  std::cout << "check full bounds" << std::endl;
  {
    std::vector<subvector<int>> vv2;
    for (auto& v : vv) vv2.push_back(subvector<int>(v));
    vv[0].push_back(-1);

    for (auto& v : vv2) std::cout << "size v -> " << v.size() << std::endl;
  }

  return 0;
}
