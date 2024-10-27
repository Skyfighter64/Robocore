#include <Arduino.h>
/**
 * 
 * class providing logging 
 * functionality to the web interface 
 * and serial connection
 * 
*/

class Logger
{

private:
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
 * convert the given string into logger format
*/
String createLogLine(String text)
{
    return "[" + getTimeStamp() + "]: " + text + "\n";
}

void print(String text)
{
        // todo
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