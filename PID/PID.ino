#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

double TICKS_TO_DIST = 0.013614; 
double target = 30; //30 cm
double kP = 12;
double kI = 2.3;
double kD = 1;
double lastError = target;
double previousTime = 0;
double integralTerm = 0;
double derivativeTerm = 0;
double timeInterval = 0;
double error;
int power = 0;

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

void loop() {
  // put your main code here, to run repeatedly:
  
    error = target - getDistance();
    timeInterval = (millis() - previousTime) * .001; // in seconds
    integralTerm += (error * timeInterval);
    derivativeTerm = (error - lastError)/timeInterval;

    power = (int)(error * kP) + (int)(integralTerm * kI) + (int)(derivativeTerm * kD);

  if(error > 0.1){
    motors.setSpeeds(power, power);

    previousTime = millis();
    lastError = target - getDistance();
  }else{
    motors.setSpeeds(0, 0);
    lcd.print(getDistance());
  }
    
 
}
