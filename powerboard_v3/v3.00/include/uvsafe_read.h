
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */


class After_Pir {
unsigned long last_update;
unsigned long time_delay;
bool pir_state = true;

public:
After_Pir(unsigned long _time_delay){
        time_delay = _time_delay;
}

bool run (int sensor_){
        if (sensor_ != 0) {
                pir_state = false;
                last_update = millis();

        }
        else{
                // si ya esperamos mas de lo que queremos esperar para reactivar las lamparas

                if((millis() - last_update) > time_delay) {
                        pir_state = true;
                }
        }
        return !pir_state; //logica inversa por que el contacto es NC
}
};



class Debounce {
int ledState = HIGH;             // the current state of the output pin
int buttonState = -1;                 // the current reading from the input pin
int lastButtonState = LOW;       // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;      // the last time the output pin was toggled
unsigned long debounceDelay = 50;        // the debounce time; increase if the output flickers

public:
Debounce(long _debounceDelay) {
        debounceDelay = _debounceDelay;
}

int Update(int _reading) {
        int reading = _reading;
        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonState) {
                // reset the debouncing timer
                lastDebounceTime = millis();
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
                // whatever the reading is at, it's been there for longer than the debounce
                // delay, so take it as the actual current state:
                // if the button state has changed:
                if (reading != buttonState) {
                        buttonState = reading;
                        // only toggle the LED if the new button state is HIGH
                        ledState = reading;
                }
        }
        lastButtonState = reading;
        return ledState;
}
};


class ReadSensors {
SENSOR_STRUCT sensor_state;
Adafruit_MCP23017 gpio;
//Tiempos definidos en uvsafe_user_definitions
int MagneticDebouncingTime = MAGNETIC_DEBOUNCE; //ms
int PirDebouncingTime = PIR_DEBOUNCE; //ms
int DeadmanDebouncingTime = DEADMAN_DEBOUNCE; //ms

Debounce mag_deb_1 = Debounce(MagneticDebouncingTime);
Debounce mag_deb_2 = Debounce(MagneticDebouncingTime);

Debounce pir_deb_1 = Debounce(PirDebouncingTime);
Debounce pir_deb_2 = Debounce(PirDebouncingTime);
Debounce pir_deb_3 = Debounce(PirDebouncingTime);
Debounce pir_deb_4 = Debounce(PirDebouncingTime);

//Debounce para los beeps
//Todos tiene 10 ms mas que lo necesario para hacer el dbounce del beep
//sea mas largo que el debounce del boton y asi, cuando el beep suena, es
//seguro que el boton ya se registro.
Debounce auto_button_deb = Debounce(AUTO_DEBOUNCE + 10); //Este esta fijo por que solo quiero hacer un peque~o debounce fijo
Debounce beep_deadman1_deb = Debounce(DEADMAN_DEBOUNCE + 10); //Este esta fijo por que solo quiero hacer un peque~o debounce fijo
Debounce beep_deadman2_deb = Debounce(DEADMAN_DEBOUNCE + 10); //Este esta fijo por que solo quiero hacer un peque~o debounce fijo


Debounce deadman_deb1 = Debounce(DeadmanDebouncingTime);
Debounce deadman_deb2 = Debounce(DeadmanDebouncingTime);

After_Pir after_pir = After_Pir(AFTER_PIR);

public:
ReadSensors(SENSOR_STRUCT _sensor_state, Adafruit_MCP23017 _gpio) {
        gpio = _gpio;
        sensor_state = _sensor_state;
}

int last_beep_deadman = 1;
int last_beep_deadman1 = 1;
int last_beep_deadman2 = 1;
int beep_deadman1 = 1;
int beep_deadman2 = 1;

SENSOR_STRUCT read_sensors(uvs_mode current_mode) {
        /*
           This function reads all the declared sensors
           :return: struct conteniendo el estado de los sensores
           :rtype: SENSOR_STRUCT
         */
        sensor_state.auto_button = auto_button_deb.Update(digitalRead(AUTO_Pin));
        sensor_state.deadman1_sw = deadman_deb1.Update(digitalRead(DEADMAN1_Pin));
        sensor_state.deadman2_sw = deadman_deb2.Update(digitalRead(DEADMAN2_Pin));
        beep_deadman1 = beep_deadman1_deb.Update(digitalRead(DEADMAN1_Pin));
        beep_deadman2 = beep_deadman2_deb.Update(digitalRead(DEADMAN2_Pin));

        if (sensor_state.magnetic_status == 0){
          if (!(sensor_state.deadman1_sw && sensor_state.deadman2_sw)) {
            if (last_beep_deadman != 0){
              last_beep_deadman = (sensor_state.deadman1_sw && sensor_state.deadman2_sw);
              beeper.Trigger(TWO_BEEP);
            }
          }
          else
            last_beep_deadman = (sensor_state.deadman1_sw && sensor_state.deadman2_sw);

            if (!beep_deadman1) {
              if (last_beep_deadman1 != 0){
                last_beep_deadman1 = beep_deadman1;
                beeper.Trigger(ONE_BEEP);
              }
            }
            else
              last_beep_deadman1 = beep_deadman1;

              if (!beep_deadman2) {
                if (last_beep_deadman2 != 0){
                  last_beep_deadman2 = beep_deadman2;
                  beeper.Trigger(ONE_BEEP);
                }
              }
              else
                last_beep_deadman2 = beep_deadman2;
        }

        sensor_state.pir_1 = pir_deb_1.Update(digitalRead(PIR1_Pin));
        sensor_state.pir_2 = pir_deb_2.Update(digitalRead(PIR2_Pin));
        sensor_state.pir_3 = pir_deb_3.Update(digitalRead(PIR3_Pin));
        sensor_state.pir_4 = pir_deb_4.Update(digitalRead(PIR4_Pin));
        sensor_state.pir_transition = sensor_state.pir_1 + sensor_state.pir_2 + sensor_state.pir_3 + sensor_state.pir_4;
        if (current_mode == mode_auto_on)
          sensor_state.pir_status = after_pir.run(sensor_state.pir_transition);
        else
          sensor_state.pir_status =   sensor_state.pir_transition;

        sensor_state.magnetic_1 = mag_deb_1.Update(gpio.digitalRead(MAGNETIC1));
        sensor_state.magnetic_2 = mag_deb_2.Update(gpio.digitalRead(MAGNETIC2));
        if((sensor_state.magnetic_1 == 0 ) and (sensor_state.magnetic_2 == 0)){
          sensor_state.magnetic_status = 0;
        }
        else if ((sensor_state.magnetic_1 == 1 ) and (sensor_state.magnetic_2 == 1)){
          sensor_state.magnetic_status = 1;
        }
        else{
          sensor_state.magnetic_status = -1;
        }
        sensor_state.lamp_1 = gpio.digitalRead(LAMP1);
        sensor_state.lamp_2 = gpio.digitalRead(LAMP2);
        sensor_state.lamp_3 = gpio.digitalRead(LAMP3);
        sensor_state.lamp_4 = gpio.digitalRead(LAMP4);
        sensor_state.lamp_5 = gpio.digitalRead(LAMP5);
        sensor_state.lamp_6 = gpio.digitalRead(LAMP6);
        sensor_state.lamp_deadman = gpio.digitalRead(LAMP_DEADMAN);
        sensor_state.lamp_auto = gpio.digitalRead(LAMP_AUTO);
        return sensor_state;
}

void print_sensor_state() {
        Serial.print("auto: ");
        Serial.print(digitalRead(AUTO_Pin));
        Serial.print(" | deadman_sw: ");
        Serial.print(sensor_state.deadman1_sw);
        Serial.print(sensor_state.deadman2_sw);
        Serial.print(" | PIR :");
        Serial.print(sensor_state.pir_1);
        Serial.print(sensor_state.pir_2);
        Serial.print(sensor_state.pir_3);
        Serial.print(sensor_state.pir_4);
        Serial.print(" | mag: ");
        Serial.print(sensor_state.magnetic_1);
        Serial.print(sensor_state.magnetic_2);
        Serial.print(" | lamps: ");
        Serial.print(sensor_state.lamp_1);
        Serial.print(sensor_state.lamp_2);
        Serial.print(sensor_state.lamp_3);
        Serial.print(sensor_state.lamp_4);
        Serial.print(sensor_state.lamp_5);
        Serial.print(sensor_state.lamp_6);
}
};
