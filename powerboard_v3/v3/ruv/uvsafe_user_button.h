void user_button_update(RGBLeds LedsIndicadores) {
  auto_button.Update();
  safety_functions();

  if (operation_mode == mode_manual) {
    // Si el usuario ejecuta un push largo
    if (auto_button.clicks == -1 && auto_button.depressed == true)
    {
      LedsIndicadores.confirm_push(true);

      while (auto_button.depressed) { //No hacer nada mientras el usuario no suelta el boton
        auto_button.Update();
      }
      LedsIndicadores.confirm_push(false);
      operation_mode = mode_auto_init;
    }
  }
  else{

    safety_functions();

  }
}
