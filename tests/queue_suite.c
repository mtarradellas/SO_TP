// Example test suite
#include "../src/Kernel/procQueue.c"
#include "CUnit/Basic.h"

static procQueue queue;
static void setup() {
  queue = newQueue();
}
static void clean() {
  freeQueue(queue);
}

void create_queue_test() {
  setup();
  CU_ASSERT_PTR_NOT_NULL(queue);
  clean();
}

void poll_test() {
  setup();
  tProcess phony = {1, "phony_process"};
  offer(queue, &phony);
  tProcess *actual = poll(queue);
  CU_ASSERT_EQUAL(actual->pid, 1);
  CU_ASSERT_STRING_EQUAL(actual->name, "phony_process");
  clean();
}

void poll_right_order_test() {
  setup();

  tProcess phonies[] = {
    {1, "phony_1"},
    {2, "phony_2"},
    {3, "phony_3"}
  };
  offer(queue, &(phonies[0]));
  offer(queue, &(phonies[1]));
  offer(queue, &(phonies[2]));

  CU_ASSERT_EQUAL(poll(queue)->pid, 1);
  CU_ASSERT_EQUAL(poll(queue)->pid, 2);
  CU_ASSERT_EQUAL(poll(queue)->pid, 3);
  clean();
}

void size_test() {
  setup();
  tProcess phonies[] = {
    {1, "phony_1"},
    {2, "phony_2"},
    {3, "phony_3"}
  };
  offer(queue, &(phonies[0]));
  CU_ASSERT_EQUAL(getSize(queue), 1);
  offer(queue, &(phonies[1]));
  CU_ASSERT_EQUAL(getSize(queue), 2);
  poll(queue);
  CU_ASSERT_EQUAL(getSize(queue), 1);
  clean();
}

int add_queue_tests(CU_pSuite pSuite) {
  if (NULL == CU_ADD_TEST(pSuite, create_queue_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, poll_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, poll_right_order_test)) return 0;
  if (NULL == CU_ADD_TEST(pSuite, size_test)) return 0;
  return 1;
}
