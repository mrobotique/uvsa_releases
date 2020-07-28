*Adafruit_MCP23017
https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
*** Pin Addressing

When using single pin operations such as pinMode(pinId, dir) or digitalRead(pinId) or digitalWrite(pinId, val) then the pins are addressed using the ID's below. For example, for set the mode of GPB0 then use pinMode(8, ...).
Physial Pin # 	Pin Name 	Pin ID
21 	GPA0 	0
22 	GPA1 	1
23 	GPA2 	2
24 	GPA3 	3
25 	GPA4 	4
26 	GPA5 	5
27 	GPA6 	6
28 	GPA7 	7
1 	GPB0 	8
2 	GPB1 	9
3 	GPB2 	10
4 	GPB3 	11
5 	GPB4 	12
6 	GPB5 	13
7 	GPB6 	14
8 	GPB7 	15

*FastLED Library
https://github.com/FastLED/FastLED
**FastLED color reference: http://fastled.io/docs/3.1/struct_c_r_g_b.html

CAMBIOS VERSION 3
* RELEVADORES: Se redujo el numero de relevadores de 8 a 6. Los relevadores estan localizados en la GPIO de la siguiente manera en el MCP23017 (address 000)
  + Lampara1 : GPA5
  + Lampara2 : GPA4
  + Lampara3 : GPA3
  + Lampara4 : GPA2
  + Lampara5 : GPA1
  + Lampara6 : GPA0

* Los botones DEADMAN y BOTON PRINCIPAL ahora son 2. Los LED de los botones estan conectados en el MCP23017 de la siguiente manera:
  + LED deadman 1 y 2 (en paralelo): GPA6
  + LED boton principal: GPA7

* Los botones estan localizados (Entradas) en:
  + Deadman1: digital input 8
  + Deadman2: digital input 6

* Se redujo el numero de sensores magneticos de 6 a 2. Ahora estan localizados e el MCP23017 de la siguiente manera:
  + Mag1: GPB0
  + Mag2: GPB1  

* Se agrego un buzzer. Esta en el MCP23017 en GPB2
* Se agrego un LED de status en digital output 5



CAMBIOS VERSION 2
* Se agrego secuencia de leds de boot (inicio)
* Los sensores magneticos 2 al 6 ya no son tomados en cuenta
* El sensor magnetico 1 es tomado en cuenta en el modo manual para detectar el escudo y habilitar el deadman switch
* El sensor magnetico 1 es tomado en cuenta en el modo automatico para evitar prender la lampara 1 cuando esta colocado el escudo
* El boton principal tiene dos funciones: click cortos--> aumenta el tiempo de exposicion de la lampara (5minutos por click). El click largo lanza la el proceso de exposicion
* Se agrego timeout al timer setup : si el usuario programa un tiempo pero no activa la lampara, luego de 25 segundos el sistema regresara a su estado basal (manual)
* Se agrego timeout al pir. Si los pir estan activos por mas de 3 mintuos el sistema vuelve a un estado quasi-manual en el que los leds estan en naranja solido (para indicar al usuario que la maquina no esta en modo automatico y que el ciclo de exposicion no se efectuo
  por que la deteccion PIR estuvo activa por mas de 3 minutos). Para salir de este modo hay que usar un boton (principal o deadman)
* Se modifico el codigo de color
    - rojo: modo manual sin escudo
    - verde: modo manual listo para ser operado
    - azul in-out: iniciando cuenta regresiva para comenzar el tiempo de desinfeccion
    - violeta solido: En modo timer setup cada led representa 5 min
    - violeta in-out: proceso en curso (manual o automatico)
    - naranja in-out: proceso en curso pero sistema en pausa (PIR activos)
