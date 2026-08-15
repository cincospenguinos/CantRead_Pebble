#include <pebble.h>

#include "bitmasks/number_bitmask_interface.h"

static Window *s_window;
static Layer *s_canvas_layer;

const int16_t EMERY_BLOCK_WIDTH = 20;
const int16_t EMERY_BLOCK_HEIGHT = 19;

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

  GPoint origin = GPoint(0, 0);
  for (int16_t y = 0; y < 6; y++) {
    for (int16_t x = 0; x < 5; x++) {
      if (should_draw_block(DISPLAY_NUM_FIVE, x, y)) {
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

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Bounds are %d x %d", bounds.size.w, bounds.size.h);

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);

  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Display this time wherever
  APP_LOG(APP_LOG_LEVEL_DEBUG, "The time is %s", s_time_buffer);

  for (int i = 0; i < 8; i++) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d: %c (%i)", i, s_time_buffer[i], s_time_buffer[i]);
  }
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
  const bool animated = true;
  window_stack_push(s_window, animated);
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
