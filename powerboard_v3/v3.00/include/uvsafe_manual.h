
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */


void lamparas_manual(Adafruit_MCP23017 gpio){
        //control del led de los deadman
        if (sensor_state.magnetic_status == 0) lamp_state.lamp_deadman = 1;
        else lamp_state.lamp_deadman = 0;
        //Control de las lamparas uv
        if((operation_mode == mode_manual) || (operation_mode == mode_manual_push)) {
                if((sensor_state.deadman1_sw == 0) && (sensor_state.deadman2_sw == 0) && (sensor_state.magnetic_status == 0)) {
                        lamp_state.lamp_1 = 1;
                        lamp_state.lamp_2 = 1;
                        lamp_state.lamp_3 = 1;
                        lamp_state.lamp_4 = 1;
                        lamp_state.lamp_5 = 1;
                        lamp_state.lamp_6 = 0;// En modo MANUAL apagado por default
                        operation_mode = mode_manual_push;
                }
                else{
                        lamp_state.lamp_1 = 0;
                        lamp_state.lamp_2 = 0;
                        lamp_state.lamp_3 = 0; // en modo manual apagado por default
                        lamp_state.lamp_4 = 0; // en modo manual apagado por default
                        lamp_state.lamp_5 = 0;
                        lamp_state.lamp_6 = 0;
                        operation_mode = mode_manual;
                }
                write_lamps(lamp_state, gpio);
        }
}
