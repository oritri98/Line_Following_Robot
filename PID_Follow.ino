bool detect_stop_line() {
  // Check if all or most sensors see black
  int black_count = 0;
  
  for (int i = 0; i < sensorNumber; i++) {
    if (sensorDigital[i] == 1) {
      black_count++;
    }
  }
  
  // If 10 or more sensors see black, it's probably a stop line
  if (black_count >= 10) {
    return true;
  }
  
  return false;
}

void line_follow() {
  int stop_counter = 0;
  bool stopped = false;
  
  while (1) {
    PID_Controller(base_speed, kp, kd);
    
    // Check for stop line
    if (detect_stop_line()) {
      stop_counter++;
      
      // Require consecutive detections to avoid false positives
      if (stop_counter > 3 && !stopped) {
        // Stop the robot
        motor(0, 0);
        digitalWrite(led, HIGH);
        
        // Display stop message on OLED
        oled.clear();
        oled.set2X();
        text("STOP LINE!", 4, 2);
        text("Detected", 6, 4);
        
        // Blink LED while stopped
        for (int i = 0; i < 5; i++) {
          digitalWrite(led, !digitalRead(led));
          delay(200);
        }
        
        // Wait for stop delay
        delay(stop_delay);
        
        // Move forward a little to get off the stop line
        motor(base_speed / 2, base_speed / 2);
        delay(200);
        motor(0, 0);
        
        stopped = true;
        stop_counter = 0;
        digitalWrite(led, LOW);
        
        // Clear OLED and continue
        oled.clear();
        continue;
      }
    } else {
      stop_counter = 0;
      stopped = false;
    }

    switch (bitSensor) {
      case 0b11110011000000: direction = "left"; break;
      case 0b11100011000000: direction = "left"; break;
      case 0b01110011000000: direction = "left"; break;
      //
      case 0b00000011001111: direction = "right"; break;
      case 0b00000011001110: direction = "right"; break;
      case 0b00000011000111: direction = "right"; break;
      //T
      case 0b11000000000011: direction = "T"; break;
      case 0b10000000000001: direction = "T"; break;
      //
      case 0b11000011000011: direction = "cross"; break;
      case 0b10000011000001: direction = "cross"; break;
    }

    if (sumOnSensor == 0) {
      if (direction != "straight") {
        delay(turn_delay);
        if (direction == "left") {
          turnLeft(turn_speed, turn_speed);
        } else if (direction == "right") {
          turnRight(turn_speed, turn_speed);
        } else if (direction == "T") {
          if (t_scan == "left") {
            turnLeft(turn_speed, turn_speed);
          } else {
            turnRight(turn_speed, turn_speed);
          }
        } else if (direction == "cross") {
          if (cross_scan == "left") {
            turnLeft(turn_speed, turn_speed);
          } else {
            turnRight(turn_speed, turn_speed);
          }
        }
      }
    }
  }
}