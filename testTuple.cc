#include <gtest/gtest.h>

#include "Tuple.h"

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Classic, GetTest) {
  tpl::Tuple<int, double, std::string> t1(42, 3.14, "Hello World !");
  EXPECT_EQ(t1.get<0>(), 42);
  EXPECT_EQ(t1.get<1>(), 3.14);
  EXPECT_EQ(t1.get<2>(), "Hello World !");
}

TEST(Comparator, Inferior) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 < t2, true);
  EXPECT_EQ(t2 < t1, false);
  EXPECT_EQ(t3 < t1, false);
}

TEST(Comparator, InferiorOrEquals) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 <= t2, true);
  EXPECT_EQ(t2 <= t1, false);
  EXPECT_EQ(t3 <= t1, true);
}

TEST(Comparator, Superior) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 > t2, false);
  EXPECT_EQ(t2 > t1, true);
  EXPECT_EQ(t3 > t1, false);
}

TEST(Comparator, SuperiorOrEqual) {
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  const tpl::Tuple t3(t1);
  EXPECT_EQ(t1 >= t2, false);
  EXPECT_EQ(t2 >= t1, true);
  EXPECT_EQ(t3 >= t1, true);
}

TEST(Operator, Plus) {
  const tpl::Tuple t1(1,   1,   0.1, 0.1);
  const tpl::Tuple t2(0.1, 0.1, 1,   1);

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

TEST(Operator, Minus) {
  const tpl::Tuple t1(1,   1,   0.1, 0.1);
  const tpl::Tuple t2(0.1, 0.1, 1,   1);

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
  const tpl::Tuple t1(1,   1,   0.5, 0.5);
  const tpl::Tuple t2(0.5, 0.5, 1,   1);

  const tpl::Tuple t3 = t1 * t2;
  EXPECT_EQ(t3.get<0>(), 0.5);
  EXPECT_EQ(t3.get<1>(), 0.5);
  EXPECT_EQ(t3.get<2>(), 0.5);
  EXPECT_EQ(t3.get<3>(), 0.5);

  tpl::Tuple t4 = t1;
  t4 *= t2;
  EXPECT_EQ(t4.get<0>(), 0);
  EXPECT_EQ(t4.get<1>(), 0);
  EXPECT_EQ(t4.get<2>(), 0.5);
  EXPECT_EQ(t4.get<3>(), 0.5);
}

TEST(Operator, Divide) {
  const tpl::Tuple t1(10,  10,  0.5,  0.5);
  const tpl::Tuple t2(0.5, 0.5, 10,   10);

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


/*TEST(ExempleUtilisation, inpdf) {
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
  tpl::Tuple<std::size_t, double> t4(2lu, 2.0);
  auto t5 = t3 * t4;
  EXPECT_EQ(t5.get<0>(), 20);
  EXPECT_EQ(t5.get<1>(), 20);

  auto t6 = std::move(t) | std::move(t5);

  EXPECT_EQ(t6.get<0>(), 0);
  EXPECT_EQ(t6.get<1>(), 0);
  EXPECT_EQ(t6.get<2>(), "The cake is a lie!");
  EXPECT_EQ(t6.get<3>(), 20);
  EXPECT_EQ(t6.get<4>(), 20);
}*/
