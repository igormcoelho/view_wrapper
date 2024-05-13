// SPDX-License-Identifier:  MIT
// Copyright (C) 2024 - https://github.com/igormcoelho/view_wrapper

#ifdef MAKE
#include <catch2/catch.hpp>
#else
#include <catch2/catch_test_macros.hpp>
#endif
//
#include <view_wrapper/View.hpp>

TEST_CASE("Teste1") {
  int x = 10;
  REQUIRE(x == 12);
}
