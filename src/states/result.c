
#include <stdio.h>
#include "result.h"

typedef struct _resultData {
    uint8_t count;
} resultData;

resultData result_data;

void result_init(void) {
    result_data.count = 0;
    printf("result_init\n");
}

uint8_t result_step(void) {
    printf("result_step\n");
    result_data.count += 1;
    if (result_data.count > 4) return 1;
    return 0;
}

uint8_t result_exit(void) {
    printf("result_exit\n");
    return 1;
}
