// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#include <iostream>
//
#include <view_wrapper/subvector.hpp>

using view_wrapper::subvector;

int main() {
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
