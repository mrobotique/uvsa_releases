
#include <FastLED.h>

//Define la posicion del primer led por segmento
const int seg0 = 0;
const int seg1 = 1;
const int seg2 = 2;
const int seg3 = 3;
const int seg4 = 4;
const int seg5 = 5;
const int seg6 = 6;

int previous_mode;
int LedCount = 0;
//Delay para el fade in fade out
unsigned long fadeDelay = 15; //en mS
unsigned long last_millis = millis();
int max_intensity = 164;
int min_intensity = 10;
int intensity = min_intensity;
bool toggle_inc = false;

void uvsafe_timer_Complete(void);

class RGBLeds{
int current_mode; //Modo en el que se encuentra el uvs
unsigned long previousMillis;
unsigned long data_period = 10;  // tiempo en milisegs
//tiempo de inicio (cuenta regresiva)
unsigned long init_time = 600; //1 minuto
//tiempo por segmento = tiempo de inicio / total de leds
unsigned long init_time_segment = int (init_time/seg6);

public:
RGBLeds(uvs_mode _current_mode){
  current_mode = _current_mode;
}

public:
  void Update(uvs_mode _current_mode, SENSOR_STRUCT sensors){
    current_mode = _current_mode;
    switch (current_mode) {
      case mode_manual:
        if(previous_mode != current_mode){
          intensity = 0;
          FastLED.show(intensity);
          previous_mode = current_mode;
        }
        manual_pattern(sensors);

        break;
      case mode_auto_init:
        //TODO
        if(previous_mode != current_mode){
          LedCount = 0;
          leds[LedCount] = CRGB::DeepSkyBlue;
          FastLED.show(max_intensity);
          if(LedCount<1) LedCount++;
          previous_mode = current_mode;
          toggle_inc = false;
          last_millis = millis();
        }
        init_pattern();
        break;

      case mode_auto_on:
        if(previous_mode != current_mode){
          intensity = min_intensity;//intensity cant be < min_intensity
          previous_mode = current_mode;
          toggle_inc = false;

        }
        auto_pattern();
        //aqui tiene que tener dos modos. primero el de la cuenta regresiva
        //en donde los leds se iluminaran poco a poco y despues
        // el fade in fade out mientras el timer no ha acabado.
        // ver enum timer mode (HAL.h)
        break;

        case mode_manual_push:
        if(previous_mode != current_mode){
          intensity = min_intensity;//intensity cant be < min_intensity
          previous_mode = current_mode;
          toggle_inc = false;
        }
          manual_push_pattern();
          break;

      case mode_test:
        //TBD
        break;

      default:
        //El default es el modo manual... por seguirdad
        break;
    }//case
  }

  void init_pattern(){

    if ((millis() - last_millis) > init_time_segment){
      last_millis = millis();
      if (LedCount<seg6){
      leds[LedCount] = CRGB::DeepSkyBlue;
      FastLED.show(max_intensity);
      }
      LedCount++;
      if (LedCount > seg6){
          LedCount = 0;
          intensity = min_intensity; //intensity cant be < min_intensity
          operation_mode = mode_auto_on;
          last_millis = millis();
          //Asegurarse que el timer esta parado

          //load the time
           //UVtimer.setCounter(Horas, Minutos, Segundos, UVtimer.COUNT_DOWN, onComplete);
          //uvsafe_timer.setCounter(0, tiempo_exposicion, 0, uvsafe_timer.COUNT_DOWN, uvsafe_timer_Complete);
          uvsafe_timer.setCounter(0, 0, 10, uvsafe_timer.COUNT_DOWN, uvsafe_timer_Complete);
          uvsafe_timer.start(); //Activa la cuenta regresiva
      }
  }
}

  void auto_pattern(){
    uvsafe_timer.run();          
    if ((millis() - last_millis) > fadeDelay){
      last_millis = millis();

      for (int i=seg0; i<seg6; i++){
        leds[i] = CRGB::Purple;
        }

    FastLED.show(intensity);

    if ( (intensity > max_intensity) || (intensity <= min_intensity)){
      toggle_inc = !toggle_inc;
    }

    if (toggle_inc) {
      intensity++;
    }
    else {
      intensity--;
    }
  }
  }

  void manual_push_pattern(){
    if ((millis() - last_millis) > fadeDelay){
      last_millis = millis();

      for (int i=seg0; i<seg6; i++){
        leds[i] = CRGB::Red;
        }

    FastLED.show(intensity);

    if ( (intensity > max_intensity) || (intensity <= min_intensity)){
      toggle_inc = !toggle_inc;
    }

    if (toggle_inc) {
      intensity++;
    }
    else {
      intensity--;
    }
  }
  }

  void manual_pattern(SENSOR_STRUCT sensors){
    if(sensors.magnetic_1 == 1){
      for(int i=seg0; i<seg1; i++){
        leds[i] = CRGB::Green;
      }
      }
    else{
      for(int i=seg0; i<seg1; i++){
        leds[i] = CRGB::Red;
      }
      }

    if(sensors.magnetic_2 == 1){
      for(int i=seg1; i<seg2; i++){
        leds[i] = CRGB::Green;
      }
      }
    else{
      for(int i=seg1; i<seg2; i++){
        leds[i] = CRGB::Red;
      }
    }

    if(sensors.magnetic_3 == 1){
      for(int i=seg2; i<seg3; i++){
        leds[i] = CRGB::Green;
      }
    }
    else{
      for(int i=seg2; i<seg3; i++){
        leds[i] = CRGB::Red;
      }
    }

    if(sensors.magnetic_4 == 1){
      for(int i=seg3; i<seg4; i++){
        leds[i] = CRGB::Green;
      }
    }
    else{
      for(int i=seg3; i<seg4; i++){
        leds[i] = CRGB::Red;
      }
    }

    if(sensors.magnetic_5 == 1){
      for(int i=seg4; i<seg5; i++){
        leds[i] = CRGB::Green;
      }
    }
    else{
      for(int i=seg4; i<seg5; i++){
        leds[i] = CRGB::Red;
      }
    }

    if(sensors.magnetic_6 == 1){
      for(int i=seg5; i<seg6; i++){
        leds[i] = CRGB::Green;
      }
    }
    else{
      for(int i=seg5; i<seg6; i++){
        leds[i] = CRGB::Red;
      }
    }
    FastLED.show(64);
  }

  void confirm_push(bool on){
    if(on){
    for (int i=seg0; i<seg6; i++){
      leds[i] = CRGB::DeepPink;
      }
     FastLED.show(max_intensity);
   }
    else
    {
    for (int i=seg0; i<seg6; i++){
      leds[i] = CRGB::Black;
      }
     FastLED.show(min_intensity);
   }

  }
};

void uvsafe_timer_Complete(void){
  Serial.print("timer end");
  operation_mode = mode_manual;
  uvsafe_timer.stop();
}
