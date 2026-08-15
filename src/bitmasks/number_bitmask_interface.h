#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    DISPLAY_NUM_ZERO,
    DISPLAY_NUM_ONE,
    DISPLAY_NUM_TWO,
    DISPLAY_NUM_THREE,
    DISPLAY_NUM_FOUR,
    DISPLAY_NUM_FIVE,
    DISPLAY_NUM_SIX,
    DISPLAY_NUM_SEVEN,
    DISPLAY_NUM_EIGHT,
    DISPLAY_NUM_NINE,
} DisplayNumberEnum;

bool should_draw_block(DisplayNumberEnum e, int16_t x, int16_t y);
