
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */


void check_pir(){
        if ((operation_mode != mode_manual) && (operation_mode != mode_boot)){
                if ((millis() - last_pir_time) > MAX_PIR_TIME_BEFORE_MANUAL) {
                        pir_timeout = true;
                        operation_mode = mode_manual;
                }
                if(sensor_state.pir_status == 0) last_pir_time = millis();
        }
        else{
          last_pir_time = millis();
        }
}


void safety_functions(void){
//Si algun sensor manual cambia de estado debemos apagar todo y pasar a manual_pattern
        if ((digitalRead(DEADMAN1_Pin) == 0) || (digitalRead(DEADMAN2_Pin) == 0)) {
          if ((operation_mode == mode_auto_on) || (operation_mode == mode_auto_init)) beeper.Trigger(BEEP_OFF);
          if (beeper.CurrentMode() == FIN_DE_CICLO) beeper.Trigger(ONE_BEEP);
          operation_mode = mode_manual;
        }
        if ((operation_mode != mode_manual) && (auto_button.clicks != 0)) {
          operation_mode = mode_manual;
          beeper.Trigger(BEEP_OFF);
        }
        if ((operation_mode != mode_manual) && (auto_button.clicks >= 1)) {
          operation_mode = mode_manual;
          beeper.Trigger(BEEP_OFF);
        }
        check_pir();
}
