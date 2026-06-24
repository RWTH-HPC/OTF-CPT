// clang-format off
// RUN: %t
// clang-format on

#include "containers/String.h"

int main() {
  String A{"abcde"}, B{};
  for (int i = 0; i < A.size(); i++)
    CHECK_EQ(A[i], 'a' + i);
  CHECK(B.empty());
  CHECK_EQ(B.size(), 0);
  CHECK_EQ(B.length(), 0);
  CHECK_NOT(A.empty());
  CHECK_EQ(A.size(), 5);
  CHECK_EQ(A.length(), 5);

  B.assign(A.begin() + 1, A.end());
  for (int i = 0; i < B.size(); i++)
    CHECK_EQ(B[i], 'b' + i);
  CHECK_NOT(B.empty());
  CHECK_EQ(B.size(), 4);

  A.insert(0, B);
  CHECK_EQ(A.size(), 9);

  auto C = A;
  CHECK_EQ(A.size(), C.size());
  CHECK(A == C);
  CHECK_NE(A.c_str(), C.c_str());

  C = String("cdefg");
  CHECK_EQ(C.size(), 5);
  for (int i = 0; i < C.size(); i++)
    CHECK_EQ(C[i], 'c' + i);

  C = B;
  CHECK_EQ(C.size(), B.size());

  auto D = A.substr(2, 3);
  CHECK(D == "dea");
}
