// clang-format off
// RUN: %t
// clang-format on

#include "containers/Vector.h"

int main() {
  Vector<int> A{{1, 2, 3, 4, 5}}, B{};
  //  for (int i = 0; i < 5; i++)
  //    A.push_back(i+1);
  for (int i = 0; i < A.size(); i++)
    CHECK_EQ(A[i], i + 1);
  CHECK(B.empty());
  CHECK_EQ(B.size(), 0);
  CHECK_NOT(A.empty());
  CHECK_EQ(A.size(), 5);

  B.assign(A.begin() + 1, A.end());
  for (int i = 0; i < B.size(); i++)
    CHECK_EQ(B[i], i + 2);
  CHECK_NOT(B.empty());
  CHECK_EQ(B.size(), 4);

  A.insert(A.begin(), B.begin(), B.end());
  CHECK_EQ(A.size(), 9);

  auto aIt = A.insert(A.end(), B.begin(), B.end());
  CHECK_EQ(A.size(), 13);
  CHECK_EQ(aIt, &A[9]);

  auto C = A;
  CHECK_EQ(A.size(), C.size());
  CHECK(A == C);
  CHECK_NE(A.data(), C.data());

  C = Vector<int>({2, 3, 4, 5, 6});
  CHECK_EQ(C.size(), 5);
  for (int i = 0; i < C.size(); i++)
    CHECK_EQ(C[i], i + 2);

  C = B;
  CHECK_EQ(C.size(), B.size());
}