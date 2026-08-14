#include <stdint.h>
#include <stdbool.h>

#include "number_bitmask_interface.h"

const uint32_t BITMASKS[] = {
    0b00000000111001110011100111000000, // ZERO
    0b00100001000010000100001000010000, // ONE
    0b00000001111000000000000111100000, // TWO
};

bool should_draw_block(DisplayNumberEnum e, int16_t x, int16_t y) {
    uint32_t number_defn = BITMASKS[e];
    uint32_t the_bit = (1 << (y * 5 + x));

    return (number_defn & the_bit) != 0;
}
