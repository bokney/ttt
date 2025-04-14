
#ifdef TEST
#include "mock_gb/mock_gb.h"
#else
#include <gb/gb.h>
#endif

void game_init(void);
uint8_t game_step(void);
uint8_t game_exit(void);
