
#ifndef STATE_H
#define STATE_H

#ifdef TEST
#include "mock_gb/mock_gb.h"
#else
#include <gb/gb.h>
#endif

typedef struct _state {
    uint8_t state;
    void (*init)(void);
    uint8_t (*step)(void);
    uint8_t (*exit)(void);
} state;

void assign(
    state *target,
    void (*init)(void),
    uint8_t (*step)(void),
    uint8_t (*exit)(void)
);

uint8_t run(state *self);

#endif
