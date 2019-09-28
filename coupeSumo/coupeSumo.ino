
#include <Adafruit_VL53L0X.h>

//Pins for motor
#define ENABLE_A 5
#define MOTOR_A1 6
#define MOTOR_A2 7
#define ENABLE_B 8
#define MOTOR_B1 9
#define MOTOR_B2 10

//definition of Infrared sensor

Adafruit_VL53L0X infraredSensor = Adafruit_VL53L0X();
#define LEFT 0
#define RIGHT 1
#define TIMER 250
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
  //Enabling motor
  digitalWrite(ENABLE_A, HIGH);
  digitalWrite(ENABLE_B, HIGH);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  infraredSensor.rangingTest(&measure, false);
  //setting red filtered
  digitalWrite(colorSensorS2, LOW);
  digitalWrite(colorSensorS3, LOW);
  redFrequency = pulseIn(colorSensorOutput, LOW);
  if (redFrequency < OFF_TABLE) { // black detected
    if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 10) { //we're close to the target
      moveForward();
    } else {
      changeDirection();
      moveForward();
    }

  } else { // white detected
    moveBackward();
    changeDirection();
  }
  delay(TIMER);
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
void changeDirection(){
  int nextDirection = random(0, 2);//0 = LEFT, 1 = RIGHT
  if (nextDirection == LEFT) {
    turnLeft();
    delay(250);
  } else {
    turnRight();
    delay(250);
  }
}
