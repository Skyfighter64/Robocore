#include <Arduino.h>
#include <core/differential_drive.cpp>


/**
 * This file shows how to let a differential drive robot drive around
 * with a given angle and speed
*/


DifferentialDrive robot = DifferentialDrive();
void setup() 
{
  // put your setup code here, to run once:
  delay(3000);
  robot.Stop();
  // test different speeds
  robot.Drive(128, 0); // half speed forward
  delay(1000);
  robot.Drive(255, 0); // full speed forward
  delay(1000);
  robot.Drive(-255, 0); // Full speed backwards
  delay(1000);
  robot.Drive(0, 0); // stop
  delay(1000);

  // test different angles
  robot.Drive(255, PI_FOURTH); // One-wheel rotation to the left (counterclockwise)
  delay(1000);
  robot.Drive(255, -PI_FOURTH); // One-wheel rotation to the right (clockwise)
  delay(1000);
  robot.Drive(255, 2*PI_FOURTH); // Turn-around counterclockwise
  delay(1000);
  robot.Drive(255, -2*PI_FOURTH); // Turn-around clockwise
  delay(1000);
  robot.Drive(255, 3*PI_FOURTH); // backwards one-wheel rotation counterclockwise
  delay(1000);
  robot.Drive(255, -3*PI_FOURTH); // backwards one-wheel rotation clockwise
  delay(1000);
  robot.Drive(255, 4*PI_FOURTH); // also full speed backwards, but with angle
  delay(1000);

  robot.Stop();
  delay(1000);

  // try some in-between rotations
  for(double angle = 0; angle < TWO_PI; angle += 0.1)
  {
    robot.Drive(255, angle);
    delay(100);
  }

  robot.Stop();
}

void loop() {
  // put your main code here, to run repeatedly:
}