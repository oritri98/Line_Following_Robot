void motor(int LPWM, int RPWM) {
  if (LPWM > 0) {
    digitalWrite(left_motor_forward, HIGH);
    digitalWrite(left_motor_backward, LOW);
  } else {
    digitalWrite(left_motor_forward, LOW);
    digitalWrite(left_motor_backward, HIGH);
  }

  if (RPWM > 0) {
    digitalWrite(right_motor_forward, HIGH);
    digitalWrite(right_motor_backward, LOW);
  } else {
    digitalWrite(right_motor_forward, LOW);
    digitalWrite(right_motor_backward, HIGH);
  }

  LPWM = constrain(LPWM, -255, 255);
  RPWM = constrain(RPWM, -255, 255);

  analogWrite(left_motor_speed, abs(LPWM));
  analogWrite(right_motor_speed, abs(RPWM));
}


void motor_test() {
  while (1) {
    motor(250, 250);
    digitalWrite(led, HIGH);
    delay(3000);
    digitalWrite(led, LOW);
    motor(0, 0);
    return;
  }
}