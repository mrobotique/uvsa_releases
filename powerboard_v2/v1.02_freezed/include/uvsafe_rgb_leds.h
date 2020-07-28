
/*
 *  by: MRO for IWI
 *  Todos los derechos reservados
 *  Aguascalientes, Mexico. Mayo 2020
 */


#include <FastLED.h>
#include "uvsafe_time.h"
//Las definiciones por led por segemnto estan declaradas en uvsafe_user_definitions.h

int previous_mode;
int LedCount = 0;
//Delay para el fade in fade out
unsigned long fadeDelay = 15; //en mS
unsigned long last_millis = millis();
// max_intensity y min_intensity estan definidos en uvsafe_user_definitions

int intensity = min_intensity; //intensity no puede ser menor que min intensity
                               //en caso de que se necesite una intensidad menor
                               //se puede usar pero al salir de la condicion donde se necesita
                               //esta intensidad menor entonces la intensidad se debe igualar
                               //a min_intensity
bool toggle_inc = false; //controla el fade in fade out


class RGBLeds {
int current_mode; //Modo en el que se encuentra el uvs
unsigned long previousMillis;
//tiempo de inicio (cuenta regresiva)
unsigned long init_time = TIEMPO_INICIO; //definido en uvsafe_user_definitions
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
                if(previous_mode != current_mode) {
                        intensity = 0;
                        FastLED.show(intensity);
                        previous_mode = current_mode;
                }
                manual_pattern(sensors);

                break;
        case mode_auto_init:
                //TODO
                if(previous_mode != current_mode) {
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
                if(previous_mode != current_mode) {
                        intensity = min_intensity; //intensity cant be < min_intensity
                        previous_mode = current_mode;
                        toggle_inc = false;
                }
                auto_pattern();
                break;

        case mode_manual_push:
                if(previous_mode != current_mode) {
                        intensity = min_intensity; //intensity cant be < min_intensity
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
        } //case
}

void init_pattern(){

        if ((millis() - last_millis) > init_time_segment) {
                last_millis = millis();
                if (LedCount<seg6) {
                        leds[LedCount] = CRGB::DeepSkyBlue;
                        FastLED.show(int(max_intensity/3.0)); //estaba muy intenso
                }
                LedCount++;
                if (LedCount > seg6) {
                        LedCount = 0;
                        intensity = min_intensity; //intensity cant be < min_intensity
                        operation_mode = mode_auto_on;
                        last_millis = millis();
                        //Asegurarse que el timer esta parado
                }
        }
}

void auto_pattern(){

        if ((millis() - last_millis) > fadeDelay) {
                last_millis = millis();

                for (int i=seg0; i<seg6; i++) {
                        if(sensor_state.pir_status != 0) {
                                if(sensor_state.pir_transition != 0)
                                  //Si los PIR detectan a alguien, entonces amarillo y la deteccion esta activa
                                  leds[i] = CRGB::OrangeRed;
                                else //La deteccion no esta activa y pronto se van a reiniciar la UV
                                  leds[i] = CRGB::DeepSkyBlue;
                        }
                        else{
                                //Si los PIR estan  ok entonces es morado
                                leds[i] = CRGB::Purple;
                        }
                }

                FastLED.show(intensity);

                if ( (intensity > max_intensity) || (intensity <= min_intensity)) {
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
        if ((millis() - last_millis) > fadeDelay) {
                last_millis = millis();

                for (int i=seg0; i<seg6; i++) {
                        leds[i] = CRGB::Red;
                }

                FastLED.show(intensity);

                if ( (intensity > max_intensity) || (intensity <= min_intensity)) {
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
        if(sensors.magnetic_1 == 1) {
                for(int i=seg0; i<seg1; i++) {
                        leds[i] = CRGB::Green;
                }
        }
        else{
                for(int i=seg0; i<seg1; i++) {
                        leds[i] = CRGB::Red;
                }
        }

        if(sensors.magnetic_2 == 1) {
                for(int i=seg1; i<seg2; i++) {
                        leds[i] = CRGB::Green;
                }
        }
        else{
                for(int i=seg1; i<seg2; i++) {
                        leds[i] = CRGB::Red;
                }
        }

        if(sensors.magnetic_3 == 1) {
                for(int i=seg2; i<seg3; i++) {
                        leds[i] = CRGB::Green;
                }
        }
        else{
                for(int i=seg2; i<seg3; i++) {
                        leds[i] = CRGB::Red;
                }
        }

        if(sensors.magnetic_4 == 1) {
                for(int i=seg3; i<seg4; i++) {
                        leds[i] = CRGB::Green;
                }
        }
        else{
                for(int i=seg3; i<seg4; i++) {
                        leds[i] = CRGB::Red;
                }
        }

        if(sensors.magnetic_5 == 1) {
                for(int i=seg4; i<seg5; i++) {
                        leds[i] = CRGB::Green;
                }
        }
        else{
                for(int i=seg4; i<seg5; i++) {
                        leds[i] = CRGB::Red;
                }
        }

        if(sensors.magnetic_6 == 1) {
                for(int i=seg5; i<seg6; i++) {
                        leds[i] = CRGB::Green;
                }
        }
        else{
                for(int i=seg5; i<seg6; i++) {
                        leds[i] = CRGB::Red;
                }
        }
        FastLED.show(64);
}

void confirm_push(bool on){
        if(on) {
                for (int i=seg0; i<seg6; i++) {
                        leds[i] = CRGB::DeepPink;
                }
                FastLED.show(int(max_intensity/3.0)); //demasiado intenso
        }
        else
        {
                for (int i=seg0; i<seg6; i++) {
                        leds[i] = CRGB::Black;
                }
                FastLED.show(min_intensity);
        }

}
};
