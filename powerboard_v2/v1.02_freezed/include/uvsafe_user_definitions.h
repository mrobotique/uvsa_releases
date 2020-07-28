/*** Definiciones de Usuario
by: MRO for IWI
Todos los derechos reservados
Aguascalientes, Mexico. Mayo 2020

En este archivo se definen todos los valores usados en todos los modulos
relacionados con este programa. Si el usuario necesitara cambiar/modificar/actualizar
algun valor de tiempo debe referirse a este archivo
***/

#define EXPOSITION_TIME 300 //[s]  Tiempo de exposicion de la lampara. Usado en uvsafe_auto
#define TIEMPO_INICIO 60000 //[ms] 1minuto. Este tiempo es el que transcurre antes de encender las lamparas despues
                            //de lanzar la rutina con el boton auto

/***********************************************
************   LEDS  ***************************
*************************************************/
//Definincion de los segmentos de leds
//usados en uvsafe_rgb_leds
//el segmento 0 abarca del led i=0 al led i=2. El segmento 1 abarca del
//led i=3 al led i=5... etc
#define seg0 0
#define seg1 3
#define seg2 6
#define seg3 9
#define seg4 12
#define seg5 15
#define seg6 18

//Intensidades de los leds
#define max_intensity 164
#define min_intensity 10

/****************  DEBOUNCING TIMES *******************/
//Estas definiciones son usadas en uvsafe_read
#define MAGNETIC_DEBOUNCE 800 //ms
#define PIR_DEBOUNCE 100 //ms
#define DEADMAN_DEBOUNCE 1000 //ms
#define AFTER_PIR 30000 //[ms] Tiempo para encender las lamparas despues que ningun sensor PIR detecta presencia.
