
#ifdef TEST
#include "mock_gb/mock_crash_handler.h"
#include <stddef.h>
#else
#include <gb/crash_handler.h>
#include <gb/gb.h>
#endif

#include "obj.h"

#define MAX_OBJ 10

obj *active = NULL;
obj *inactive = NULL;
obj obj_pool[MAX_OBJ];

void obj_pool_init(void) {
    for (int i = 0; i < MAX_OBJ - 1; i++) {
        obj_pool[i].next = &obj_pool[i + 1];
    }
    obj_pool[MAX_OBJ - 1].next = NULL;
    inactive = &obj_pool[0];
}

obj *obj_create(
    void (*init)(void *data),
    uint8_t (*step)(void *data),
    uint8_t (*exit)(void *data),
    void *data
) {
    if (!inactive) {
        return (obj *)NULL;
    }

    obj *new_obj = inactive;
    inactive = inactive->next;

    new_obj->init = init;
    new_obj->step = step;
    new_obj->exit = exit;
    new_obj->data = data;
    new_obj->next = NULL;

    if (new_obj->init) {
        new_obj->init(new_obj->data);
    }

    if (!active) {
        active = new_obj;
    } else {
        obj *tail = active;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = new_obj;
    }

    return new_obj;
}

void obj_release(obj **target) {
    if (!target || !*target) return;
    obj *bye = *target;
    bye->data = NULL;
    bye->init = NULL;
    bye->step = NULL;
    bye->exit = NULL;
    bye->next = inactive;
    inactive = bye;
    *target = NULL;
}

void obj_run_cycle(obj *start) {
    if (start) {
        obj *curr = start;
        while (curr) {
            curr->step(curr->data);
            curr = curr->next;
        }
    }
}
