
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */

#include <avr/wdt.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_MCP23017.h>
#include "HAL.h"
#include "uvsafe_version_control.h"
#include "uvsafe_user_definitions.h"
#include "uvsafe_read.h"
#include "uvsafe_write.h"
#include "uvsafe_comm.h"
#include "uvsafe_rgb_leds.h"
#include "uvsafe_safety.h"
#include "uvsafe_user_button.h"
#include "uvsafe_manual.h"
#include "uvsafe_auto.h"
#include "uvsafe_init_test.h"
#include "uvsafe_read_serial_1.h"


Adafruit_MCP23017 gpio;
SoftwareSerial GUISerial(2, 3); // RX, TX
DataSender SerialDataSender(sensor_state);
RGBLeds LedsIndicadores(mode_manual); //inicializa siempre en manual
ReadSensors sensors(sensor_state, gpio);

void setup() {
        bool run_test = false;
        bool configure_buzzer = false;
        int addr = 0; //eeprom address to save the data
        bool BUZZER_ENABLED_BUFFER;

        wdt_disable(); //deshabilitar watchdog para no tener interrupciones
        init_gpio(gpio);
        Serial.begin(115200); //Regular serial port -- Terminal/debug/program
        GUISerial.begin(115200); //Serial port for GUI
        operation_mode = mode_boot;
        activity_led.Update_mode(3);
        auto_button.debounceTime = AUTO_DEBOUNCE;

        //Hacer las configuraciones necesarias si el usuario esta apretando un boton
        //al momento de arrancar el aparato.
        //Primero, checamos el boton AUTO. si este boton esta apretado al enceder
        //el aparato entonces, al soltar todos los botones, se correra la rutina
        //de prueba de leds:
        if (!digitalRead(AUTO_Pin)) run_test = true;
        //Despues, checamos para el boton deadman 1. Si este esta apretado, entonces
        //el estado de "BUZZER_ENABLED" cambia, dependiend de su estado actual,
        // de falso a verdadero o viceversa.
        BUZZER_ENABLED_BUFFER = EEPROM.read(addr);
        if (!digitalRead(DEADMAN1_Pin)){
             configure_buzzer = true;
             if (EEPROM.read(addr) == false){
               BUZZER_ENABLED_BUFFER = true;
               EEPROM.write(addr, true);
             }
             else{
               BUZZER_ENABLED_BUFFER = false;
               EEPROM.write(addr, false);
             }

         }
        if (run_test || configure_buzzer){
        //Corre el test solo si el boton esta activado
          beeper.Trigger(ONE_BEEP); //Avisa
          BUZZER_ENABLED = true;
          while (!digitalRead(AUTO_Pin) || !digitalRead(DEADMAN1_Pin)) //bloquea el asunto hasta que los botones se sueltan
          {
            gpio.digitalWrite(BUZZER,beeper.Update());
          }
          if (run_test == true) init_test(gpio);
        }
        beeper.Trigger(TWO_BEEP);        
        // reserva JSON_SERIAL1_BUFFER_SIZE bytes para el JSON de entrada del puerto serie 1 (control BT)
        //JSON_SERIAL1_BUFFER_SIZE definido en HAL
        inputString.reserve(JSON_SERIAL1_BUFFER_SIZE);        
        wdt_enable(WDTO_4S);// nunca usar menos de 250 ms si no se va a resetar sin control
        BUZZER_ENABLED = BUZZER_ENABLED_BUFFER;
}

void loop() {
        wdt_reset(); //ping al watchdog
        sensor_state = sensors.read_sensors(operation_mode);
        SerialDataSender.Update(sensor_state);
        LedsIndicadores.Update(operation_mode, sensor_state);
        user_button_update(LedsIndicadores, gpio);
        safety_functions();
        lamparas_manual(gpio);
        lamparas_auto(gpio);
        activity_led.Update();
        gpio.digitalWrite(BUZZER,beeper.Update());
}