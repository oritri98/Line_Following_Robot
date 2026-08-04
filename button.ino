byte push(byte button_number) {
  uint32_t b_timer = 0;
  if (!digitalRead(button_number)) {
    delay(50);
    while (!digitalRead(button_number)) {
      delay(50);
      b_timer += 50;
      if (b_timer > 500) digitalWrite(led, 1);
    }
    digitalWrite(led, 0);
    if (b_timer > 500) return 2;
    else if (b_timer > 0) return 1;
  }
  return 0;
}