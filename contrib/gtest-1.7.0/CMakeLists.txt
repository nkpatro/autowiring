add_library(
  gtest
  fused-src/gtest/gtest-all.cc
)
target_include_directories(
  gtest
  PUBLIC
  fused-src
)
