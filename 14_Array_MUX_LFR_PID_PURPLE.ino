#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <EEPROM.h>
SSD1306AsciiWire oled;

// Define control pins for the multiplexer
#define S0 11
#define S1 10
#define S2 8
#define S3 7
#define SIG_PIN A7

//motorpins
#define right_motor_forward 2
#define right_motor_backward 4
#define left_motor_forward 5
#define left_motor_backward 6
//speed control pins (PWM)
#define right_motor_speed 3
#define left_motor_speed 9

//Sensor Variables
#define sensorNumber 14
int sensorADC[sensorNumber];
int sensorDigital[sensorNumber];
int bitWeight[sensorNumber] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
int WeightValue[sensorNumber] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140 };
int maximum[sensorNumber], minimum[sensorNumber], threshold[sensorNumber];
int sumOnSensor;
int sensorWight;
int bitSensor;
int min, max, mid;

//PID Variables
float line_position;
float error;
float center_position = 75;
float derivative, previous_error;
int base_speed;
int kp;
int kd = 0;
int t_hand_rule;      // 0=left, 1=right for T-junctions
int cross_hand_rule;  // 0=left, 1=right for Cross junctions
//delay parameters
int inverse_delay;
int turn_speed;
int turn_delay;
int stop_delay;
int forward_delay;
int U_turn_delay;
int brake_delay;

//turns parameter
String direction = "straight";
String t_scan;       // for T-junctions
String cross_scan;   // for Cross junctions

//button
#define button1 A0
#define button2 12
#define button3 A1
//Led
#define led 13

// ============================================
// FUNCTION PROTOTYPES - ADD THESE
// ============================================
byte push(byte button_number);
byte menu();
void eeprom_refresh();
void team_name();
void text(String t, byte x, byte y);
void line_follow();
void calibration();
void edit_parameter();
void analog_display();
void digital_display();
void motor_test();
void memory_clear();
void analog_Serial();
void Load_Parameter();
void display_current_settings();
void display_threshold();
// ============================================

//........................................................................................
void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);

  pinMode(left_motor_speed, OUTPUT);
  pinMode(right_motor_speed, OUTPUT);

  pinMode(led, OUTPUT);

  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(System5x7);
  eeprom_refresh();
  
  // Set scan directions based on EEPROM values
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
  
  team_name();
  display_threshold();
}

//...............................................................................................................................................
void loop() {
  byte r = push(button1);
  if (r) {
    if (r == 1) {
      r = menu();
      if (r) {
        if (r == 1) line_follow();
        else if (r == 2) calibration();
        else if (r == 3) edit_parameter();
        else if (r == 4) analog_display();
        else if (r == 5) digital_display();
        else if (r == 6) motor_test();
        else if (r == 7) memory_clear();
        else if (r == 8) {
          while (!push(button1)) {
            analog_Serial();
          }
        } else if (r == 9) Load_Parameter();
        else if (r == 10) display_current_settings();
      }
    }
    team_name();
  }
}

void display_current_settings() {
  oled.clear();
  oled.set1X();
  
  text("Current Settings:", 2, 0);
  text("----------------", 2, 1);
  
  // Show T-junction hand rule
  String tRuleText = (t_hand_rule == 0) ? "Left" : "Right";
  text("T-Junc: " + tRuleText, 2, 3);
  
  // Show Cross-junction hand rule
  String crossRuleText = (cross_hand_rule == 0) ? "Left" : "Right";
  text("Cross: " + crossRuleText, 2, 4);
  
  // Show PID settings
  text("KP: " + String(kp) + "  KD: " + String(kd), 2, 5);
  text("Speed: " + String(base_speed), 2, 6);
  
  text("Press Button to exit", 2, 7);
  
  while (!push(button1)) {
    delay(50);
  }
}