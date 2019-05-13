// Example test suite
#include "../src/Kernel/include/process.h"
#include "../src/Kernel/queue.c"
#include "CUnit/Basic.h"

static queue_t queue;
static void setup() { queue = queueCreate(sizeof(tProcess*)); }
static void clean() { queueFree(queue); }

void create_queue_test() {
  setup();
  CU_ASSERT_PTR_NOT_NULL(queue);
  clean();
}

void poll_test() {
  setup();
  tProcess phony = {1, "phony_process"};
  tProcess* phony_ptr = &phony;
  queueOffer(queue, &phony_ptr);
  tProcess* actual = NULL;
  queuePoll(queue, &actual);
  CU_ASSERT_EQUAL(actual->pid, 1);
  CU_ASSERT_STRING_EQUAL(actual->name, "phony_process");
  clean();
}

void poll_right_order_test() {
  setup();

  tProcess phony1 = {1, "phony_1"};
  tProcess phony2 = {2, "phony_2"};

  tProcess* phony1_p = &phony1;
  tProcess* phony2_p = &phony2;

  queueOffer(queue, &phony1_p);
  queueOffer(queue, &phony2_p);


  tProcess* actual1;
  tProcess* actual2;

  queuePoll(queue, &actual1);
  queuePoll(queue, &actual2);

  CU_ASSERT_EQUAL(actual1->pid, 1);
  CU_ASSERT_EQUAL(actual2->pid, 2);


  clean();
}

void size_test() {
  setup();

  tProcess phony1 = {1, "phony_1"};
  tProcess phony2 = {2, "phony_2"};

  tProcess* phony1_p = &phony1;
  tProcess* phony2_p = &phony2;

  queueOffer(queue, &phony1_p);
  CU_ASSERT_EQUAL(queueSize(queue), 1);
  queueOffer(queue, &phony2_p);
  CU_ASSERT_EQUAL(queueSize(queue), 2);

  clean();
}

int add_queue_tests(CU_pSuite pSuite) {
  if (NULL == CU_ADD_TEST(pSuite, create_queue_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, poll_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, poll_right_order_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, size_test)) return 0;
  return 1;
}
