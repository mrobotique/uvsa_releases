class Debounce {
    int ledState = HIGH;         // the current state of the output pin
    int buttonState = -1;             // the current reading from the input pin
    int lastButtonState = LOW;   // the previous reading from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

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
    int MagneticDebouncingTime = 800;
    int PirDebouncingTime = 300;
    int DeadmanDebouncingTime = 1000;

    Debounce mag_deb_1 = Debounce(MagneticDebouncingTime);
    Debounce mag_deb_2 = Debounce(MagneticDebouncingTime);
    Debounce mag_deb_3 = Debounce(MagneticDebouncingTime);
    Debounce mag_deb_4 = Debounce(MagneticDebouncingTime);
    Debounce mag_deb_5 = Debounce(MagneticDebouncingTime);
    Debounce mag_deb_6 = Debounce(MagneticDebouncingTime);

    Debounce pir_deb_1 = Debounce(PirDebouncingTime);
    Debounce pir_deb_2 = Debounce(PirDebouncingTime);
    Debounce pir_deb_3 = Debounce(PirDebouncingTime);
    Debounce pir_deb_4 = Debounce(PirDebouncingTime);

    Debounce deadman_deb = Debounce(DeadmanDebouncingTime);

  public:
    ReadSensors(SENSOR_STRUCT _sensor_state, Adafruit_MCP23017 _gpio) {
      gpio = _gpio;
      sensor_state = _sensor_state;
    }

    SENSOR_STRUCT read_sensors() {
      /*
        This function reads all the declared sensors
        :return: struct conteniendo el estado de los sensores
        :rtype: SENSOR_STRUCT
      */
      sensor_state.deadman_sw = deadman_deb.Update(digitalRead(DEADMAN_Pin));
      sensor_state.pir_1 = pir_deb_1.Update(digitalRead(PIR1_Pin));
      sensor_state.pir_2 = pir_deb_2.Update(digitalRead(PIR2_Pin));
      sensor_state.pir_3 = pir_deb_3.Update(digitalRead(PIR3_Pin));
      sensor_state.pir_4 = pir_deb_4.Update(digitalRead(PIR4_Pin));
      sensor_state.magnetic_1 = mag_deb_1.Update(gpio.digitalRead(MAGNETIC1));
      sensor_state.magnetic_2 = mag_deb_2.Update(gpio.digitalRead(MAGNETIC2));
      sensor_state.magnetic_3 = mag_deb_3.Update(gpio.digitalRead(MAGNETIC3));
      sensor_state.magnetic_4 = mag_deb_4.Update(gpio.digitalRead(MAGNETIC4));
      sensor_state.magnetic_5 = mag_deb_5.Update(gpio.digitalRead(MAGNETIC5));
      sensor_state.magnetic_6 = mag_deb_6.Update(gpio.digitalRead(MAGNETIC6));
      sensor_state.lamp_1 = gpio.digitalRead(LAMP1);
      sensor_state.lamp_2 = gpio.digitalRead(LAMP2);
      sensor_state.lamp_3 = gpio.digitalRead(LAMP3);
      sensor_state.lamp_4 = gpio.digitalRead(LAMP4);
      sensor_state.lamp_5 = gpio.digitalRead(LAMP5);
      sensor_state.lamp_6 = gpio.digitalRead(LAMP6);
      sensor_state.lamp_7 = gpio.digitalRead(LAMP7);
      sensor_state.lamp_8 = gpio.digitalRead(LAMP8);
      return sensor_state;
    }

    void print_sensor_state() {
      Serial.print("auto: ");
      Serial.print(digitalRead(AUTO_Pin));
      Serial.print(" | deadman_sw: ");
      Serial.print(sensor_state.deadman_sw);
      Serial.print(" | PIR :");
      Serial.print(sensor_state.pir_1);
      Serial.print(sensor_state.pir_2);
      Serial.print(sensor_state.pir_3);
      Serial.print(sensor_state.pir_4);
      Serial.print(" | mag: ");
      Serial.print(sensor_state.magnetic_1);
      Serial.print(sensor_state.magnetic_2);
      Serial.print(sensor_state.magnetic_3);
      Serial.print(sensor_state.magnetic_4);
      Serial.print(sensor_state.magnetic_5);
      Serial.print(sensor_state.magnetic_6);
      Serial.print(" | lamps: ");
      Serial.print(sensor_state.lamp_1);
      Serial.print(sensor_state.lamp_2);
      Serial.print(sensor_state.lamp_3);
      Serial.print(sensor_state.lamp_4);
      Serial.print(sensor_state.lamp_5);
      Serial.print(sensor_state.lamp_6);
      Serial.print(sensor_state.lamp_7);
      Serial.println(sensor_state.lamp_8);
    }
};
