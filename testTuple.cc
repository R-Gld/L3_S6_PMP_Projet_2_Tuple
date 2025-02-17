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

TEST(Classic, General_ToSplit) { // TODO split test
  const tpl::Tuple t1(1,   1,   1.1, 1.1);
  const tpl::Tuple t2(2.2, 2.2, 2,   2);
  EXPECT_EQ(t1 < t2, true);
  EXPECT_EQ(t1 <= t2, true);
  EXPECT_EQ(t1 > t2, false);
  EXPECT_EQ(t1 >= t2, false);
  const tpl::Tuple t3 = t1 + t2;
  EXPECT_EQ(t3.get<0>(), 3.2);
  EXPECT_EQ(t3.get<1>(), 3.2);
  EXPECT_EQ(t3.get<2>(), 3.1);
  EXPECT_EQ(t3.get<3>(), 3.1);
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
