
#ifndef OBJ_H
#define OBJ_H

#ifdef TEST
#include "mock_gb/mock_gb.h"
#else
#include <gb/gb.h>
#endif

typedef struct _obj {
    void (*init)(void *data);
    uint8_t (*step)(void *data);
    uint8_t (*exit)(void *data);
    void *data;
    struct _obj *next;
} obj;

void obj_pool_init(void);

obj *obj_create(
    void (*init)(void *data),
    uint8_t (*step)(void *data),
    uint8_t (*exit)(void *data),
    void *data
);

void obj_release(obj **target);

void obj_run_cycle(obj *start);

#endif
