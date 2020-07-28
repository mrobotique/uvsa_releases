#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "HAL.h"
#include <SoftwareSerial.h>
#include "uvsafe_read.h"
#include "uvsafe_write.h"
#include "uvsafe_comm.h"
#include "uvsafe_rgb_leds.h"
#include "uvsafe_safety.h"
#include "uvsafe_user_button.h"
#include "uvsafe_manual.h"

Adafruit_MCP23017 gpio;
SoftwareSerial GUISerial(2, 3); // RX, TX
DataSender SerialDataSender(sensor_state);
RGBLeds LedsIndicadores(mode_manual); //inicializa siempre en manual
ReadSensors sensors(sensor_state, gpio);




void setup() {
  init_gpio(gpio);
  Serial.begin(115200); //Regular serial port -- Terminal/debug/program
  GUISerial.begin(115200); //Serial port for GUI
  operation_mode = mode_manual;

  }

void loop() {
  // put your main code here, to run repeatedly:
  sensor_state = sensors.read_sensors();
  sensors.print_sensor_state();
  //SerialDataSender.Update(sensor_state);
  chenille_test(gpio);
  LedsIndicadores.Update(operation_mode, sensor_state);
  //user_button_update(LedsIndicadores);
  safety_functions();
  //lamparas_manual(gpio);
  uvsafe_timer.run(); //tick para el timer
  delay(300); //Debug para los relays

}

void tUpComplete(void){}
