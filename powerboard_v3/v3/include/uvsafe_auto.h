/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */
//Prototipo
void uvsafe_timer_Complete(void);

//Instancia de timer. Exposition time esta definido en uvsafe_user_definitions
Countdown expo_timer(EXPOSITION_TIME, uvsafe_timer_Complete);  //300 seg (5min) para probar



void lamparas_auto(Adafruit_MCP23017 gpio){
        if ((operation_mode == mode_auto_on) && (sensor_state.magnetic_status == 1)) {
                if (expo_timer.is_running() != true) {
                        expo_timer.reset();
                        expo_timer.new_time(nuevo_tiempo_exposicion); //nuevo_tiempo_exposicion esta definido en uvsafe_user_button.h
                        timer_count = 0;
                        expo_timer.start();
                }
                expo_timer.run();
                if (sensor_state.pir_status != 0) {
                        expo_timer.pause();
                        lamp_state.lamp_1 = 0;
                        lamp_state.lamp_2 = 0;
                        lamp_state.lamp_3 = 0;
                        lamp_state.lamp_4 = 0;
                        lamp_state.lamp_5 = 0;
                        lamp_state.lamp_6 = 0;// En modo MANUAL apagado por default
                }
                else{
                        expo_timer.start();
                        tiempo_restante = expo_timer.get_time() - 1;//se le resta uno para que el t_min == 0.. cuestiones de logica de programacion
                        lamp_state.lamp_1 = 1;
                        lamp_state.lamp_2 = 1;
                        lamp_state.lamp_3 = 1;
                        lamp_state.lamp_4 = 1;
                        lamp_state.lamp_5 = 1;
                        lamp_state.lamp_6 = 1;
                }
                write_lamps(lamp_state, gpio);
        }
        else{
                expo_timer.reset();
                if (operation_mode != mode_auto_init){
                tiempo_restante = 0;
                }
        }
}

void uvsafe_timer_Complete(void){
        expo_timer.reset();
        operation_mode = mode_manual;
        beeper.Trigger(FIN_DE_CICLO);
}
