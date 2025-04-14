
#ifdef TEST
#include "mock_gb/mock_gb.h"
#else
#include <gb/gb.h>
#endif

void splash_init(void);
uint8_t splash_step(void);
uint8_t splash_exit(void);
