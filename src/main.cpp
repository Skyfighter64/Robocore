#include <Arduino.h>
#include <webserver/RobotWebServer.cpp>
#include <logger/logger.cpp>
#include <core/rotary_encoder.cpp>
#include <core/differential_drive.cpp>


//define rotary encoder pin numbers
#define LEFT_ENCODER 2
#define RIGHT_ENCODER 5
// encoder specific parameters
#define ENCODER_STEPS 36
#define HISTORY_SIZE 5

RobotWebServer server = RobotWebServer();
Logger logger = Logger(server);

RotaryEncoder encoderL(LEFT_ENCODER, ENCODER_STEPS, HISTORY_SIZE);;
RotaryEncoder encoderR(RIGHT_ENCODER, ENCODER_STEPS, HISTORY_SIZE);

DifferentialDrive robot = DifferentialDrive();

void setup() 
{
  //Serial.begin(115200);
  server.start();
}

void loop() {
  // update encoders
   for(int i = 0; i < 1000; i++)
   {
     encoderL.Update();
     encoderR.Update();
   }
   if(encoderL.peekCountDelta() > 0 || encoderR.peekCountDelta() > 0)
   {
     logger.print("Left: ");
     logger.print(String(encoderL.getCount()));
     logger.print("Right: ");
     logger.print(String(encoderR.getCount()));
   }
}