#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

double TICKS_TO_DIST = 0.013614; 
double RPM_TO_CMS = 0.136;
double target = 60; //60 cm
double kP = 30; //60 5.32 s
double kI = 1.7;
double kD = 0;
double pastTime = 0;
double lastError;
double error;
double errorSum;
double errorRate;
double motorSpeed;

void setup() {
  // put your setup code here, to run once:
  lcd.clear();
  encoders.init();
  delay(1000);
}

double getDistance(){
  double ticks = (encoders.getCountsLeft() + encoders.getCountsRight())/2;
  return ticks * TICKS_TO_DIST;
}

void move(double distance){
  double ticks = (encoders.getCountsLeft() + encoders.getCountsRight())/2;
  while(ticks * TICKS_TO_DIST < distance){
    motors.setSpeeds(200, 200);
    ticks = (encoders.getCountsLeft() + encoders.getCountsRight())/2;
  }
  motors.setSpeeds(0,0);
  exit(0);
}

void loop() {
  // put your main code here, to run repeatedly:

  error = target - getDistance();
  lastError = error;

  lcd.print(getDistance());
  delay(500);
  lcd.clear();

  double currTime = millis() * .001;
  double elapsedTime = currTime - pastTime;
  pastTime = currTime;
  
  errorSum += error * elapsedTime; 

  motorSpeed = ((kP * error) + (kI * errorSum)) * RPM_TO_CMS;

  motors.setSpeeds(motorSpeed, motorSpeed);
}
