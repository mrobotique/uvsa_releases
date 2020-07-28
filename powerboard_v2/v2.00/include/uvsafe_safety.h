/*
 *  by: MRO for IWI
 *  Todos los derechos reservados
 *  Aguascalientes, Mexico. Mayo 2020
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
        if (digitalRead(DEADMAN_Pin) == 0) operation_mode = mode_manual;
        if ((operation_mode != mode_manual) && (auto_button.clicks != 0)) operation_mode = mode_manual;
        if ((operation_mode != mode_manual) && (auto_button.clicks >= 1)) operation_mode = mode_manual;
        check_pir();
}
