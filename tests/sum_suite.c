// Example test suite
#include "../sum/sum.c"
#include "CUnit/Basic.h"

void sum_positive_test() { CU_ASSERT_EQUAL(sum(2, 14), 16); }

void sum_cero_test() { CU_ASSERT_EQUAL(sum(0, 0), 0); }

void sum_negative_test() { CU_ASSERT_EQUAL(sum(-1, 6), 5); }


int add_sum_tests(CU_pSuite pSuite) {
  if (NULL == CU_ADD_TEST(pSuite, sum_positive_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, sum_cero_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, sum_negative_test)) return 0;

  return 1;
}
