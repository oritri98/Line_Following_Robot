String parameter_list(byte a) {
  if (a == 1) return "Speed           ";
  else if (a == 2) return "Rotate Speed    ";
  else if (a == 3) return "Inverse Delay   ";
  else if (a == 4) return "KP              ";
  else if (a == 5) return "KD              ";
  else if (a == 6) return "T-Junc Rule     ";
  else if (a == 7) return "Cross Rule      ";
  else if (a == 8) return "Turn Delay      ";
  else if (a == 9) return "Brake Delay     ";
  else if (a == 10) return "U Turn Delay    ";
  else if (a == 11) return "Stop Delay      ";
  else if (a == 12) return "Forward Delay   ";
}

int get_eeprom_address(int param_index) {
  switch(param_index) {
    case 1: return 43;
    case 2: return 44;
    case 3: return 45;
    case 4: return 46;
    case 5: return 47;
    case 6: return 53;
    case 7: return 54;
    case 8: return 48;
    case 9: return 49;
    case 10: return 50;
    case 11: return 51;
    case 12: return 52;
    default: return 43;
  }
}

void edit_parameter() {
  oled.set1X();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 12;
  int value;

  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8; i++) {
        if (i + base > limit) break;
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        
        if (i + base == 6 || i + base == 7) {
          int eeprom_addr = get_eeprom_address(i + base);
          value = EEPROM.read(eeprom_addr);
          String ruleText = (value == 0) ? "Left" : "Right";
          String paramName = parameter_list(i + base);
          text(String() + "->" + paramName + ruleText + "    ", 2, i);
        } else {
          int eeprom_addr = get_eeprom_address(i + base);
          value = EEPROM.read(eeprom_addr);
          text(String() + "->" + parameter_list(i + base) + String(value) + "   ", 2, i);
        }
      }
    }

    byte x = push(button3);
    byte r = push(button1);

    if (x) {
      if (x == 1) {
        ind++;
        if (ind > limit) {
          ind = 1;
          base = ind;
          peak = base + 7;
        } else if (ind > peak) {
          peak = ind;
          base = peak - 7;
        }
      }
    }

    if (r) {
      if (r == 1) {
        ind--;
        if (ind < 1) {
          ind = limit;
          peak = ind;
          base = peak - 7;
        } else if (ind < base) {
          base = ind;
          peak = base + 7;
        }
      }
    }

    if (r == 2) {
      set_adjust(ind);
      temp = 0;
    }

    if (x == 2) {
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
      return;
    }
  }
}

void set_adjust(int indicator) {
  oled.set2X();
  oled.clear();

  int value;
  int eeprom_addr = get_eeprom_address(indicator);
  
  if (indicator == 6 || indicator == 7) {
    value = EEPROM.read(eeprom_addr);
    String ruleText = (value == 0) ? "Left" : "Right";
    String paramName = (indicator == 6) ? "T-Junc Rule" : "Cross Rule";
    
    int temp = -1;
    
    while (1) {
      if (temp != value) {
        temp = value;
        ruleText = (value == 0) ? "Left" : "Right";
        text(paramName + ": " + ruleText, 2, 3);
      }

      byte x = push(button3);
      byte r = push(button1);

      if (r && r == 1) {
        value = (value == 0) ? 1 : 0;
      }
      
      if (x && x == 1) {
        value = (value == 0) ? 1 : 0;
      }

      if (x == 2 || r == 2) {
        EEPROM.write(eeprom_addr, value);
        delay(10);
        oled.set1X();
        return;
      }
    }
  } else {
    value = EEPROM.read(eeprom_addr);
    int temp = -1;
    int limit = 255;
    int step_size = (indicator == 4 || indicator == 5) ? 1 : 5;

    while (1) {
      if (temp != value) {
        temp = value;
        text("SET: " + String(value) + "    ", 5, 3);
      }

      byte x = push(button3);
      byte r = push(button1);

      if (r && r == 1) {
        value += step_size;
        if (value > limit) value = 0;
      }

      if (x && x == 1) {
        value -= step_size;
        if (value < 0) value = limit;
      }

      if (x == 2 || r == 2) {
        EEPROM.write(eeprom_addr, value);
        delay(10);
        oled.set1X();
        return;
      }
    }
  }
}