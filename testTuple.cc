#include <cmath>
#include <gtest/gtest.h>

#include "Tuple.h"

/**
 * Structure utilisée pour tester les comparateurs.
 */
struct TestStruct {
  int i;
  double d;
  TestStruct(const int i, const double d): i(i), d(d) {}

  friend bool operator<(const TestStruct &lhs, const TestStruct &rhs) {
    if (lhs.i < rhs.i)
      return true;
    if (rhs.i < lhs.i)
      return false;
    return lhs.d < rhs.d;
  }

  friend bool operator<=(const TestStruct &lhs, const TestStruct &rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>(const TestStruct &lhs, const TestStruct &rhs) {
    return rhs < lhs;
  }

  friend bool operator>=(const TestStruct &lhs, const TestStruct &rhs) {
    return !(lhs < rhs);
  }

  friend bool operator==(const TestStruct &lhs, const TestStruct &rhs) {
    return lhs.i == rhs.i
           && lhs.d == rhs.d;
  }

  friend bool operator!=(const TestStruct &lhs, const TestStruct &rhs) {
    return !(lhs == rhs);
  }

  TestStruct operator+(const TestStruct &rhs) const {
    return {i + rhs.i, d + rhs.d};
  }

  TestStruct operator-(const TestStruct &rhs) const {
    return {i - rhs.i, d - rhs.d};
  }

  TestStruct& operator+=(const TestStruct &rhs) {
    i += rhs.i;
    d += rhs.d;
    return *this;
  }
};


int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* ------------------ */

/**
 * Test given in the documentation of `tpl::Tuple::plus_impl`.
 */
TEST(decltype, explained) {
  [[maybe_unused]] constexpr int i = 1;
  [[maybe_unused]] constexpr double d = 5.0;
  constexpr bool res = std::is_same_v<decltype(i + d), double>;
  EXPECT_EQ(res, true);
}

TEST(Get, Classic) {
  tpl::Tuple<int, double, std::string> t1(42, 3.14, "Hello World !");
  EXPECT_EQ(t1.get<0>(), 42);
  EXPECT_EQ(t1.get<1>(), 3.14);
  EXPECT_EQ(t1.get<2>(), "Hello World !");
}

TEST(Get, Affectation) {
  auto t1 = tpl::makeTuple(42, 9.4, 3.5f, std::string("Ceci est une phrase"));
  t1.get<0>() = -1;
  t1.get<1>() = -1.0;
  t1.get<2>() = -1.f;
  t1.get<3>() = std::string("-1");

  EXPECT_EQ(t1.get<0>(), -1);
  EXPECT_EQ(t1.get<1>(), -1.0);
  EXPECT_EQ(t1.get<2>(), -1.f);
  EXPECT_EQ(t1.get<3>(), "-1");
}

/**
 * Vérifie que lorsque l'on créer un tuple en const. Les valeurs de Tuple#get<Idx>() sont bien des références constante.
 * Et donc qu'elle ne peuvent pas être modifier.
 */
TEST(Get, Const) {
  const auto t1 = tpl::makeTuple(5, 1.0, std::string("abc"));

  constexpr bool is_const_0 = std::is_same_v<decltype(t1.get<0>()), const int&>;
  EXPECT_EQ(is_const_0, true);


  constexpr bool is_const_1 = std::is_same_v<decltype(t1.get<1>()), const double&>;
  EXPECT_EQ(is_const_1, true);

  constexpr bool is_const_2 = std::is_same_v<decltype(t1.get<2>()), const std::string&>;
  EXPECT_EQ(is_const_2, true);

  EXPECT_EQ(t1.get<0>(), 5);
  EXPECT_EQ(t1.get<1>(), 1.0);
  EXPECT_EQ(t1.get<2>(), "abc");
}

TEST(Comparator, Inferior) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 < t2, true);
  EXPECT_EQ(t2 < t1, false);
  EXPECT_EQ(t3 < t1, false);
}

TEST(Comparator, Inferior_Case_Equals) {
  const auto t1 = tpl::makeTuple(1, 5, 10, std::string("abc"));
  const auto t2 = t1; // Create a copy of t1 to get the exact same tuple. // NOLINT(*-unnecessary-copy-initialization)
  EXPECT_EQ(t1 < t2, false);
}

TEST(ComparatorWithStruct, Inferior) {
  const auto is1 = TestStruct(1, 1.1);
  const auto is2 = TestStruct(2, 2.2);

  const auto t1 = tpl::makeTuple(is1);
  const auto t2 = tpl::makeTuple(is2);

  EXPECT_EQ(t1 < t2, true);
}

TEST(Comparator, Inferior_Case_Null) {
  constexpr auto t1 = tpl::makeTuple();
  constexpr auto t2 = tpl::makeTuple();
  EXPECT_EQ(t1 < t2, false);
}

TEST(Comparator, InferiorOrEquals) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 <= t2, true);
  EXPECT_EQ(t2 <= t1, false);
  EXPECT_EQ(t3 <= t1, true);
}

TEST(ComparatorWithStruct, InferiorOrEqual) {
  const auto is1 = TestStruct(1, 1.1);
  const auto is2 = TestStruct(1, 1.1);

  const auto t1 = tpl::makeTuple(is1);
  const auto t2 = tpl::makeTuple(is2);

  EXPECT_EQ(t1 <= t2, true);
}

TEST(Comparator, Superior) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 > t2, false);
  EXPECT_EQ(t2 > t1, true);
  EXPECT_EQ(t3 > t1, false);
}

TEST(ComparatorWithStruct, Superior) {
  const auto is1 = TestStruct(1, 1.1);
  const auto is2 = TestStruct(2, 2.2);

  const auto t1 = tpl::makeTuple(is1);
  const auto t2 = tpl::makeTuple(is2);

  EXPECT_EQ(t2 > t1, true);
}

TEST(Comparator, SuperiorOrEqual) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);

  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 >= t2, false);
  EXPECT_EQ(t2 >= t1, true);
  EXPECT_EQ(t3 >= t1, true);
}

TEST(ComparatorWithStruct, SuperiorOrEqual) {
  const auto is1 = TestStruct(1, 1.1);
  const auto is2 = TestStruct(2, 2.2);

  const auto t1 = tpl::makeTuple(is1);
  const auto t2 = tpl::makeTuple(is2);

  EXPECT_EQ(t1 >= t2, false);
}


TEST(Operator, Plus) {
  const auto t1 = tpl::makeTuple(1,   1,   0.1, 0.1);
  const auto t2 = tpl::makeTuple(0.1, 0.1, 1,   1);

  const tpl::Tuple t3 = t1 + t2;
  EXPECT_EQ(t3.get<0>(), 1.1);
  EXPECT_EQ(t3.get<1>(), 1.1);
  EXPECT_EQ(t3.get<2>(), 1.1);
  EXPECT_EQ(t3.get<3>(), 1.1);

  tpl::Tuple t4 = t1;
  t4 += t2;
  // Here, the two first values are int and the operator inplace += does not modify the type, so the double 0.1 is truncated to an int 0. That's why (int) 1 + 0.1 is equals to 1 here.
  EXPECT_EQ(t4.get<0>(), 1);
  EXPECT_EQ(t4.get<1>(), 1);
  EXPECT_EQ(t4.get<2>(), 1.1);
  EXPECT_EQ(t4.get<3>(), 1.1);
}

TEST(Operator, PlusString) {
  const auto t1 = tpl::makeTuple(static_cast<std::string>("abc"));
  const auto t2 = tpl::makeTuple(static_cast<std::string>("def"));

  const auto t3 = t1 + t2;
  EXPECT_EQ(t3.get<0>(), "abcdef");
}

TEST(Operator, PlusEqString) {
  auto t1 = tpl::makeTuple(static_cast<std::string>("abc"));
  const auto t2 = tpl::makeTuple(static_cast<std::string>("def"));
  t1 += t2;
  EXPECT_EQ(t1.get<0>(), "abcdef");
}

TEST(OperatorWithStruct, Plus) {
  const auto is1 = TestStruct(1, 1.1);
  const auto is2 = TestStruct(2, 2.2);

  const auto t1 = tpl::makeTuple(is1);
  const auto t2 = tpl::makeTuple(is2);

  const auto t3 = t1 + t2;

  const auto is = t3.get<0>();

  EXPECT_EQ(is.i, 3);
  EXPECT_EQ(std::round(is.d * 10000.0) / 10000.0, 3.3); // Here we round the double to avoid floating point errors (1.1 + 2.2 = 3.300...03)
}

TEST(Operator, Minus) {
  const auto t1 = tpl::makeTuple(1,   1,   0.1, 0.1);
  const auto t2 = tpl::makeTuple(0.1, 0.1, 1,   1);

  const tpl::Tuple t3 = t1 - t2;
  EXPECT_EQ(t3.get<0>(), 0.9);
  EXPECT_EQ(t3.get<1>(), 0.9);
  EXPECT_EQ(t3.get<2>(), -0.9);
  EXPECT_EQ(t3.get<3>(), -0.9);

  tpl::Tuple t4 = t1;
  t4 -= t2;
  EXPECT_EQ(t4.get<0>(), 0);
  EXPECT_EQ(t4.get<1>(), 0);
  EXPECT_EQ(t4.get<2>(), -0.9);
  EXPECT_EQ(t4.get<3>(), -0.9);
}

TEST(Operator, Times) {
  const auto t1 = tpl::makeTuple(10, 10, 0.5, 0.5);
  const auto t2 = tpl::makeTuple(0.5, 0.5, 1,   1);

  const tpl::Tuple t3 = t1 * t2;
  EXPECT_EQ(t3.get<0>(), 5);
  EXPECT_EQ(t3.get<1>(), 5);
  EXPECT_EQ(t3.get<2>(), 0.5);
  EXPECT_EQ(t3.get<3>(), 0.5);

  tpl::Tuple t4 = t1;
  t4 *= t2;
  EXPECT_EQ(t4.get<0>(), 5);
  EXPECT_EQ(t4.get<1>(), 5);
  EXPECT_EQ(t4.get<2>(), 0.5);
  EXPECT_EQ(t4.get<3>(), 0.5);
}

TEST(Operator, Divide) {
  const auto t1 = tpl::makeTuple(10, 10, 0.5, 0.5);
  const auto t2 = tpl::makeTuple(0.5, 0.5, 10,   10);

  const tpl::Tuple t3 = t1 / t2;
  EXPECT_EQ(t3.get<0>(), 20);
  EXPECT_EQ(t3.get<1>(), 20);
  EXPECT_EQ(t3.get<2>(), 0.05);
  EXPECT_EQ(t3.get<3>(), 0.05);

  tpl::Tuple t4 = t1;
  t4 /= t2;
  EXPECT_EQ(t4.get<0>(), 20);
  EXPECT_EQ(t4.get<1>(), 20);
  EXPECT_EQ(t4.get<2>(), 0.05);
  EXPECT_EQ(t4.get<3>(), 0.05);
}

TEST(Operator, Concat) {
  auto t1 = tpl::makeTuple(1, 0.5, 1.3f, std::string("abc"));
  auto t2 = tpl::makeTuple(std::string("yep"), '\t', 5, -99999);

  auto t3 = std::move(t1) | std::move(t2);

  EXPECT_EQ(t3.get<0>(), 1);
  EXPECT_EQ(t3.get<1>(), 0.5);
  EXPECT_EQ(t3.get<2>(), 1.3f);
  EXPECT_EQ(t3.get<3>(), "abc");
  EXPECT_EQ(t3.get<4>(), "yep");
  EXPECT_EQ(t3.get<5>(), '\t');
  EXPECT_EQ(t3.get<6>(), 5);
  EXPECT_EQ(t3.get<7>(), -99999);
}

TEST(Operator, ConcatWithDifferentSizes) {
  auto t1 = tpl::makeTuple(std::string("abc"), 0);
  auto t2 = tpl::makeTuple(std::string("def"), std::string("ghi"), 1);

  auto t3 = std::move(t1) | std::move(t2);
  EXPECT_EQ(t3.get<0>(), "abc");
  EXPECT_EQ(t3.get<1>(), 0);
  EXPECT_EQ(t3.get<2>(), "def");
  EXPECT_EQ(t3.get<3>(), "ghi");
  EXPECT_EQ(t3.get<4>(), 1);
}

TEST(Operator, ConcatWithOneEmpty) {
  auto t1 = tpl::makeTuple(1, 5, 86, 2.f, std::string("H1XcSY3M@!Me4yvCrv3Y8*q52B*wywPmgsyKG@8U5ht#UXuBX^gjL5rur!hNhy2%"));

  auto t2 = tpl::makeTuple();

  const auto t3 = std::move(t1) | std::move(t2);

  EXPECT_EQ(t1, t3);
}

TEST(Operator, ConcatFourTpl) {
  auto t1 = tpl::makeTuple(std::string("tpl0"));
  auto t2 = tpl::makeTuple(std::string("tpl1"));
  auto t3 = tpl::makeTuple(std::string("tpl2"));
  auto t4 = tpl::makeTuple(std::string("tpl3"));

  auto t5 = std::move(t1) | std::move(t2) | std::move(t3) | std::move(t4);

  EXPECT_EQ(t5.get<0>(), "tpl0");
  EXPECT_EQ(t5.get<1>(), "tpl1");
  EXPECT_EQ(t5.get<2>(), "tpl2");
  EXPECT_EQ(t5.get<3>(), "tpl3");
}


TEST(ExempleUtilisation, inpdf) {
  tpl::Tuple<int, double, std::string> t(42, 3.14, "The cake is ");
  EXPECT_EQ(t.get<0>(), 42);
  EXPECT_EQ(t.get<1>(), 3.14);
  EXPECT_EQ(t.get<2>(), "The cake is ");

  tpl::Tuple<int, double, std::string> t2(-42, -3.14, "a lie!");

  t += t2;

  EXPECT_EQ(t.get<0>(), 0);
  EXPECT_EQ(t.get<1>(), 0);
  EXPECT_EQ(t.get<2>(), "The cake is a lie!");

  auto t3 = tpl::makeTuple(10, 10.0f);
  tpl::Tuple t4(2lu, 2.0);
  auto t5 = t3 * t4;
  EXPECT_EQ(t5.get<0>(), 20);
  EXPECT_EQ(t5.get<1>(), 20);

  auto t6 = std::move(t) | std::move(t5);

  EXPECT_EQ(t6.get<0>(), 0);
  EXPECT_EQ(t6.get<1>(), 0);
  EXPECT_EQ(t6.get<2>(), "The cake is a lie!");
  EXPECT_EQ(t6.get<3>(), 20);
  EXPECT_EQ(t6.get<4>(), 20);
}
