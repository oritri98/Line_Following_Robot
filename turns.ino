void turnLeft(int L, int R) {
  while (sensorDigital[4] != 1) {
    motor(-L, R);
    read_black_line();
  }
  direction = "straight";
}

void turnRight(int L, int R) {
  while (sensorDigital[3] != 1) {
    motor(R, -L);
    read_black_line();
  }
  direction = "straight";
}