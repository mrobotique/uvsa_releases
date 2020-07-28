/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */

const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_SERIAL1_BUFFER_SIZE + 1; //JSON_SERIAL1_BUFFER_SIZE definido en HAL.h

void serialEvent() {
  while (Serial.available()) {
    // recibe el primer byte:
    char inChar = (char)Serial.read();
    // Lo agrega al string:
    if (int(inChar) != 0){
       inputString += inChar; //Descarta un caracter nulo que envia el bt
    }
    // Si el caracter que llega es "newline" (\n) 
    // el string esta completo y hacemos algo con el.
    if (inChar == '\n') {
        // Si el aparato esta en manual entonces acepta el dato que viene por el puero serie.
        
        if (operation_mode == mode_manual){
            DynamicJsonDocument doc(capacity);
            DeserializationError error = deserializeJson(doc, inputString);
            if (error || (sensor_state.magnetic_status != 1)){
                beeper.Trigger(ERROR);
                
            }
            else{
                beeper.Trigger(TWO_BEEP);
                nuevo_tiempo_exposicion = doc["time"]; 
                tiempo_restante = nuevo_tiempo_exposicion;
                for (int i=timer_count; i<NUM_LEDS; i++) {
                leds[i] = CRGB::Black;
                }
                FastLED.show(0); //apagar los leds
                operation_mode = mode_auto_init;
        }
    } 
    //Si no esta en manual, apaga todo y se pone en modo manual
    else{
        beeper.Trigger(ONE_BEEP);
        operation_mode = mode_manual;
        }
    inputString = "";
    }
  }
}