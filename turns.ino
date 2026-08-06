void turnLeft(int L, int R) {
  while (sensorDigital[4] != 1) {
    motor(-L, R);
    read_black_line();
  }
  direction = "straight";
}

