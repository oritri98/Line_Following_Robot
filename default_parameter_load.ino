void Load_Parameter() {
  oled.set2X();
  oled.clear();
  text("Confirm?", 11, 3);
  oled.set1X();
  text("Long Press to Confirm", 1, 7);
  while (1) {
    byte r = push(button1);
    if (r == 1) break;
    else if (r == 2) {
      oled.set2X();
      oled.clear();
      text("  DONE!  ", 11, 3);
      
      EEPROM.write(43, 200);
      EEPROM.write(44, 120);
      EEPROM.write(45, 30);
      EEPROM.write(46, 10);
      EEPROM.write(47, 5);
      EEPROM.write(48, 40);
      EEPROM.write(49, 30);
      EEPROM.write(50, 80);
      EEPROM.write(51, 2000);  // Stop delay increased for stop line
      EEPROM.write(52, 50);
      EEPROM.write(53, 1);
      EEPROM.write(54, 1);

      eeprom_refresh();
      
      if (t_hand_rule == 0) {
        t_scan = "left";
      } else {
        t_scan = "right";
      }
      
      if (cross_hand_rule == 0) {
        cross_scan = "left";
      } else {
        cross_scan = "right";
      }
      
      delay(200);
      break;
    }
  }
}