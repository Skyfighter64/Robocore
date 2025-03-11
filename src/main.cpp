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


// process incoming events from the websocket
void processEvent(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
  {
    data[len] = 0;
    // check here for incoming button presses
    if (strcmp((char*)data, "stopRobot") == 0) 
    {
      //this->sendText("stopped robot");
      Serial.println("stopped robot");
      logger.print("Stopped");
      robot.Stop();
    }
    else if (strcmp((char*)data, "forward") == 0) 
    {
      Serial.println("Driving forward");
      logger.print("Driving forward");
      robot.Drive(255,0);
    }
    else if (strcmp((char*)data, "turnLeft") == 0) 
    {
      logger.print("turning left");
      robot.Drive(255,PI_FOURTH);
    }
    else if (strcmp((char*)data, "turnRight") == 0) 
    {
      logger.print("turning right");
      robot.Drive(255,-PI_FOURTH);
    }
  }
} 


// define how the web socket should behave on incoming commands
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type) 
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      // web socket events come in here
      Serial.printf("Got event data ");
      processEvent(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  server.start(onEvent);

  // put your setup code here, to run once:
  delay(3000);
  robot.Stop();
  robot.Drive(255, -PI_FOURTH);
}

void loop() {

  // drive forward
  //logger.print("Driving forward");
  
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