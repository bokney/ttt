
#ifdef TEST
#include "mock_gb/mock_crash_handler.h"
#include <stddef.h>
#else
#include <gb/crash_handler.h>
#endif

#include "obj.h"

#define MAX_OBJ 20

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
    uint8_t (*destroy)(void *data),
    void (*draw)(void *data),
    void *data
) {
    if (!inactive) {
        return (obj *)NULL;
    }

    obj *new_obj = inactive;
    inactive = inactive->next;

    new_obj->init = init;
    new_obj->step = step;
    new_obj->destroy = destroy;
    new_obj->draw = draw;
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

void obj_release(obj *target) {
    if (!active || !target) return;

    if (active == target) {
        active = active->next;
    } else {
        obj *prev = active;
        while (prev->next && prev->next != target) {
            prev = prev->next;
        }
        if (prev->next == target) {
            prev->next = target->next;
        }
    }

    target->data = NULL;
    target->init = NULL;
    target->step = NULL;
    target->destroy = NULL;
    target->draw = NULL;
    target->next = inactive;
    inactive = target;
}

void obj_release_all(void) {
    obj *curr = active;
    obj *next;
    while (curr != NULL) {
        next = curr->next;

        curr->data  = NULL;
        curr->init  = NULL;
        curr->step  = NULL;
        curr->destroy  = NULL;
        curr->draw  = NULL;

        curr->next = inactive;
        inactive   = curr;

        curr = next;
    }
    active = NULL;
}

void obj_render(void) {
    obj *curr = active;
    while (curr != NULL) {
        if (curr->draw) {
            curr->draw(curr->data);
        }
        curr = curr->next;
    }
}

void obj_run_cycle(void) {
    obj *curr = active;
    while (curr) {
        obj *next = curr->next;
        if (curr->step)
            curr->step(curr->data);
        curr = next;
    }
    obj_render();
}
