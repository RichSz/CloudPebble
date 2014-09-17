#include <pebble.h>
#include <time.h>

  
Window       *my_window;
TextLayer    *time_layer;
TextLayer    *day_layer;
TextLayer    *date_layer;
TextLayer    *sep_layer;

static GFont itsTimeFont;
static GFont itsDayFont;


//------------------------------------------------------------------------------
void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Allocate long-lived storage (required by TextLayer)
  static char buffer[] = "00:00am";
  
  //Write the time to the buffer in a safe manner
  strftime(buffer, sizeof("00:00am"), "%l:%M%p", tick_time);
  
  //Set the TextLayer to display the buffer
  text_layer_set_text(time_layer, buffer);

  static char day_buffer[] = "wednesday";
  strftime(day_buffer, sizeof("wednesday"), "%A", tick_time);
  text_layer_set_text(day_layer, day_buffer);
  
  static char date_buffer[] = "september 23";
  strftime(date_buffer, sizeof("september 23"), "%B %d", tick_time);
  text_layer_set_text(date_layer, date_buffer);
  
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  short int hour = t->tm_hour;


  if (((units_changed & HOUR_UNIT) != 0) && ((hour > 4) && (hour <= 21)))
  {
//    vibes_short_pulse();
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
  my_window = window_create();
  window_set_background_color(my_window, GColorBlack);
  window_set_fullscreen(my_window, true);

  itsTimeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_AtComputer_38));
  itsDayFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_AtComputer_22));
  
  // time_layer
  time_layer = text_layer_create(GRect(0, 25, 144, 52));
  text_layer_set_background_color(time_layer, GColorBlack);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, itsTimeFont);
  layer_add_child(window_get_root_layer(my_window), (Layer *)time_layer);
  
  
  
  // day_layer
  day_layer = text_layer_create(GRect(0, 100, 144, 30));
  text_layer_set_background_color(day_layer, GColorBlack);
  text_layer_set_text_color(day_layer, GColorWhite);
  text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
  text_layer_set_font(day_layer, itsDayFont);
  layer_add_child(window_get_root_layer(my_window), (Layer *)day_layer);
  
  
  // date_layer
  date_layer = text_layer_create(GRect(0, 133, 144, 30));
  text_layer_set_background_color(date_layer, GColorBlack);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, itsDayFont);
  layer_add_child(window_get_root_layer(my_window), (Layer *)date_layer);
  
  // separator layer
  sep_layer = text_layer_create(GRect(0, 80, 144, 2));
  text_layer_set_background_color(sep_layer, GColorWhite);
  layer_add_child(window_get_root_layer(my_window), (Layer *)sep_layer);
  
  
  
  
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
  
  window_stack_push(my_window, true);
}


//------------------------------------------------------------------------------
void handle_deinit(void)
{
  text_layer_destroy(time_layer);
  text_layer_destroy(day_layer);
  text_layer_destroy(date_layer);
  window_destroy(my_window);
}


//------------------------------------------------------------------------------
int main(void)
{
  handle_init();
  app_event_loop();
  handle_deinit();
}
