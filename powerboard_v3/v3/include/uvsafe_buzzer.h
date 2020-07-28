
/*
 *  by: MRO for UVSA GROUP
 *  Todos los derechos reservados
 *  Winnipeg, Manitoba. Canada. Julio 2020
 */

#define BEEP_ON 1
#define BEEP_OFF 0
#define ONE_BEEP 2
#define TWO_BEEP 3
#define CONTINOUS_BEEP 4
#define POST_PIR 5
#define FIN_DE_CICLO 6
#define ERROR 7

bool BUZZER_ENABLED = true;

class UVBuzzer
{
        // Class Member Variables
        // These are initialized at startup
        unsigned long OnTime; // milliseconds of on-time
        unsigned long OffTime; // milliseconds of off-time
        int counter = 0;
        int beeps = 0;
        int mode = 0;

        // These maintain the current state
        int outputState;           // outputState used to set the out
        unsigned long previousMillis; // will store last time output was updated

        // Constructor - creates a Flasher
        // and initializes the member variables and state
public:
        UVBuzzer()
        {
                outputState = LOW;
                previousMillis = 0;
        }

        void Trigger(int modes){
                mode = modes;
                switch (modes) {
                  case 2:
                    beeps = 1;
                    break;
                  case 3:
                    beeps = 2;
                    break;
                  case 4:
                    beeps = 3;
                    break;
                    case 5:
                      beeps = 3;
                      break;
                    case 6:
                      beeps = 3;
                      break;
                    case 7:
                      beeps = 1;
                      break;
                  default:
                    beeps = 0;
                    break;
                }

        }

        void CheckMode(){
                switch (mode) {
                case 0: //Beep off
                        OnTime = 0;
                        break;

                case 1: //beep constatemente on
                        OffTime = 0;
                        break;

                case 2: //caso para un solo beep
                        OnTime = 35;
                        OffTime = 35;
                        break;

                case 3: //bip bip
                        OnTime = 35;
                        OffTime = 35;
                        break;

                case 4: //beeeep shhhhhhhhh beeeep
                        OnTime = 800;
                        OffTime = 3000;
                        break;
                case 5: //beep shhhhh beep
                        OnTime = 100;
                        OffTime = 900;
                        break;
                case 6: //despues de acabar un ciclo bip y shhhhhhhhhh
                        OnTime = 500;
                        OffTime = 7000;
                        break;
                case 7: //Error
                        OnTime = 1000;
                        OffTime = 1;
                        break;

                }
        }

        int CurrentMode(void){
                return mode;
        }

        int Update(void)
        {       CheckMode();
                if (BUZZER_ENABLED == false) return 0;
                if (OnTime == 0) return 0;
                if (OffTime == 0) return 1;

                // check to see if it's time to change the state of the LED
                unsigned long currentMillis = millis();
                if(beeps > 0){
                if((outputState == HIGH) && (currentMillis - previousMillis >= OnTime))
                {
                        outputState = LOW; // Turn it off
                        previousMillis = currentMillis; // Remember the time
                        if(beeps != 3) beeps--;
                }
                else if ((outputState == LOW) && (currentMillis - previousMillis >= OffTime))
                {
                        outputState = HIGH; // turn it on
                        previousMillis = currentMillis; // Remember the time
                }
              }
              else {
                outputState = 0;
              }
                return outputState;
        }
};
