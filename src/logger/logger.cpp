#include <Arduino.h>
#include <ArduinoJson.h>
#include <webserver/RobotWebServer.cpp>
#include <core/rotary_encoder.cpp>

#ifndef LOGGER_CPP
#define LOGGER_CPP
/**
 * 
 * class providing logging 
 * functionality to the web interface 
 * and serial connection
 * 
*/


/**
 * Logging JSON format
 * {
 *      "time" : <millisecond timestamp>,
 *      "sensor" : "log",
 *      "data" : "Log message goes here"
 * }
 */

/**
 * Encoder data JSON format
 * {
 *      "time" : <millisecond timestamp>,
 *      "sensor" : "encoder",
 *      "wheel" : l|r
 *      "count" : <wheel count integer>
 *      "speed" : <current speed>
 *      "distance" : <total rotated distance> 
 * }
*/

class Logger
{

private:
    RobotWebServer server;

    String getTimeStamp()
    {
        unsigned long time = millis();
        unsigned long ms = time % 1000;
        unsigned long seconds = time/1000 % 60;
        unsigned long minutes = time/60000 % 60;
        unsigned long hour = time/3600000 % 60;
        
        String timeStamp = String(hour) + "h:" + 
                            String(minutes)+ "m:" + 
                            String(seconds) + "s" + 
                            String(ms) + "ms";
        return timeStamp;
    }

public:

/**
 * default constructor
 * _server: instance of a robotWebServer
*/
Logger(RobotWebServer &_server) : server(_server)
{

}

/**
 * convert the given string into logger format
*/
String createLogLine(String text)
{
    return "[" + getTimeStamp() + "]: " + text + "\n";
}

void print(String text)
{
    this->server.sendText(createLogLine(text));
}

/*
todos for this class:

- create interface for logger output types
    - init function
    - write function
    - add multiple log levels
- log to multiple outputs at the same time
- make it a singelton

*/


};

#endif //LOGGER_CPP