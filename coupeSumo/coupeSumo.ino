#include <vl53l0x_tuning.h>
#include <vl53l0x_interrupt_threshold_settings.h>
#include <vl53l0x_i2c_platform.h>
#include <Adafruit_VL53L0X.h>
#include <vl53l0x_api_core.h>
#include <vl53l0x_types.h>
#include <vl53l0x_device.h>
#include <vl53l0x_api_ranging.h>
#include <vl53l0x_platform.h>
#include <vl53l0x_def.h>
#include <vl53l0x_api_calibration.h>
#include <vl53l0x_platform_log.h>
#include <vl53l0x_api.h>
#include <vl53l0x_api_strings.h>

//Pins for motor
#define ENABLE_A 0
#define MOTOR_A1 1
#define MOTOR_A2 2
#define ENABLE_B 3
#define MOTOR_B1 4
#define MOTOR_B2 5

//definition of Infrared sensor

Adafruit_VL53L0X infraredSensor = Adafruit_VL53L0X();
#define LEFT 0
#define RIGHT 1
//pins for color sensor
#define colorSensorS0 6
#define colorSensorS1 7
#define colorSensorS2 8
#define colorSensorS3 9
#define colorSensorOutput 10

// Limit value for detecting where we're going out of the table
#define OFF_TABLE 100

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup() {
  //Initialising Motors
  pinMode(ENABLE_A, OUTPUT);
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(ENABLE_B, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  //Initialising color sensor
  pinMode(colorSensorS0, OUTPUT);
  pinMode(colorSensorS1, OUTPUT);
  pinMode(colorSensorS2, OUTPUT);
  pinMode(colorSensorS2, OUTPUT);
  pinMode(colorSensorOutput, INPUT);
  //Setting frequency scaling
  digitalWrite(colorSensorS0, HIGH);
  digitalWrite(colorSensorS1, LOW);

}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  infraredSensor.rangingTest(&measure, false);
  //setting red filtered
  digitalWrite(colorSensorS2, LOW);
  digitalWrite(colorSensorS3, LOW);
  redFrequency = pulseIn(colorSensorOutput, LOW);
  if (redFrequency < OFF_TABLE) { // black detected
    if (measure.RangeStatus != 4 && measure.RangeMillimeter < 10) { //we're close to the target
      moveForward();
    } else {
      int nextDirection = random(0, 2);//0 = LEFT, 1 = RIGHT
      if (nextDirection == LEFT) {
        moveLeft();
        delay(250);
      } else {
        moveRight();
        delay(250);
      }
      moveForward();
    }

  } else { // white detected
    moveBackward();
  }
  delay(250);
}

void moveBackward() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

void moveForward() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}
