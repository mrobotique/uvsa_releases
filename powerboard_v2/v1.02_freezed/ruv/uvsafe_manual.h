void lamparas_manual(Adafruit_MCP23017 gpio){
  if((operation_mode == mode_manual) || (operation_mode == mode_manual_push)) {
    if(sensor_state.deadman_sw == 0){
      lamp_state.lamp_1 = 1 * sensor_state.magnetic_1;
      lamp_state.lamp_2 = 1 * sensor_state.magnetic_2;
      lamp_state.lamp_3 = 0; // en modo manual apagado por default
      lamp_state.lamp_4 = 0; // en modo manual apagado por default
      lamp_state.lamp_5 = 1 * sensor_state.magnetic_5;
      lamp_state.lamp_6 = 1 * sensor_state.magnetic_6;
      lamp_state.lamp_7 = 0; //relay auxiliar para futuras aplicaciones
      lamp_state.lamp_8 = 0; // relay auxiliar para futuras aplicaciones
      operation_mode = mode_manual_push;
    }
    else{
      lamp_state.lamp_1 = 0;
      lamp_state.lamp_2 = 0;
      lamp_state.lamp_3 = 0; // en modo manual apagado por default
      lamp_state.lamp_4 = 0; // en modo manual apagado por default
      lamp_state.lamp_5 = 0;
      lamp_state.lamp_6 = 0;
      lamp_state.lamp_7 = 0; //relay auxiliar para futuras aplicaciones
      lamp_state.lamp_8 = 0; // relay auxiliar para futuras aplicaciones
      operation_mode = mode_manual;
    }
    write_lamps(lamp_state, gpio);
  }
}
