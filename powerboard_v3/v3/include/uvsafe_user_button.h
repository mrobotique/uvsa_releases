
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */


#define CINCO_MINUTOS 300 //en segundos
int last_beep = 1;


void user_button_update(RGBLeds LedsIndicadores, Adafruit_MCP23017 gpio) {

  if (sensor_state.magnetic_status == 1){
          lamp_state.lamp_auto = 1;
          auto_button.Update();
          safety_functions();
          if (sensor_state.auto_button == 0) {
            if (last_beep != 0){
              last_beep = sensor_state.auto_button;
              beeper.Trigger(ONE_BEEP);
            }
          }
          else
            last_beep = sensor_state.auto_button;


          if (timer_count == 0) last_click_update = millis();
          // Si el switch no es accionado en TIMEOUT_CLICK tiempo el contador se restablece
          else if ((millis() - last_click_update) > TIMEOUT_CLICK) timer_count = 0;

          if (operation_mode == mode_manual) {
                  // Si el usuario ejecuta un push largo
                  if (auto_button.clicks == -1 && auto_button.depressed == true)
                  {
                          beeper.Trigger(TWO_BEEP);
                          LedsIndicadores.confirm_push(true);
                          if (timer_count == 0) timer_count = 1; //El tiempo minimo a enviar son cinco minutos
                          nuevo_tiempo_exposicion = timer_count * CINCO_MINUTOS;
                          tiempo_restante = nuevo_tiempo_exposicion;
                          timer_count = 0;
                          while (auto_button.depressed) { //No hacer nada mientras el usuario no suelta el boton
                                  gpio.digitalWrite(BUZZER,beeper.Update());
                                  auto_button.Update();
                          }

                          LedsIndicadores.confirm_push(false);
                          operation_mode = mode_auto_init;
                  }
                  if ((digitalRead(DEADMAN1_Pin) == 0) || (digitalRead(DEADMAN2_Pin) == 0)) timer_count = 0; //si el usuario se arrepiente... le pica al deadman y regresamos a manual normal
                  if ((auto_button.clicks > 0) && (auto_button.changed) && !pir_timeout && (previous_mode != mode_auto_on) &&
                     (previous_mode != mode_auto_init) && (auto_button.depressed == false)){
                          timer_count += 1;
                          if (timer_count > NUM_LEDS) timer_count = 1;
                  }
          }
          else{
                  timer_count = 0;
                  safety_functions();
          }
  }
else{
  lamp_state.lamp_auto = 0;
  }
}
