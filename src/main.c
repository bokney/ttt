
#ifdef TEST
#include "mock_gb/mock_crash_handler.h"
#else
#include <gb/crash_handler.h>
#include <stdio.h>
#include <string.h>
#endif

#include "obj.h"
// #include "game_states.h"

// uint32_t global_counter = 0;
extern struct _obj *active;
extern struct _obj *inactive;

typedef struct state_a_data_ {
    uint8_t a_number;
    char a_string[32];
} state_a_data;

void state_a_init(void *data) {
    state_a_data *state_data;
    state_data = (state_a_data *)data;
    printf("  -  State A Init  -\n");
    state_data->a_number++;
    strcpy(state_data->a_string, "~~~~~\0");
}

uint8_t state_a_step(void *data) {
    state_a_data *state_data;
    state_data = data;
    printf("  -  State A Step %u  -\n", state_data->a_number);
    state_data->a_number++;
    if (state_data->a_number > 3) {
        return 1;
    }
    delay(100);
    return 0;
}

uint8_t state_a_exit(void *data) {
    state_a_data *state_data;
    state_data = data;
    printf("  -  State A Exit  -\n");
    state_data->a_number++;
    return 0;
}

typedef struct state_b_data_ {
    uint8_t a_number;
    char a_string[32];
} state_b_data;

void state_b_init(void *data) {
    state_b_data *state_data;
    state_data = data;
    printf("  -  State B Init  -\n");
    strcpy(state_data->a_string, "It's a string.\0");
    state_data->a_number = 0;
}

uint8_t state_b_step(void *data) {
    state_b_data *state_data;
    state_data = data;
    printf("  -  State B Step %u  -\n", state_data->a_number);
    state_data->a_number++;
    if (state_data->a_number > 4) {
        return 1;
    }
    delay(100);
    return 0;
}

uint8_t state_b_exit(void *data) {
    state_b_data *state_data;
    state_data = data;
    printf("  -  State B Exit  -\n");
    return 0;
}

int main(void) {
    obj_pool_init();

    state_a_data state_a;
    obj *state_a_obj = obj_create(
        state_a_init,
        state_a_step,
        state_a_exit,
        &state_a
    );
    state_a_data state_b;
    obj *state_b_obj = obj_create(
        state_b_init,
        state_b_step,
        state_b_exit,
        &state_b
    );
    printf("Hello\n");
    
    obj *curr = active;
    uint8_t count = 0;
    while (curr) {
        printf("Active count: %u\n", count);
        curr = curr->next;
        count++;
    }
    delay(1000);
    
    curr = inactive;
    count = 0;
    while (curr) {
        printf("Inactive count: %u\n", count);
        curr = curr->next;
        count++;
    }
    delay(1000);

    for (;;) {
        obj_run_cycle(active);
    }
}
