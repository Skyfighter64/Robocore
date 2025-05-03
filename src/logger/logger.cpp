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


void print(String text)
{
    JsonDocument doc;
    doc["time"] = millis();
    doc["sensor"] = "log";
    doc["text"] = text;

    String output;
    serializeJson(doc, output);
    this->server.sendText(output);
}


/**
 * Encoder Logging
 */
void displayEncoderData(String wheel, RotaryEncoder encoder)
{
    JsonDocument doc;
    doc["time"] = millis();
    doc["sensor"] = "encoder";
    doc["wheel"] = wheel;
    doc["speed"] = encoder.getRPkS(); // speed in rounds per 1000s
    doc["count"] = encoder.getCount();
    doc["distance"] = encoder.getRounds() * 31.4159; // distance in cm // todo: replace hard coded U with robot stats

    String output;
    serializeJson(doc, output);
    this->server.sendText(output);
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