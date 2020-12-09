#include <Wire.h>
#include <Zumo32U4.h>

const uint16_t maxSpeed = 200;

Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;

int lastError = 0;
double kP = 0.6;
int kD = 7;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

void calibrateSensors()
{
  lcd.clear();
  delay(1000);
  
  for(int i = 0; i < 120; i++)
  {
     if (i > 30 && i <= 90){
      motors.setSpeeds(-200, 200);
    }else{
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void setup()
{

  lineSensors.initFiveSensors();

  // Wait for button A to be pressed and released.
  lcd.clear();
  lcd.print(F("Press A"));
  lcd.gotoXY(0, 1);
  lcd.print(F("to calib"));
  buttonA.waitForButton();

  calibrateSensors();
  lcd.clear();
  lcd.print(F("Go!"));
}

void loop()
{
  int position = lineSensors.readLine(lineSensorValues); //left < right

  int16_t error = position - 2000; 

  int16_t speedDifference = (error * kP) + (kD * (error - lastError));

  lastError = error;

  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

  // Constrain our motor speeds to be between 0 and maxSpeed.
  // One motor will always be turning at maxSpeed, and the other
  // will be at maxSpeed-|speedDifference| if that is positive,
  // else it will be stationary.  For some applications, you
  // might want to allow the motor speed to go negative so that
  // it can spin in reverse.
  leftSpeed = constrain(leftSpeed, -100, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, -100, (int16_t)maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
}
