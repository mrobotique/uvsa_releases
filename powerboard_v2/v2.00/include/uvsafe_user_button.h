/*
 *  by: MRO for IWI
 *  Todos los derechos reservados
 *  Aguascalientes, Mexico. Mayo 2020
 */

#define CINCO_MINUTOS 300 //en segundos

void user_button_update(RGBLeds LedsIndicadores) {
        auto_button.Update();
        safety_functions();

        if (timer_count == 0) last_click_update = millis();
        // Si el switch no es accionado en TIMEOUT_CLICK tiempo el contador se restablece
        else if ((millis() - last_click_update) > TIMEOUT_CLICK) timer_count = 0;

        if (operation_mode == mode_manual) {
                // Si el usuario ejecuta un push largo
                if (auto_button.clicks == -1 && auto_button.depressed == true)
                {
                        LedsIndicadores.confirm_push(true);
                        if (timer_count == 0) timer_count = 1; //El tiempo minimo a enviar son cinco minutos
                        nuevo_tiempo_exposicion = timer_count * CINCO_MINUTOS;
                        timer_count = 0;
                        while (auto_button.depressed) { //No hacer nada mientras el usuario no suelta el boton
                                auto_button.Update();
                        }

                        LedsIndicadores.confirm_push(false);
                        operation_mode = mode_auto_init;
                }
                if (digitalRead(DEADMAN_Pin) == 0) timer_count = 0; //si el usuario se arrepiente... le pica al deadman y regresamos a manual normal
                if ((auto_button.clicks > 0) && (auto_button.changed) && !pir_timeout && (previous_mode != mode_auto_on) && (previous_mode != mode_auto_init)){
                        last_click_update = millis();
                        timer_count += 1;
                        if (timer_count > NUM_LEDS) timer_count = 1;
                }
        }
        else{
                timer_count = 0;
                safety_functions();
        }
}
