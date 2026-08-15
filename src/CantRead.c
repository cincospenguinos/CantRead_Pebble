#include <pebble.h>

#include "bitmasks/number_bitmask_interface.h"

static Window *s_window;
static Layer *s_canvas_layer;

const int16_t EMERY_BLOCK_WIDTH = 20;
const int16_t EMERY_BLOCK_HEIGHT = 19;

static DisplayNumberEnum CURRENT_NUMBERS[4];

void update_display_numbers(char *s_time_buffer) {
  CURRENT_NUMBERS[0] = display_number_from_char(s_time_buffer[0]);
  CURRENT_NUMBERS[1] = display_number_from_char(s_time_buffer[1]);
  CURRENT_NUMBERS[2] = display_number_from_char(s_time_buffer[3]);
  CURRENT_NUMBERS[3] = display_number_from_char(s_time_buffer[4]);
}

GPoint ALL_POSITIONS[4] = {
  GPoint(0, 0),
  GPoint(EMERY_BLOCK_WIDTH * 5, 0),
  GPoint(0, EMERY_BLOCK_HEIGHT * 6),
  GPoint(EMERY_BLOCK_WIDTH * 5, EMERY_BLOCK_HEIGHT * 6),
};

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  update_display_numbers(s_time_buffer);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Time is %s", s_time_buffer);
  layer_mark_dirty(s_canvas_layer);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

  for (int i = 0; i < 4; i++) {
    DisplayNumberEnum number_to_display = CURRENT_NUMBERS[i];
    GPoint origin = ALL_POSITIONS[i];

    for (int16_t y = 0; y < 6; y++) {
      for (int16_t x = 0; x < 5; x++) {
        if (should_draw_block(number_to_display, x, y)) {
          GRect bounds = GRect(
            origin.x + x * EMERY_BLOCK_WIDTH,
            origin.y + y * EMERY_BLOCK_HEIGHT,
            EMERY_BLOCK_WIDTH,
            EMERY_BLOCK_HEIGHT
          );

          graphics_draw_rect(ctx, bounds);
          graphics_fill_rect(ctx, bounds, 0, GCornersAll);
        }
      }
    }
  }
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Time is %s", s_time_buffer);
  update_display_numbers(s_time_buffer);
}

static void prv_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });

  window_stack_push(s_window, true);
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
