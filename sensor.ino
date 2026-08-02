void read_black_line() {
  sumOnSensor = 0;
  sensorWight = 0;
  bitSensor = 0;

  for (int i = 0; i < sensorNumber; i++) {
    selectChannel(i);
    delayMicroseconds(20);
    sensorADC[i] = analogRead(SIG_PIN);

    if (sensorADC[i] > threshold[i]) {
      sensorDigital[i] = 1;
    } else {
      sensorDigital[i] = 0;
    }

    sumOnSensor += sensorDigital[i];
    sensorWight += sensorDigital[i] * WeightValue[i];
    bitSensor += sensorDigital[i] * bitWeight[(sensorNumber - 1) - i];
  }
}

void selectChannel(int channel) {
  digitalWrite(S0, bitRead(channel, 0));
  digitalWrite(S1, bitRead(channel, 1));
  digitalWrite(S2, bitRead(channel, 2));
  digitalWrite(S3, bitRead(channel, 3));
}

void analog_Serial() {
  while (!push(button1)) {
    for (int i = 0; i < sensorNumber; i++) {
      selectChannel(i);
      sensorADC[i] = analogRead(SIG_PIN);
      Serial.print(String(sensorADC[i]) + ",  ");
    }
    Serial.println();
  }
}

void analog_display() {
  oled.clear();
  while (!push(button1)) {
    oled.set1X();
    read_black_line();
    for (byte i = 0; i < 4; i++) {
      if (sensorADC[i + 4] < 10) text(String(sensorADC[i + 3]) + "   ", 10, i * 2);
      else if (sensorADC[i + 4] < 100) text(String(sensorADC[i + 3]) + "  ", 10, i * 2);
      else if (sensorADC[i + 4] < 1000) text(String(sensorADC[i + 3]) + " ", 10, i * 2);
      else text(String(sensorADC[i + 4]), 10, i * 2);

      text(String(sensorADC[3 - i]) + "   ", 70, i * 2);
    }
  }
}

void digital_display() {
  oled.clear();
  oled.set2X();
  while (!push(button1)) {
    read_black_line();
    for (byte i = 0; i < 4; i++) {
      if (sensorDigital[i + 4] < 10) text(String(sensorDigital[i + 3]) + "   ", 10, i * 2);
      else if (sensorDigital[i + 4] < 100) text(String(sensorDigital[i + 3]) + "  ", 10, i * 2);
      else if (sensorDigital[i + 4] < 1000) text(String(sensorDigital[i + 3]) + " ", 10, i * 2);
      else text(String(sensorDigital[i + 4]), 10, i * 2);

      text(String(sensorDigital[3 - i]) + "   ", 70, i * 2);
    }
  }
  oled.set1X();
}

void calibration() {
  oled.clear();
  oled.set2X();
  text("Calibrating... ", 04, 2);
  oled.set2X();
  delay(50);
  cal();
  show_calibration();
}

void cal() {
  byte i = 0;
  for (i = 0; i < sensorNumber; i++) {
    maximum[i] = 0;
    minimum[i] = 1024;
  }
  motor(-150, 150);
  for (short int c = 0; c < 500; c++)
    for (i = 0; i < sensorNumber; i++) {
      selectChannel(i);
      sensorADC[i] = analogRead(SIG_PIN);
      maximum[i] = max(maximum[i], sensorADC[i]);
      minimum[i] = min(minimum[i], sensorADC[i]);
    }
  motor(0, 0);
  delay(200);
  motor(150, -150);
  for (short int c = 0; c < 500; c++)
    for (i = 0; i < sensorNumber; i++) {
      selectChannel(i);
      sensorADC[i] = analogRead(SIG_PIN);
      maximum[i] = max(maximum[i], sensorADC[i]);
      minimum[i] = min(minimum[i], sensorADC[i]);
    }
  motor(0, 0);
  delay(200);
  motor(150, -150);
  for (short int c = 0; c < 500; c++)
    for (i = 0; i < sensorNumber; i++) {
      selectChannel(i);
      sensorADC[i] = analogRead(SIG_PIN);
      maximum[i] = max(maximum[i], sensorADC[i]);
      minimum[i] = min(minimum[i], sensorADC[i]);
    }
  motor(0, 0);
  delay(200);
  motor(-150, 150);
  for (short int c = 0; c < 500; c++)
    for (i = 0; i < sensorNumber; i++) {
      selectChannel(i);
      sensorADC[i] = analogRead(SIG_PIN);
      maximum[i] = max(maximum[i], sensorADC[i]);
      minimum[i] = min(minimum[i], sensorADC[i]);
    }
  motor(0, 0);

  for (i = 0; i < sensorNumber; i++) {
    threshold[i] = (maximum[i] - minimum[i]) * 0.5 + minimum[i];
    EEPROM.write(i, threshold[i] / 4);
    delay(10);
    EEPROM.write(i + sensorNumber, maximum[i] / 4);
    delay(10);
    EEPROM.write(i + (sensorNumber * 2), minimum[i] / 4);
    delay(10);
  }
  return;
}

void side_calibration() {
  read_black_line();

  for (int i = (sensorNumber - 1); i >= 0; i--) {
    Serial.print(String(bitRead(bitSensor, i)));
  }

  if (bitSensor == 0b10000000000000) {
    Serial.print("  Side: Left");
  } else if (bitSensor == 0b00000000000001) {
    Serial.print("  Side: Right");
  } else {
    Serial.print("  Side: NULL");
  }

  Serial.println();
}

void show_calibration() {
  for (int i = 0; i < sensorNumber; i++) {
    mid = threshold[i] = EEPROM.read(i) * 4;
    max = maximum[i] = EEPROM.read(i + sensorNumber) * 4;
    min = minimum[i] = EEPROM.read(i + (sensorNumber * 2)) * 4;
  }
  oled.clear();
  oled.set2X();
  while (!push(button1)) {
    text("Low:  " + String(min), 0, 0);
    text("Mid:  " + String(mid), 0, 3);
    text("High: " + String(max), 0, 6);
  }
  oled.set1X();
}