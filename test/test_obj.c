
#include "unity.h"
#include "obj.h"
#include <stdint.h>

extern obj *active;
extern obj *inactive;

static obj *self;
static obj *to_remove;
static obj *to_keep;

static int init_called = 0;
static int step_called = 0;
static int draw_called = 0;


void dummy_init(void *data) {
    init_called++;
}

uint8_t dummy_step(void *data) {
    step_called++;
    return 0;
}

uint8_t dummy_destroy(void *data) {
    return 0;
}

void dummy_draw(void *data) {
    
}

void counting_draw(void *data) {
    draw_called++;
}

uint8_t self_releasing_step(void *data) {
    obj_release(self);
    return 0;
}

uint8_t mixed_step(void *data) {
    if (data == (void*)1) obj_release(to_remove);
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

void test_obj_create_should_allocate_and_initialize(void) {
    obj *o = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    TEST_ASSERT_NOT_NULL(o);
    TEST_ASSERT_EQUAL(1, init_called);
    TEST_ASSERT_EQUAL_PTR(dummy_step, o->step);
    TEST_ASSERT_EQUAL_PTR(dummy_destroy, o->destroy);
    TEST_ASSERT_EQUAL_PTR(dummy_draw, o->draw);
    TEST_ASSERT_NULL(o->next);
}

void test_obj_release_should_return_to_inactive_pool(void) {
    obj *o1 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    int inactive_before = count_inactive();
    obj_release(o1);
    TEST_ASSERT_EQUAL(inactive_before + 1, count_inactive());
    TEST_ASSERT_NULL(active);
}

void test_obj_release_middle_object(void) {
    obj *o1 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj *o2 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj *o3 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    int inactive_before = count_inactive();
    obj_release(o2);
    TEST_ASSERT_EQUAL(inactive_before + 1, count_inactive());
    TEST_ASSERT_EQUAL_PTR(o1->next, o3);
}

void test_obj_release_all_should_empty_active_list(void) {
    obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj_release_all();
    TEST_ASSERT_NULL(active);
    TEST_ASSERT_EQUAL(20, count_inactive());
}

void test_obj_run_cycle_should_call_step_on_each(void) {
    obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj_run_cycle();
    TEST_ASSERT_EQUAL(2, step_called);
}

void test_obj_run_cycle_should_call_draw_on_each(void) {
    draw_called = 0;
    obj_create(NULL, dummy_step, NULL, counting_draw, NULL);
    obj_create(NULL, dummy_step, NULL, counting_draw, NULL);
    obj_run_cycle();
    TEST_ASSERT_EQUAL(2, draw_called);
}

void test_obj_self_removal(void) {
    self = obj_create(NULL, self_releasing_step, NULL, NULL, NULL);
    TEST_ASSERT_NOT_NULL(self);
    obj_run_cycle();
    TEST_ASSERT_NULL(active);
}

void test_obj_mixed_removal(void) {
    to_remove = obj_create(NULL, mixed_step, NULL, NULL, (void*)1);
    to_keep   = obj_create(NULL, mixed_step, NULL, NULL, (void*)2);
    TEST_ASSERT_EQUAL_PTR(active, to_remove);
    obj_run_cycle();
    TEST_ASSERT_EQUAL_PTR(active, to_keep);
    TEST_ASSERT_NULL(to_keep->next);
}

void test_obj_release_null(void) {
    int inactive_before = count_inactive();
    obj_release(NULL);
    TEST_ASSERT_EQUAL(inactive_before, count_inactive());
    TEST_ASSERT_NULL(active);
}

void test_obj_double_release(void) {
    obj *o = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    TEST_ASSERT_NOT_NULL(o);
    int inactive_before = count_inactive();
    obj_release(o);
    int inactive_after_first = count_inactive();
    TEST_ASSERT_EQUAL(inactive_before + 1, inactive_after_first);
    TEST_ASSERT_NULL(active);
    obj_release(o);
    TEST_ASSERT_EQUAL(inactive_after_first, count_inactive());
}

void test_obj_order_integrity(void) {
    obj *o1 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj *o2 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    obj *o3 = obj_create(dummy_init, dummy_step, dummy_destroy, dummy_draw, NULL);
    TEST_ASSERT_NOT_NULL(o1);
    TEST_ASSERT_NOT_NULL(o2);
    TEST_ASSERT_NOT_NULL(o3);
    TEST_ASSERT_EQUAL_PTR(o1, active);
    TEST_ASSERT_EQUAL_PTR(o2, o1->next);
    TEST_ASSERT_EQUAL_PTR(o3, o2->next);
    obj_release(o2);
    TEST_ASSERT_EQUAL_PTR(o3, o1->next);
    TEST_ASSERT_NULL(o3->next);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_obj_create_should_allocate_and_initialize);
    RUN_TEST(test_obj_release_should_return_to_inactive_pool);
    RUN_TEST(test_obj_release_middle_object);
    RUN_TEST(test_obj_release_all_should_empty_active_list);
    RUN_TEST(test_obj_run_cycle_should_call_step_on_each);
    RUN_TEST(test_obj_run_cycle_should_call_draw_on_each);
    RUN_TEST(test_obj_self_removal);
    RUN_TEST(test_obj_mixed_removal);
    RUN_TEST(test_obj_release_null);
    RUN_TEST(test_obj_order_integrity);

    return UNITY_END();
}
