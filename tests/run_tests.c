#include "CUnit/Basic.h"
#include "include/sum_suite.h"
#include "include/queue_suite.h"

static CU_pSuite addSuiteToRegistry(char* suiteName);
static int exitWithError();

typedef struct {
  char* name;
  int (*addTests)(CU_pSuite);
} suite_t;

// test suites
suite_t suites[] = {
  {"sum_suite", &add_sum_tests},
  {"queue_suite", &add_queue_tests}
};

int main(void) {
  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  for (int i = 0; i < (sizeof(suites)/sizeof(suite_t)); i++) {
    CU_pSuite pSuite = addSuiteToRegistry(suites[i].name);
    if (NULL == pSuite) return exitWithError();
    if (!((suites[i].addTests)(pSuite))) return exitWithError();
  }

  // Sets the basic run mode, CU_BRM will show maximum output of run details
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // Run the tests and show the run summary
  CU_basic_run_tests();
  return CU_get_error();
}

static CU_pSuite addSuiteToRegistry(char* suiteName) {
  CU_pSuite pSuite = NULL;
  pSuite = CU_add_suite(suiteName, 0, 0);
  return pSuite;
}

static int exitWithError() {
  CU_cleanup_registry();
  return CU_get_error();
}
