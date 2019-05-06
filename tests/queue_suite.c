// Example test suite
#include "../src/Kernel/PIDQueue.c"
#include "CUnit/Basic.h"

void create_queue_test() {
  PIDQueue queue = newQueue();
  CU_ASSERT_PTR_NOT_NULL(queue);
  freeQueue(queue);
}

void poll_test() {
  PIDQueue queue = newQueue();
  offer(queue, 1);
  CU_ASSERT_EQUAL(poll(queue), 1);
  freeQueue(queue);
}

void poll_right_order_test() {
  PIDQueue queue = newQueue();
  offer(queue, 1);
  offer(queue, 2);
  offer(queue, 3);
  CU_ASSERT_EQUAL(poll(queue), 1);
  CU_ASSERT_EQUAL(poll(queue), 2);
  CU_ASSERT_EQUAL(poll(queue), 3);

  freeQueue(queue);
}

void size_test() {
  PIDQueue queue = newQueue();
  offer(queue, 1);
  CU_ASSERT_EQUAL(getSize(queue), 1);
  offer(queue, 2);
  CU_ASSERT_EQUAL(getSize(queue), 2);
  poll(queue);
  CU_ASSERT_EQUAL(getSize(queue), 1);
  freeQueue(queue);
}

int add_queue_tests(CU_pSuite pSuite) {
  if (NULL == CU_ADD_TEST(pSuite, create_queue_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, poll_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, poll_right_order_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, size_test)) return 0;
  return 1;
}
