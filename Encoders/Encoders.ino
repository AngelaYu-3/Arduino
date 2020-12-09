#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

double TICKS_TO_DIST = 0.013614; //tiks to cm

void setup() {
  // put your setup code here, to run once:
  lcd.clear();
  encoders.init();
}

void driveTo(double distance, double motorSpeed){
    double isBackward = false;
    double currTicks = 0; 
    double currDist = 0;
    double targetTicks = distance / TICKS_TO_DIST;

    if(motorSpeed < 0){
      isBackward = true;
    }
    
    motors.setSpeeds(motorSpeed, motorSpeed);
    
    while(abs(currTicks) < targetTicks){
      currTicks += (encoders.getCountsAndResetLeft() + encoders.getCountsAndResetRight())/2;
      currDist = currTicks * TICKS_TO_DIST;
      lcd.clear();
      
      delay(2);
    }

    motors.setSpeeds(0, 0);
    //lcd.clear();
    lcd.print(currDist);
    delay(200);
    /*lcd.print("target reached!");
    
    for(int i = 0; i < 16; i++){
      lcd.scrollDisplayLeft();
      delay(150);
    }*/
}

void loop() {
  // put your main code here, to run repeatedly:
  bool isPressed = buttonA.isPressed();
  
  if(isPressed){   
      driveTo(30, 100);  //30 cm forwards
      driveTo(15, -100); //15 cm backwards
      driveTo(7.5, 100); //7.5 cm forwards
      driveTo(3.75, -100); //3.75 cm backwards
      
  }
}
