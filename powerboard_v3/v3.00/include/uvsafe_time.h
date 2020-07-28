
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */


class Countdown {
//modes
int STOP = 0;
int START = 1;
bool isRunning = false;

typedef void (*timer_callback)(void);

int mode = STOP;   //timer mode
unsigned long time_period = 0;   //Tiempo que el usuario desea contar
unsigned long left_time = 0;   //Tiempo restante
unsigned long delta_time = 1000;   //unidad minima que se descuenta al timer en mili segundos
unsigned long last_update = 0;
timer_callback onComplete;   //esta es la funcion que se llama cuando el tiempo llega a cero

public:
Countdown(unsigned long _time_period, timer_callback _onComplete ){
        time_period = _time_period + 1; //Agrego un segundo por que unsigned no permite comparar <0
        left_time = time_period;
        last_update = millis();
        onComplete = _onComplete;
}


void run(){
        if ((millis() - last_update) > delta_time) {
                last_update = millis();
                if (mode == START) {
                        left_time -= 1; //restar un segundo al tiempo restante
                } //MODE==start
        } //delta_time
        if (left_time == 0) { //la cuenta ya llego a cero
                stop();
                left_time = time_period;
                onComplete(); //call the callback
        }
}


void new_time(unsigned long _new_time){
  stop();
  time_period = _new_time + 1; //Agrego un segundo por que unsigned no permite comparar <0
  left_time = time_period;
}


void start(){
        mode = START;
        isRunning = true;
}


void stop(){
        mode = STOP;
        isRunning = false;
}


void pause(){
        mode = STOP; //but it is still running
}


void reset(){
        mode = STOP;
        left_time = time_period;
        isRunning = false;
}

unsigned long get_time(){
        return left_time;
}


bool is_running(){
        return isRunning;
}
};
