#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;

#define NUM_SENSORS 6 
unsigned int sensorValues[NUM_SENSORS]; 

int prevError = 0;
int maxSpeed = 400;
double kP = 1/4; //values need to be tuned
double kD = 6;

void calibrateSensors(){
  ledGreen(true);

  for(int i = 0; i < 120; i++){
    if(i > 60){
      motors.setSpeeds(-200, 200);
    }else{
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }

  motors.setSpeeds(0, 0);

  ledGreen(false);
}

void setup() {
  // put your setup code here, to run once:
  lcd.clear();
  lineSensors.initFiveSensors();
  calibrateSensors();
}

void loop() {
  // put your main code here, to run repeatedly:
  int position = lineSensors.readLine(sensorValues);
  lcd.print(position);

  int error = position - 2000;
  prevError = error;
  int speedDifference = (int)(kP * error) + (int)(kD * (error - prevError));
  int leftSpeed = maxSpeed + speedDifference;
  int rightSpeed = maxSpeed - speedDifference;

  motors.setSpeeds(leftSpeed, rightSpeed);

}
