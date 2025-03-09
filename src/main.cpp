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
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.begin(115200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  server.start();

  // put your setup code here, to run once:
  delay(3000);
  robot.Stop();
}

void loop() {

  robot.Drive(255, PI_FOURTH);
  // update encoders
  for(int i = 0; i < 1; i++)
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