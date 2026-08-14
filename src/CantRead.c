#include <pebble.h>

static Window *s_window;
static Layer *s_canvas_layer;

const int16_t EMERY_BLOCK_WIDTH = 25;
const int16_t EMERY_BLOCK_HEIGHT = 19;

// I think I like the bitmask solution
const uint32_t MASK =       0b00000000000000000000000000000001;
const uint32_t NUMBER_DEF = 0b00000000111001110011100111000000;
static void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

  GPoint origin = GPoint(0, 0);
  for (int i = 0; i < 30; i++) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Checking %u", MASK >> i);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Value was %u", MASK & NUMBER_DEF);

    if ((MASK >> i) & NUMBER_DEF) {
      int16_t y = i / 5;
      int16_t x = i % 5;

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

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);

  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
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
