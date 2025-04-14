
#include "unity.h"
#include "state.h"

uint8_t test_counter_a, test_counter_b;

void setUp(void) {

}

void tearDown(void) {

}

void init_func(void) {
    test_counter_a = 4;
    test_counter_b = 0;
}

uint8_t iter_func(void) {
    test_counter_a--;
    test_counter_b++;
    if (test_counter_a == 0)
        return 1;
    return 0;
}

uint8_t exit_func(void) {
    return 10;
}

void test_assign(void) {
    state test_state;
    test_state.state = 10;
    test_state.init = NULL;
    test_state.step = NULL;
    test_state.exit = NULL;
    TEST_ASSERT_EQUAL(test_state.state, 10);
    TEST_ASSERT_EQUAL(test_state.init, NULL);
    TEST_ASSERT_EQUAL(test_state.step, NULL);
    TEST_ASSERT_EQUAL(test_state.exit, NULL);
    assign(
        &test_state,
        &init_func,
        &iter_func,
        &exit_func
    );
    TEST_ASSERT_EQUAL(test_state.state, 0);
    TEST_ASSERT_EQUAL(test_state.init, &init_func);
    TEST_ASSERT_EQUAL(test_state.step, &iter_func);
    TEST_ASSERT_EQUAL(test_state.exit, &exit_func);
}

void test_basic_functionality(void) {
    state test_state;
    assign(
        &test_state,
        &init_func,
        &iter_func,
        &exit_func
    );
    test_counter_a = 255;
    test_counter_b = 255;
    uint8_t result;
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 3);
    TEST_ASSERT_EQUAL(test_counter_b, 1);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 2);
    TEST_ASSERT_EQUAL(test_counter_b, 2);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 1);
    TEST_ASSERT_EQUAL(test_counter_b, 3);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 0);
    TEST_ASSERT_EQUAL(test_counter_b, 4);
    TEST_ASSERT_EQUAL(10, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 3);
    TEST_ASSERT_EQUAL(test_counter_b, 1);
    TEST_ASSERT_EQUAL(0, result);
}

void test_null_init(void) {
    state test_state;
    assign(
        &test_state,
        NULL,
        &iter_func,
        &exit_func
    );
    test_counter_a = 6;
    test_counter_b = 3;
    uint8_t result;
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 5);
    TEST_ASSERT_EQUAL(test_counter_b, 4);
    TEST_ASSERT_EQUAL(0, result);
}

void test_null_iter(void) {
    state test_state;
    assign(
        &test_state,
        &init_func,
        NULL,
        &exit_func
    );
    test_counter_a = 255;
    test_counter_b = 255;
    uint8_t result;
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 4);
    TEST_ASSERT_EQUAL(test_counter_b, 0);
    TEST_ASSERT_EQUAL(10, result);
    test_counter_a = 255;
    test_counter_b = 255;
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 4);
    TEST_ASSERT_EQUAL(test_counter_b, 0);
    TEST_ASSERT_EQUAL(10, result);
}

void test_null_exit(void) {
    state test_state;
    assign(
        &test_state,
        &init_func,
        &iter_func,
        NULL
    );
    test_counter_a = 255;
    test_counter_b = 255;
    uint8_t result;
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 3);
    TEST_ASSERT_EQUAL(test_counter_b, 1);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 2);
    TEST_ASSERT_EQUAL(test_counter_b, 2);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 1);
    TEST_ASSERT_EQUAL(test_counter_b, 3);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 0);
    TEST_ASSERT_EQUAL(test_counter_b, 4);
    TEST_ASSERT_EQUAL(0, result);
    result = run(&test_state);
    TEST_ASSERT_EQUAL(test_counter_a, 3);
    TEST_ASSERT_EQUAL(test_counter_b, 1);
    TEST_ASSERT_EQUAL(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_assign);
    RUN_TEST(test_basic_functionality);
    RUN_TEST(test_null_init);
    RUN_TEST(test_null_iter);
    RUN_TEST(test_null_exit);
    return UNITY_END();
}
