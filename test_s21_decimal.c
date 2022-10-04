#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}, 0};
s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}, 0};
s21_decimal case1 = {{0, 0, 0, 0}, 0};
s21_decimal case2 = {{1, 0, 0, 0}, 0};
s21_decimal case3 = {{0x7FFFFFFF, 0, 0, 0}, 0};
s21_decimal case4 = {{0x7FFFFFFF, 0, 0, 0x80000000}, 0};
s21_decimal case5 = {{0x80000000, 0, 0, 0x90000}, 0};
s21_decimal case6 = {{755, 0, 0, 0x10000}, 0};
s21_decimal case7 = {{75, 0, 0, 0}, 0};
s21_decimal case8 = {{7551, 0, 0, 0x80020000}, 0};
s21_decimal case9 = {{76, 0, 0, 0x80000000}, 0};
s21_decimal case10 = {{755, 0, 0, 0}, 0};
s21_decimal case11 = {{5, 0, 0, 0}, 0};
s21_decimal case12 = {{150, 0, 0, 0x80000000}, 0};
s21_decimal case13 = {{2, 0, 0, 0x80000000}, 0};

// 1
START_TEST(test_s21_add) {
  int int_res = -1;
  s21_from_decimal_to_int((s21_add(case_max, case_min)), &int_res);
  ck_assert_int_eq(int_res, 0);
  s21_from_decimal_to_int((s21_add(case1, case2)), &int_res);
  ck_assert_int_eq(int_res, 1);
}
END_TEST

// 2
START_TEST(test_s21_sub) {
  int int_res = -1;
  if (!s21_is_equal((s21_sub(case1, case_max)), case_min))
    int_res = 0;
  else
    int_res = 1;
  ck_assert_int_eq(int_res, 0);
  s21_from_decimal_to_int((s21_sub(case1, case2)), &int_res);
  ck_assert_int_eq(int_res, -1);
}
END_TEST

// 3
START_TEST(test_s21_mul) {
  int int_res = -1;
  if (!s21_is_equal((s21_mul(case1, case_max)), case1) && !s21_is_equal((s21_mul(case_max, case2)), case_max))
    int_res = 0;
  else
    int_res = 1;
  ck_assert_int_eq(int_res, 0);
  s21_decimal multest = s21_mul(case_max, case_max);
  if (!s21_is_equal(multest, case1))
    int_res = 0;
  else
    int_res = 1;
  ck_assert_int_eq(int_res, 0);
  ck_assert_int_eq(multest.value_type, 1);
}
END_TEST

// 4
START_TEST(test_s21_div) {
  int int_res = -1;
  if (!s21_is_equal((s21_div(case1, case_max)), case1))
    int_res = 0;
  else
    int_res = 1;
  ck_assert_int_eq(int_res, 0);
  s21_decimal divtest = s21_div(case_max, case1);
  if (!s21_is_equal(divtest, case1))
    int_res = 0;
  else
    int_res = 1;
  ck_assert_int_eq(int_res, 0);
  ck_assert_int_eq(divtest.value_type, 3);
  divtest = s21_div(case12, case13);
  if (!s21_is_equal(divtest, case7))
    int_res = 0;
  else
    int_res = 1;
  ck_assert_int_eq(int_res, 0);
}
END_TEST

// 5
START_TEST(test_s21_mod) {
  ck_assert_msg(!s21_is_equal(case11, s21_mod(case10, case5)), "mod1");
  ck_assert_msg(!s21_is_equal(s21_negate(case2), s21_mod(case9, case7)), "mod2");
  ck_assert_msg(!s21_is_equal(case7, s21_mod(case7, case6)), "mod3");
  ck_assert_msg(!s21_is_equal(case1, s21_mod(case_max, case_max)), "mod4");
  }
END_TEST

// 6
START_TEST(test_s21_is_less) {
  ck_assert_msg(s21_is_less(case1, case2) == 0, "less1");
  ck_assert_msg(s21_is_less(case_max, case_min) == 1, "less2");
  ck_assert_msg(s21_is_less(case_min, case_max) == 0, "less3");
  ck_assert_msg(s21_is_less(case_min, case1) == 0, "less4");
  ck_assert_msg(s21_is_less(case2, case2) == 1, "less5");
}
END_TEST

// 7
START_TEST(test_s21_is_less_or_equal) {
  ck_assert_msg(s21_is_less_or_equal(case1, case2) == 0, "less_or_equal1");
  ck_assert_msg(s21_is_less_or_equal(case_max, case_min) == 1, "less_or_equal2");
  ck_assert_msg(s21_is_less_or_equal(case_min, case_max) == 0, "less_or_equal3");
  ck_assert_msg(s21_is_less_or_equal(case_min, case1) == 0, "less_or_equal4");
  ck_assert_msg(s21_is_less_or_equal(case2, case3) == 0, "less_or_equal5");
}
END_TEST

// 8
START_TEST(test_s21_is_greater) {
  ck_assert_msg(s21_is_greater(case1, case2) == 1, "greater1");
  ck_assert_msg(s21_is_greater(case_max, case_min) == 0, "greater2");
  ck_assert_msg(s21_is_greater(case_min, case_max) == 1, "greater3");
  ck_assert_msg(s21_is_greater(case_min, case1) == 1, "greater4");
  ck_assert_msg(s21_is_greater(case2, case3) == 1, "greater5");
}
END_TEST

// 9
START_TEST(test_s21_is_greater_or_equal) {
  ck_assert_msg(s21_is_greater_or_equal(case1, case2) == 1,
                "greater_or_equal1");
  ck_assert_msg(s21_is_greater_or_equal(case_max, case_min) == 0, "greater_or_equal2");
  ck_assert_msg(s21_is_greater_or_equal(case_min, case_max) == 1, "greater_or_equal3");
  ck_assert_msg(s21_is_greater_or_equal(case_min, case1) == 1, "greater_or_equal4");
  ck_assert_msg(s21_is_greater_or_equal(case2, case2) == 0,
                "greater_or_equal5");
}
END_TEST

// 10
START_TEST(test_s21_is_equal) {
  ck_assert_msg(s21_is_equal(case1, case2) == 1, "equal1");
  ck_assert_msg(s21_is_equal(case2, case2) == 0, "equal2");
}
END_TEST

// 11
START_TEST(test_s21_is_not_equal) {
  ck_assert_msg(s21_is_not_equal(case1, case2) == 0, "not_equal1");
  ck_assert_msg(s21_is_not_equal(case2, case2) == 1, "not_equal2");
}
END_TEST

// 12 14
START_TEST(test_s21_from_decimal_to_int) {
  int int_min = -2147483647;
  int min1 = 0;
  char test[16] = {};
  char test1[16] = {};
  s21_decimal casemin = {{0, 0, 0, 0}, 0};
  s21_from_int_to_decimal(int_min, &casemin);
  s21_from_decimal_to_int(casemin, &min1);
  snprintf(test, sizeof(test), "%.10d", int_min);
  snprintf(test1, sizeof(test1), "%.10d", min1);
  ck_assert_str_eq(test, test1);
  int int_max = 2147483647;
  int max1 = 0;
  s21_decimal casemin1 = {{0, 0, 0, 0}, 0};
  s21_from_int_to_decimal(int_max, &casemin1);
  s21_from_decimal_to_int(casemin1, &max1);
  ck_assert_msg(int_max == max1, "from_decimal_to_int1");
  int zero = 0;
  int zero1 = 0;
  s21_decimal case_2 = {{0, 0, 0, 0}, 0};
  s21_from_int_to_decimal(zero, &case_2);
  s21_from_decimal_to_int(case_2, &zero1);
  ck_assert_msg(zero == zero1, "from_decimal_to_int2");
}
END_TEST

// 13
START_TEST(test_s21_from_float_to_decimal) {
  float min = -0.1;
  s21_decimal test = {{1, 0, 0, 0x80010000}, 0};
  s21_from_float_to_decimal(min, &case1);
  int res = s21_is_equal(case1, test);
  ck_assert_int_eq(0, res);

  float min1 = -0.5;
  s21_decimal test1 = {{5000000, 0, 0, 0x80070000}, 0};
  s21_from_float_to_decimal(min1, &case1);
  int res1 = s21_is_equal(case1, test1);
  ck_assert_int_eq(0, res1);

  float min2 = -0.1234567;
  s21_decimal test2 = {{0x12D687, 0, 0, 0x80070000}, 0};
  s21_from_float_to_decimal(min2, &case1);
  int res2 = s21_is_equal(case1, test2);
  ck_assert_int_eq(0, res2);
}
END_TEST

// 15
START_TEST(test_s21_from_decimal_to_float) {
  s21_decimal test3 = {{1, 0, 0, 0x80010000}, 0};
  float m1 = 0;
  char t[16] = {};
  char t1[16] = {};
  s21_from_decimal_to_float(test3, &m1);
  snprintf(t, sizeof(t), "%.7f", m1);
  snprintf(t1, sizeof(t1),  "%.7f", m1);
  ck_assert_str_eq(t, t1);
}
END_TEST


// 16
START_TEST(test_s21_floor) {
  ck_assert_msg(!s21_is_equal(case7, s21_floor(case6)), "floor1");
  ck_assert_msg(!s21_is_equal(case9, s21_floor(case8)), "floor2");
}
END_TEST

// 17
START_TEST(test_s21_round) {
  ck_assert_msg(!s21_is_equal(case7, s21_round(case6)), "round1");
  ck_assert_msg(!s21_is_equal(case9, s21_round(case8)), "round2");
}
END_TEST

// 18
START_TEST(test_s21_truncate) {
  ck_assert_msg(!s21_is_equal(case7, s21_truncate(case6)), "truncate1");
  }
END_TEST

// 19
START_TEST(test_s21_negate) {
  int res = 0;
  s21_from_decimal_to_int((s21_negate(case2)), &res);
  ck_assert_msg(res == -1, "negate1");
}
END_TEST

int main() {
  Suite *s1 = suite_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  // 1
  TCase *s21_add_case = tcase_create("addTest");
  suite_add_tcase(s1, s21_add_case);
  tcase_add_test(s21_add_case, test_s21_add);

  // 2
  TCase *s21_sub_case = tcase_create("subTest");
  suite_add_tcase(s1, s21_sub_case);
  tcase_add_test(s21_sub_case, test_s21_sub);

  // 3
  TCase *s21_mul_case = tcase_create("mulTest");
  suite_add_tcase(s1, s21_mul_case);
  tcase_add_test(s21_mul_case, test_s21_mul);

  // 4
  TCase *s21_div_case = tcase_create("divTest");
  suite_add_tcase(s1, s21_div_case);
  tcase_add_test(s21_div_case, test_s21_div);

  // 6
  TCase *s21_is_less_case = tcase_create("lessTest");
  suite_add_tcase(s1, s21_is_less_case);
  tcase_add_test(s21_is_less_case, test_s21_is_less);

  // 7
  TCase *s21_is_less_or_equal_case = tcase_create("less_or_equalTest");
  suite_add_tcase(s1, s21_is_less_or_equal_case);
  tcase_add_test(s21_is_less_or_equal_case, test_s21_is_less_or_equal);

  // 8
  TCase *s21_is_greater_case = tcase_create("greaterTest");
  suite_add_tcase(s1, s21_is_greater_case);
  tcase_add_test(s21_is_greater_case, test_s21_is_greater);

  // 9
  TCase *s21_is_greater_or_equal_case = tcase_create("greater_or_equalTest");
  suite_add_tcase(s1, s21_is_greater_or_equal_case);
  tcase_add_test(s21_is_greater_or_equal_case, test_s21_is_greater_or_equal);

  // 10
  TCase *s21_is_equal_case = tcase_create("equalTest");
  suite_add_tcase(s1, s21_is_equal_case);
  tcase_add_test(s21_is_equal_case, test_s21_is_equal);

  // 11
  TCase *s21_is_not_equal_case = tcase_create("not_equalTest");
  suite_add_tcase(s1, s21_is_not_equal_case);
  tcase_add_test(s21_is_not_equal_case, test_s21_is_not_equal);

  // 12 14
  TCase *s21_from_decimal_to_int_case = tcase_create("from_decimal_to_intTest");
  suite_add_tcase(s1, s21_from_decimal_to_int_case);
  tcase_add_test(s21_from_decimal_to_int_case, test_s21_from_decimal_to_int);

  // 13
  TCase *s21_from_float_to_decimal_case = tcase_create("from_float_to_decimalTest");
  suite_add_tcase(s1, s21_from_float_to_decimal_case);
  tcase_add_test(s21_from_float_to_decimal_case, test_s21_from_float_to_decimal);

  // 15
  TCase *s21_from_decimal_to_float_case = tcase_create("from_decimal_to_floatTest");
  suite_add_tcase(s1, s21_from_decimal_to_float_case);
  tcase_add_test(s21_from_decimal_to_float_case, test_s21_from_decimal_to_float);

  // 16
  TCase *s21_floor_case = tcase_create("floorTest");
  suite_add_tcase(s1, s21_floor_case);
  tcase_add_test(s21_floor_case, test_s21_floor);

  // 17
  TCase *s21_round_case = tcase_create("roundTest");
  suite_add_tcase(s1, s21_round_case);
  tcase_add_test(s21_round_case, test_s21_round);

  // 18
  TCase *s21_truncate_case = tcase_create("truncateTest");
  suite_add_tcase(s1, s21_truncate_case);
  tcase_add_test(s21_truncate_case, test_s21_truncate);

  // 19
  TCase *s21_negate_case = tcase_create("negateTest");
  suite_add_tcase(s1, s21_negate_case);
  tcase_add_test(s21_negate_case, test_s21_negate);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
