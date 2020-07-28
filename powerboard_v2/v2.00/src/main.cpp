/*
 *  by: MRO for IWI
 *  Todos los derechos reservados
 *  Aguascalientes, Mexico. Mayo 2020
 */
#include <avr/wdt.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include "uvsafe_version_control.h"
#include "HAL.h"
#include "uvsafe_user_definitions.h"
#include "uvsafe_read.h"
#include "uvsafe_write.h"
#include "uvsafe_comm.h"
#include "uvsafe_rgb_leds.h"
#include "uvsafe_safety.h"
#include "uvsafe_user_button.h"
#include "uvsafe_manual.h"
#include "uvsafe_auto.h"

Adafruit_MCP23017 gpio;
SoftwareSerial GUISerial(2, 3); // RX, TX
DataSender SerialDataSender(sensor_state);
RGBLeds LedsIndicadores(mode_manual); //inicializa siempre en manual
ReadSensors sensors(sensor_state, gpio);


void setup() {
        delay(1500); //for system stabilization
        wdt_disable(); //deshabilitar watchdog para no tener interrupciones
        wdt_enable(WDTO_4S);// nunca usar menos de 250 ms si no se va a resetar sin control
        init_gpio(gpio);
        Serial.begin(115200); //Regular serial port -- Terminal/debug/program
        GUISerial.begin(115200); //Serial port for GUI
        auto_button.debounceTime = AUTO_DEBOUNCE;
        operation_mode = mode_boot;
}


void loop() {
        wdt_reset(); //ping al watchdog
        sensor_state = sensors.read_sensors(operation_mode);
        SerialDataSender.Update(sensor_state);
        LedsIndicadores.Update(operation_mode, sensor_state);
        user_button_update(LedsIndicadores);
        safety_functions();
        lamparas_manual(gpio);
        lamparas_auto(gpio);
}
