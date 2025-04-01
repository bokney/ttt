
#include "unity.h"
#include "obj.h"
#include <stdint.h>

extern obj *active;
extern obj *inactive;

static int init_called;
static int step_called;

void dummy_init(void *data) {
    init_called++;
}

uint8_t dummy_step(void *data) {
    step_called++;
    return 0;
}

uint8_t dummy_exit(void *data) {
    return 0;
}

static int count_inactive(void) {
    int count = 0;
    obj *curr = inactive;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

void setUp(void) {
    init_called = 0;
    step_called = 0;

    active = NULL;
    inactive = NULL;

    obj_pool_init();
}

void tearDown(void) {

}

void test_reuse_after_release(void) {
    obj *test_obj_a = obj_create(
        dummy_init, dummy_step, dummy_exit, (void *)0x1234
    );
    TEST_ASSERT_NOT_NULL(test_obj_a);

    obj *released_obj = test_obj_a;
    obj_release(&test_obj_a);
    TEST_ASSERT_NULL(test_obj_a);

    obj *test_obj_b = obj_create(
        dummy_init, dummy_step, dummy_exit, (void *)0x5678
    );
    TEST_ASSERT_NOT_NULL(test_obj_b);
    TEST_ASSERT_EQUAL_PTR(released_obj, test_obj_b);
}

void test_create_with_null_init(void) {
    obj *test_ob = obj_create(
        NULL, dummy_step, dummy_exit, (void *)0xABCD
    );
    TEST_ASSERT_NOT_NULL(test_ob);
    TEST_ASSERT_EQUAL_INT(0, init_called);
}

// void test_run_cycle_empty(void) {
//     obj_run_cycle(NULL);
// }

// void test_integration_create_run_release(void) {
//     obj *test_obj_a = obj_create(
//         dummy_init, dummy_step, dummy_exit, (void *)0x1111
//     );
//     obj *test_obj_b = obj_create(
//         dummy_init, dummy_step, dummy_exit, (void *)0x2222
//     );
//     TEST_ASSERT_NOT_NULL(test_obj_a);
//     TEST_ASSERT_NOT_NULL(test_obj_b);
    
//     step_called = 0;
//     obj_run_cycle(active);
//     TEST_ASSERT_EQUAL_INT(2, step_called);
    
//     obj_release(&test_obj_a);
//     TEST_ASSERT_NULL(test_obj_a);
    
//     step_called = 0;
//     obj_run_cycle(active);
//     TEST_ASSERT_EQUAL_INT(1, step_called);
// }

// void test_double_release(void) {
//     obj *test_obj = obj_create(dummy_init, dummy_step, dummy_exit, (void *)0x3333);
//     TEST_ASSERT_NOT_NULL(test_obj);

//     int count_before = count_inactive();
//     obj_release(&test_obj);
//     int count_after_first = count_inactive();

//     obj_release(&test_obj);
//     int count_after_second = count_inactive();
    
//     TEST_ASSERT_EQUAL_INT(count_after_first, count_after_second);
// }

// void test_active_list_ordering(void) {
//     obj *test_obj_a = obj_create(dummy_init, dummy_step, dummy_exit, (void *)0x1010);
//     obj *test_obj_b = obj_create(dummy_init, dummy_step, dummy_exit, (void *)0x2020);
//     obj *test_obj_c = obj_create(dummy_init, dummy_step, dummy_exit, (void *)0x3030);
    
//     TEST_ASSERT_NOT_NULL(test_obj_a);
//     TEST_ASSERT_NOT_NULL(test_obj_b);
//     TEST_ASSERT_NOT_NULL(test_obj_c);
    
//     TEST_ASSERT_EQUAL_PTR(test_obj_a, active);
//     TEST_ASSERT_EQUAL_PTR(test_obj_b, active->next);
//     TEST_ASSERT_EQUAL_PTR(test_obj_c, active->next->next);
//     TEST_ASSERT_NULL(active->next->next->next);
// }

// void test_run_cycle_with_null_step(void) {
//     obj *test_obj = obj_create(dummy_init, NULL, dummy_exit, (void *)0x4444);
//     TEST_ASSERT_NOT_NULL(test_obj);
    
//     step_called = 0;
//     obj_run_cycle(active);
//     TEST_ASSERT_EQUAL_INT(0, step_called);
// }

// void test_field_clearing_on_release(void) {
//     obj *test_obj = obj_create(dummy_init, dummy_step, dummy_exit, (void *)0x5555);
//     TEST_ASSERT_NOT_NULL(test_obj);
    
//     obj_release(&test_obj);
//     TEST_ASSERT_NULL(test_obj->init);
//     TEST_ASSERT_NULL(test_obj->step);
//     TEST_ASSERT_NULL(test_obj->exit);
//     TEST_ASSERT_NULL(test_obj->data);
// }

// void test_nullifying_released_pointer(void) {
//     obj *test_obj = obj_create(dummy_init, dummy_step, dummy_exit, (void *)0x6666);
//     TEST_ASSERT_NOT_NULL(test_obj);
    
//     obj_release(&test_obj);
//     TEST_ASSERT_NULL(test_obj);
// }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_reuse_after_release);
    RUN_TEST(test_create_with_null_init);
    // RUN_TEST(test_run_cycle_empty);
    // RUN_TEST(test_integration_create_run_release);
    // RUN_TEST(test_double_release);
    // RUN_TEST(test_active_list_ordering);
    // RUN_TEST(test_run_cycle_with_null_step);
    // RUN_TEST(test_field_clearing_on_release);
    // RUN_TEST(test_nullifying_released_pointer);
    return UNITY_END();
}
