void eeprom_refresh() {
  for (byte i = 0; i < sensorNumber; i++) {
    threshold[i] = EEPROM.read(i) * 4;
    maximum[i] = EEPROM.read(i + sensorNumber) * 4;
    minimum[i] = EEPROM.read(i + (sensorNumber * 2)) * 4;
  }

  base_speed = EEPROM.read(43);
  turn_speed = EEPROM.read(44);
  inverse_delay = EEPROM.read(45);
  kp = EEPROM.read(46);
  kd = EEPROM.read(47);
  turn_delay = EEPROM.read(48);
  brake_delay = EEPROM.read(49);
  U_turn_delay = EEPROM.read(50);
  stop_delay = EEPROM.read(51);
  forward_delay = EEPROM.read(52);
  t_hand_rule = EEPROM.read(53);
  cross_hand_rule = EEPROM.read(54);
}

void memory_clear() {
  oled.set2X();
  oled.clear();
  text("You Sure?", 11, 3);
  oled.set1X();
  text("Long Press to Confirm", 1, 7);
  while (1) {
    byte r = push(button1);
    if (r == 1) break;
    else if (r == 2) {
      oled.set2X();
      oled.clear();
      text("  DONE!  ", 11, 3);
      for (byte i = 0; i < 100; i++) {
        EEPROM.write(i, 0);
        delay(10);
      }
      eeprom_refresh();
      break;
    }
  }
}

void display_threshold() {
  Serial.print("Threshold :");
  Serial.print("  ");

  for (byte i = 0; i < sensorNumber; i++) {
    threshold[i] = EEPROM.read(i) * 4;
    Serial.print(String(threshold[i]) + ", ");
  }
  Serial.println();
  Serial.println();

  Serial.println("Speed: " + String(EEPROM.read(43)));
  Serial.println("Turn Speed : " + String(EEPROM.read(44)));
  Serial.println("Inverse Delay : " + String(EEPROM.read(45)));
  Serial.println("KP : " + String(EEPROM.read(46)));
  Serial.println("KD : " + String(EEPROM.read(47)));
  Serial.println("T-Junc Rule : " + String(EEPROM.read(53)) + (EEPROM.read(53) == 0 ? " (Left)" : " (Right)"));
  Serial.println("Cross Rule : " + String(EEPROM.read(54)) + (EEPROM.read(54) == 0 ? " (Left)" : " (Right)"));
  Serial.println("Turn Delay : " + String(EEPROM.read(48)));
  Serial.println("Brake Delay : " + String(EEPROM.read(49)));
  Serial.println("U Turn Delay : " + String(EEPROM.read(50)));
  Serial.println("Stop Delay : " + String(EEPROM.read(51)));
  Serial.println("Forward Delay : " + String(EEPROM.read(52)));
}