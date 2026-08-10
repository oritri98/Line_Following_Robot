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
