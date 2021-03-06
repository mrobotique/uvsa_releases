
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */


void init_test(Adafruit_MCP23017 gpio){
  int t_ON = 500;
  int t_OFF = 100;
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Black;
  FastLED.show(int(max_intensity));
  delay(t_OFF);
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Red;
  FastLED.show(int(max_intensity));
  delay(t_ON);
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Black;
  FastLED.show(int(max_intensity));
  delay(t_OFF);
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Green;
  FastLED.show(int(max_intensity));
  delay(t_ON);
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Black;
  FastLED.show(int(max_intensity));
  delay(t_OFF);
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Blue;
  FastLED.show(int(max_intensity));
  delay(t_ON);
  for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Black;
  FastLED.show(int(max_intensity));
  delay(t_OFF);
  lamp_state.lamp_deadman = 1;
  lamp_state.lamp_auto = 1;
  write_lamps(lamp_state, gpio);
  delay(t_ON);
  lamp_state.lamp_deadman = 0;
  lamp_state.lamp_auto = 0;
  write_lamps(lamp_state, gpio);
}
