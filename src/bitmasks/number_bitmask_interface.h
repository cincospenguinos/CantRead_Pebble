#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE } DisplayNumberEnum;

bool should_draw_block(DisplayNumberEnum e, int16_t x, int16_t y);
