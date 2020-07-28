
/*
 *  by: MRO for IWI
 *  Todos los derechos reservados
 *  Aguascalientes, Mexico. Mayo 2020
 */

//Prototipo
void uvsafe_timer_Complete(void);
//Instancia de timer. Exposition time esta definido en uvsafe_user_definitions
Countdown expo_timer(EXPOSITION_TIME, uvsafe_timer_Complete);  //300 seg (5min) para probar


void lamparas_auto(Adafruit_MCP23017 gpio){
        if (operation_mode == mode_auto_on) {
                if (expo_timer.is_running() != true) {
                        expo_timer.reset();
                        expo_timer.start();
                }
                expo_timer.run();

                if (sensor_state.pir_status != 0) {
                        expo_timer.pause();
                        lamp_state.lamp_1 = 0;
                        lamp_state.lamp_2 = 0;
                        lamp_state.lamp_3 = 0; // en modo manual apagado por default
                        lamp_state.lamp_4 = 0; // en modo manual apagado por default
                        lamp_state.lamp_5 = 0;
                        lamp_state.lamp_6 = 0;
                        lamp_state.lamp_7 = 0; //relay auxiliar para futuras aplicaciones
                        lamp_state.lamp_8 = 0; // relay auxiliar para futuras aplicaciones
                }
                else{

                        expo_timer.start();
                        lamp_state.lamp_1 = 1 * sensor_state.magnetic_1;
                        lamp_state.lamp_2 = 1 * sensor_state.magnetic_2;
                        lamp_state.lamp_3 = 1 * sensor_state.magnetic_3;
                        lamp_state.lamp_4 = 1 * sensor_state.magnetic_4;
                        lamp_state.lamp_5 = 1 * sensor_state.magnetic_5;
                        lamp_state.lamp_6 = 1 * sensor_state.magnetic_6;
                        lamp_state.lamp_7 = 0; //relay auxiliar para futuras aplicaciones
                        lamp_state.lamp_8 = 0; // relay auxiliar para futuras aplicaciones
                }
                write_lamps(lamp_state, gpio);
        }
        else{
                expo_timer.reset();
        }
}

void uvsafe_timer_Complete(void){
        expo_timer.reset();
        operation_mode = mode_manual;
}
