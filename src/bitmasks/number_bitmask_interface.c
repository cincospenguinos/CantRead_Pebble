#include <stdint.h>
#include <stdbool.h>
#include <pebble.h>

#include "number_bitmask_interface.h"

const uint32_t BITMASKS[] = {
    0b00000000111001110011100111000000, // ZERO
    0b00100001000010000100001000010000, // ONE
    0b00000001111000000000000111100000, // TWO
    0b00000000111100000000000111100000, // THREE
    0b00011110111101111000000111001110, // FOUR
    0b00000000111100000000001111000000, // FIVE
    0b00000000111001110000001111000000, // SIX
    0b00011110111101111011110111100000, // SEVEN
    0b00000000111000000000000111000000, // EIGHT
    0b00000000111101111000000111000000, // NINE
};

DisplayNumberEnum display_number_from_char(char numeric_char) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got char %c", numeric_char);
    switch (numeric_char) {
    case '0':
        return DISPLAY_NUM_ZERO;
    case '1':
        return DISPLAY_NUM_ONE;
    case '2':
        return DISPLAY_NUM_TWO;
    case '3':
        return DISPLAY_NUM_THREE;
    case '4':
        return DISPLAY_NUM_FOUR;
    case '5':
        return DISPLAY_NUM_FIVE;
    case '6':
        return DISPLAY_NUM_SIX;
    case '7':
        return DISPLAY_NUM_SEVEN;
    case '8':
        return DISPLAY_NUM_EIGHT;
    case '9':
        return DISPLAY_NUM_NINE;
    default:
        return DISPLAY_NUM_ZERO;
    }
}

bool should_draw_block(DisplayNumberEnum e, int16_t x, int16_t y) {
    uint32_t number_defn = BITMASKS[e];
    uint32_t the_bit = (1 << (y * 5 + x));

    return (number_defn & the_bit) != 0;
}
