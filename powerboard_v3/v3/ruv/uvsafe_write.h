void write_lamps(LAMP_STRUCT lamp_state, Adafruit_MCP23017 gpio){
  /*
  Esta funcion recibe el estado actual de las lamparas y lo transmite
  a la gpio
  */
  gpio.digitalWrite(LAMP1, lamp_state.lamp_1);
  gpio.digitalWrite(LAMP2, lamp_state.lamp_2);
  gpio.digitalWrite(LAMP3, lamp_state.lamp_3);
  gpio.digitalWrite(LAMP4, lamp_state.lamp_4);
  gpio.digitalWrite(LAMP5, lamp_state.lamp_5);
  gpio.digitalWrite(LAMP6, lamp_state.lamp_6);
  gpio.digitalWrite(LAMP7, lamp_state.lamp_7);
  gpio.digitalWrite(LAMP8, lamp_state.lamp_8);
}

void chenille_test(Adafruit_MCP23017 gpio){
  static int i = 0;
  for (int j=0; j<=7; j++){
    gpio.digitalWrite(j, 0);
  }
  gpio.digitalWrite(i, 1);
  i++;
  if(i>7){
    i=0;
  }
}
