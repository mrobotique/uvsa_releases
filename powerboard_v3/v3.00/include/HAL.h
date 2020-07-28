/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */

/*** HAL file
HAL: Hardware Abstraction Layer
requiere
https://github.com/cosmikwolf/Bounce2mcp.git
***/

#include "Adafruit_MCP23017.h"
#include <Arduino.h>
#include "FastLED.h"
#include "ClickButton.h"
#include "uvsafe_blinker.h"
#include "uvsafe_buzzer.h"


Flasher activity_led(5);
UVBuzzer beeper;

//operation modes
enum uvs_mode{
  mode_boot,
  mode_auto_on,
  mode_auto_init,
  mode_manual,
  mode_manual_push,
  mode_test,
} operation_mode;

//Timer modes
enum timer_mode{
  init_sequence,
  running
};


/** Relacionado con el boton y el timer ***/
unsigned long timer_count = 0; //Cuenta cuantas veces se ha clickeado el boton de usuario
unsigned long nuevo_tiempo_exposicion = 0;
unsigned long last_click_update; //timer para decidir cuando pasar a modo basal
unsigned long tiempo_restante; //Global para publicar el timer en el puerto serie

/*** PIR ***/
unsigned long last_pir_time = millis();
//Pir boolean para saber si la lampara se apago por que el pir se activo mucho Tiempo
bool pir_timeout = false; //el valor se actualiza -> true en uvsafe_safety.h

//On-board LED
#define BUILTIN_LED 5

//LED TypicalLEDStrip
#define LED_PIN     7
#define NUM_LEDS    18
#define BRIGHTNESS  32
CRGB leds[NUM_LEDS]; //Instance

//PIR sensors. Connected directly to the Arduino board
//D10 to D13
#define PIR1_Pin 10
#define PIR2_Pin 11
#define PIR3_Pin 12
#define PIR4_Pin 14
//Deadman switch
#define DEADMAN1_Pin 8
#define DEADMAN2_Pin 6
//Auto switch
#define AUTO_Pin 9

//Relay outputs
//Connected to the MCP23017
//GPA0 to GPA7
#define LAMP1 5
#define LAMP2 4
#define LAMP3 3
#define LAMP4 2
#define LAMP5 1
#define LAMP6 0
#define LAMP_DEADMAN 6
#define LAMP_AUTO 7

//GPB
//Magnetic sensors inputs
#define MAGNETIC1 8
#define MAGNETIC2 9
//buzzer
#define BUZZER 10

//timer variables
//unsigned long left_time = 0;

//Struct Sensores
struct SENSOR_STRUCT{
  int deadman1_sw;
  int deadman2_sw;
  int auto_button;
  int pir_1;
  int pir_2;
  int pir_3;
  int pir_4;
  int pir_status;
  int pir_transition;
  int magnetic_1;
  int magnetic_2;
  int magnetic_status;
  int lamp_1; //It is possible to read the output status.
  int lamp_2; //So, lets read the lamps staus
  int lamp_3;
  int lamp_4;
  int lamp_5;
  int lamp_6;
  int lamp_deadman;
  int lamp_auto;
} sensor_state;

//Struct outputs
struct LAMP_STRUCT{
  int lamp_1;
  int lamp_2;
  int lamp_3;
  int lamp_4;
  int lamp_5;
  int lamp_6;
  int lamp_deadman;
  int lamp_auto;
}lamp_state;

//The start button is kind of special so,
//It is intialized in a sightly different way
ClickButton auto_button(AUTO_Pin, LOW, CLICKBTN_PULLUP);


//Prototypes
void init_gpio(Adafruit_MCP23017 gpio);

/**** Operational Functions ****/
void init_gpio(Adafruit_MCP23017 gpio){
//Init gpio
//Outputs
gpio.begin();      // use default address 0
gpio.pinMode(LAMP1, OUTPUT);
gpio.pinMode(LAMP2, OUTPUT);
gpio.pinMode(LAMP3, OUTPUT);
gpio.pinMode(LAMP4, OUTPUT);
gpio.pinMode(LAMP5, OUTPUT);
gpio.pinMode(LAMP6, OUTPUT);
gpio.pinMode(LAMP_DEADMAN, OUTPUT);
gpio.pinMode(LAMP_AUTO, OUTPUT);
gpio.pinMode(BUZZER, OUTPUT);
//turn off all the gpios*/
for (int i=0; i<=8;i++){
  gpio.digitalWrite(i,0);
}
// turn off the buzzer
gpio.digitalWrite(BUZZER,0);

//Inputs
gpio.pinMode(MAGNETIC1, INPUT);
gpio.pullUp(MAGNETIC1, HIGH);  // turn on a 100K pullup internally
gpio.pinMode(MAGNETIC2, INPUT);
gpio.pullUp(MAGNETIC2, HIGH);  // turn on a 100K pullup internally

//Now the Microcontroller Inputs
pinMode(PIR1_Pin, INPUT_PULLUP);
pinMode(PIR2_Pin, INPUT_PULLUP);
pinMode(PIR3_Pin, INPUT_PULLUP);
pinMode(PIR4_Pin, INPUT_PULLUP);
pinMode(DEADMAN1_Pin, INPUT_PULLUP);
pinMode(DEADMAN2_Pin, INPUT_PULLUP);
//Microcontroller outputs
//Outputs for future applications

//LED Strip  WS2812B
FastLED.addLeds<WS2812B,LED_PIN,EOrder::GRB>(leds, NUM_LEDS);
}

//Serial port Recepcion
#define JSON_SERIAL1_BUFFER_SIZE 20
String inputString = "";         // a String para guardar el JSON de entrada
