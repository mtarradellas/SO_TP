// Example test suite
#include "../sum/sum.c"
#include "CUnit/Basic.h"

typedef void (*test_t)(void);

void sum_positive_test() { CU_ASSERT_EQUAL(sum(2, 14), 16); }

void sum_cero_test() { CU_ASSERT_EQUAL(sum(0, 0), 0); }

void sum_negative_test() { CU_ASSERT_EQUAL(sum(-1, 66), 5); }

test_t tests[] = {&sum_positive_test, &sum_cero_test, &sum_negative_test};

int add_sum_tests(CU_pSuite pSuite) {
  for (int i = 0; i < (sizeof(tests) / sizeof(test_t)); i++) {
    if (NULL == CU_ADD_TEST(pSuite, tests[i])) return 0;
  }
  return 1;
}
