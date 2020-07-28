void safety_functions(){
//Si algun sensor manual cambia de estado debemos apagar todo y pasar a manual_pattern
if (digitalRead(DEADMAN_Pin) == 0) operation_mode = mode_manual;
if ((operation_mode != mode_manual) && (auto_button.clicks != 0)) operation_mode = mode_manual;
if ((operation_mode != mode_manual) && (auto_button.clicks >= 1)) operation_mode = mode_manual;
}
