// clang-format off
// RUN: %t
// clang-format on

#include "containers/CompactHashMap.h"

int main() {
  CompactHashMap<int, int> A{}, B{};
  for (int i = 0; i < 40; i++) {
    A[i] = i;
    B[i] = 0;
  }
  CHECK_NOT(B.empty());
  CHECK_EQ(B.size(), 40);
  CHECK_NOT(A.empty());
  CHECK_EQ(A.size(), 40);
  for (int i = 0; i < A.size(); i++)
    CHECK_EQ(A[i], i);
  for (auto &b : B)
    CHECK_EQ(b.second, 0);
}