//------------------------------------------------------------------------------
//  RS Simple MICR Pebble watchface
//  by Rich Szczepanski, Shrewsbury Productions
//  Copyright 2014
//------------------------------------------------------------------------------

#include <pebble.h>
#include <time.h>

  
Window       *itsWindow;
TextLayer    *itsTimeLayer;
TextLayer    *itsDayLayer;
TextLayer    *itsDateLayer;
TextLayer    *sep_layer;

static GFont itsTimeFont;
static GFont itsDayFont;


//------------------------------------------------------------------------------
void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  // Allocate long-lived storage (required by TextLayer)
  static char buffer[] = "00:00am";
  
  
  // Write the time to the buffer in a safe manner
  if (clock_is_24h_style())
  {
    strftime(buffer, sizeof("00:00am"), "%k:%M", tick_time);
  }
  else
  {
    strftime(buffer, sizeof("00:00am"), "%l:%M%p", tick_time);
  }
  
  // Set the TextLayer to display the buffer
  text_layer_set_text(itsTimeLayer, buffer);

  static char day_buffer[] = "wednesday";
  strftime(day_buffer, sizeof("wednesday"), "%A", tick_time);
  text_layer_set_text(itsDayLayer, day_buffer);
  
  static char date_buffer[] = "september 23";
  strftime(date_buffer, sizeof("september 23"), "%B %d", tick_time);
  text_layer_set_text(itsDateLayer, date_buffer);
  
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  short int hour = t->tm_hour;


  if (((units_changed & HOUR_UNIT) != 0) && ((hour > 4) && (hour <= 21)))
  {
    static const uint32_t const segments[] = { 40, 250, 40, 250, 40 };
    VibePattern pat =
    {
      .durations = segments,
      .num_segments = ARRAY_LENGTH(segments),
    };
    
    vibes_enqueue_custom_pattern(pat);
  }
}


//------------------------------------------------------------------------------
void handle_init(void)
{
  itsWindow = window_create();
  window_set_background_color(itsWindow, GColorBlack);
  window_set_fullscreen(itsWindow, true);

  itsTimeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_AtComputer_38));
  itsDayFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_AtComputer_22));
  
  // time_layer
  itsTimeLayer = text_layer_create(GRect(0, 25, 144, 52));
  text_layer_set_background_color(itsTimeLayer, GColorBlack);
  text_layer_set_text_color(itsTimeLayer, GColorWhite);
  text_layer_set_text_alignment(itsTimeLayer, GTextAlignmentCenter);
  text_layer_set_font(itsTimeLayer, itsTimeFont);
  layer_add_child(window_get_root_layer(itsWindow), (Layer *)itsTimeLayer);
  
  // day_layer
  itsDayLayer = text_layer_create(GRect(0, 100, 144, 30));
  text_layer_set_background_color(itsDayLayer, GColorBlack);
  text_layer_set_text_color(itsDayLayer, GColorWhite);
  text_layer_set_text_alignment(itsDayLayer, GTextAlignmentCenter);
  text_layer_set_font(itsDayLayer, itsDayFont);
  layer_add_child(window_get_root_layer(itsWindow), (Layer *)itsDayLayer);
  
  // date_layer
  itsDateLayer = text_layer_create(GRect(0, 133, 144, 30));
  text_layer_set_background_color(itsDateLayer, GColorBlack);
  text_layer_set_text_color(itsDateLayer, GColorWhite);
  text_layer_set_text_alignment(itsDateLayer, GTextAlignmentCenter);
  text_layer_set_font(itsDateLayer, itsDayFont);
  layer_add_child(window_get_root_layer(itsWindow), (Layer *)itsDateLayer);
  
  // separator layer
  sep_layer = text_layer_create(GRect(0, 80, 144, 2));
  text_layer_set_background_color(sep_layer, GColorWhite);
  layer_add_child(window_get_root_layer(itsWindow), (Layer *)sep_layer);
  
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
  
  window_stack_push(itsWindow, true);
}


//------------------------------------------------------------------------------
void handle_deinit(void)
{
  text_layer_destroy(itsTimeLayer);
  text_layer_destroy(itsDayLayer);
  text_layer_destroy(itsDateLayer);
  window_destroy(itsWindow);
}


//------------------------------------------------------------------------------
int main(void)
{
  handle_init();
  app_event_loop();
  handle_deinit();
}
