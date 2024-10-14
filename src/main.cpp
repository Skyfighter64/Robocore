#include <Arduino.h>
#include <core/differential_drive.cpp>

DifferentialDrive drive = DifferentialDrive();
void setup() 
{
  // put your setup code here, to run once:
  drive.DriveCircle(0,255);
  delay(2000);
  drive.DriveCircle(500.0,255);
  delay(5000);
  drive.DriveCircle(std::numeric_limits<float>::infinity(), 255);
  delay(2000);
  drive.Stop();
}

void loop() {
  // put your main code here, to run repeatedly:
}