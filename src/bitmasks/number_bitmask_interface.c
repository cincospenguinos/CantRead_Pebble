#include <stdint.h>
#include <stdbool.h>

#include "number_bitmask_interface.h"

/*
Numbers are defined via bitmasks (this was my solution to the problem of needing a
small but simple mechanism to enumerate the different numbers.)

The end bytes enumerate the top, the next five the next row, etc. This is
counter-intuitive, I know. The indirection of the definitions makes the lookup
algorithm simple for my smooth brain.

The enumerated type is an index into this array, making this interface stupid simple
too.
 */
const uint32_t BITMASKS[] = {
    0b00000000111001110011100111000000, // ZERO
    0b00100001000010000100001000010000, // ONE
    0b00000001111000000000000111100000, // TWO
    0b00000000111100000000000111100000, // THREE
    0b00011110111101111000000111001110, // FOUR
    0b00000000111100000000001111000000, // FIVE
};

bool should_draw_block(DisplayNumberEnum e, int16_t x, int16_t y) {
    uint32_t number_defn = BITMASKS[e];
    uint32_t the_bit = (1 << (y * 5 + x));

    return (number_defn & the_bit) != 0;
}
