#include <Arduino.h>
#include <webserver/RobotWebServer.cpp>
#include<logger/logger.cpp>


RobotWebServer server = RobotWebServer();
Logger logger = Logger(server);
void setup() 
{
  Serial.begin(115200);
  server.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  logger.print("Hello there");
}