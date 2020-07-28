
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
        case mode_boot:
                boot_pattern();
                break;
        case mode_manual:
                if(previous_mode != current_mode) {
                        intensity = 0;
                        LedCount = 0;
                        FastLED.show(intensity);
                        previous_mode = current_mode;
                }
                manual_pattern(sensors);

                break;
        case mode_auto_init:
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


void boot_pattern(){
        for (int i=0; i<NUM_LEDS; i++) {
                for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Black;
                leds[i] = CRGB::Purple;
                FastLED.show(int(max_intensity/3.0)); //estaba muy intenso
                delay(50);
        }

        for (int i=NUM_LEDS; i>=0; i--) {
                for (int j=0; j<NUM_LEDS; j++) leds[j] = CRGB::Black;
                leds[i] = CRGB::Purple;
                FastLED.show(int(max_intensity/3.0)); //estaba muy intenso
                delay(50);
        }
        operation_mode = mode_manual;
        last_pir_time = millis();
        pir_timeout = false;
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
                        intensity+=INTENSITY_INC; //INTENSITY_INC definido en uvsafe_user_definitions
                }
                else {
                        intensity-=INTENSITY_INC;
                }
        }
}

void manual_push_pattern(){
        if ((millis() - last_millis) > fadeDelay) {
                last_millis = millis();

                for (int i=seg0; i<seg6; i++) {
                        leds[i] = CRGB::Purple;
                }

                FastLED.show(intensity);

                if ( (intensity > max_intensity) || (intensity <= min_intensity)) {
                        toggle_inc = !toggle_inc;
                }

                if (toggle_inc) {
                        intensity+=INTENSITY_INC;
                }
                else {
                        intensity-=INTENSITY_INC;
                }
        }


}

void manual_pattern(SENSOR_STRUCT sensors){
        if (!pir_timeout) {  //Si el timeout de los pir no esta activo haz normal
                if(timer_count == 0) {
                        if(sensors.magnetic_6 == 0) {
                                for(int i=seg0; i<seg6; i++) {
                                        leds[i] = CRGB::Green;
                                }
                        }
                        else{
                                for(int i=seg0; i<seg6; i++) {
                                        leds[i] = CRGB::Red;
                                }
                        }

                }
                else{

                        for (unsigned long i=0; i<timer_count; i++) {
                                leds[i] = CRGB::DarkMagenta;
                        }

                        for (int i=timer_count; i<NUM_LEDS; i++) {
                                leds[i] = CRGB::Black;
                        }

                        FastLED.show(int(max_intensity)); //demasiado intenso
                }
        }
        else{ //si llegamos aqui por que el PIR llego a un timeout, entonces avisa (narajna)
              // y espera a que se resetie el color apretando cualquier boton
                if ((digitalRead(DEADMAN_Pin) == 0) || (auto_button.clicks != 0)) pir_timeout = false;
                for(int i=seg0; i<seg6; i++) {
                        leds[i] = CRGB::OrangeRed;
                }
        }
        FastLED.show(64);
}

void confirm_push(bool on){
        if(on) {
                for (int i=seg0; i<seg6; i++) {
                        leds[i] = CRGB::DeepSkyBlue;
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
