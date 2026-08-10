void eeprom_refresh() {
  for (byte i = 0; i < sensorNumber; i++) {
    threshold[i] = EEPROM.read(i) * 4;
    maximum[i] = EEPROM.read(i + sensorNumber) * 4;
    minimum[i] = EEPROM.read(i + (sensorNumber * 2)) * 4;
  }
