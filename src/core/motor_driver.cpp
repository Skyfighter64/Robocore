#include <stdint.h>
#include <Arduino.h>

#ifndef MOTOR_DRIVER_CPP
#define MOTOR_DRIVER_CPP
/**
 * Motor Driver class for classical H-Bridge Motor drivers
 * with Reverse and Forward inputs
 * 
*/


class MotorDriver
{
private:
    // digital IO pin number at which the sensor is connected
    int forwardPin; 
    int reversePin;
    // the current speed of the motor from -255 to 255
    // -255 for full speed reverse to 255 for full speed forward
    int speed = 0;
    // the minimum speed at which the motor starts driving.
    int minSpeed = 0;

    /**
     * Apply the current speed and direction to the motor
     * Note: The order of calls is important here to not get 
     *       a cross-current in unprotected drivers
     */
    void UpdateMotor()
    {
           if(speed > minSpeed)
           {
                // drive forward
                analogWrite(reversePin, 0);
                analogWrite(forwardPin, abs(speed));
           }
           else if (speed < -minSpeed)
           {
                // drive backwards 
                analogWrite(forwardPin, 0);
                analogWrite(reversePin, abs(speed));
           }
           else
           {
                // turn off motor 
                analogWrite(forwardPin, 0);
                analogWrite(reversePin, 0);
           }
    }

public:
     /**
     * Constructor
     * forwardPin: the data pin at which the motor driver's forward signal is connected; has to be a PWM capable pin
     * reversePin: the data pin at which the motor driver's reverse signal is connected; has to be a PWM capable pin
     * minSpeed: the minimum speed at which the motor starts driving
    */
    MotorDriver(int forwardPin, int reversePin, int minSpeed)
    {
        this->forwardPin = forwardPin;
        this->reversePin = reversePin;
        this->minSpeed = minSpeed;

        pinMode(forwardPin, OUTPUT);
        pinMode(reversePin, OUTPUT);

        analogWrite(forwardPin, 0);
        analogWrite(reversePin, 0);
    }

    /**
     * Constructor
     * forwardPin: the data pin at which the motor driver's forward signal is connected; has to be a PWM capable pin
     * reversePin: the data pin at which the motor driver's reverse signal is connected; has to be a PWM capable pin
     * Note: Uses default minSpeed of 64/255
    */
    MotorDriver(int forwardPin, int reversePin) : MotorDriver(forwardPin, reversePin, 64){}


    /**
     * Get the current driving direction of this motor
     * returns:
     *  1 if driving forward
     *  0 if standing still
     *  -1 if driving backwards
    */
    int GetDirection()
    {
        return (speed > 0) - (speed < 0);
    }

    /**
     * set the speed for this motor
     * newSpeed: the speed to set to:
     * -255: full speed reverse
     *    0: stopped
     *  255: full speed
    */
    void SetSpeed(int newSpeed)
    {
        if(newSpeed > 255)
        {
            speed = 255;
        }
        else if (newSpeed < -255)
        {
            speed = -255;
        }
        else
        {
            speed = newSpeed;
        }

        // apply new speed to motor
        UpdateMotor();
    }

    /**
     * Get the current speed of the motor from -255 to 255
     * -255: full speed reverse
     *    0: stopped
     *  255: full speed
     * 
    */
    int GetSpeed()
    {
        return speed;
    }

    /**
     * Stop the motor from turning
    */
    void Stop()
    {
        SetSpeed(0);
    }


};
#endif //MOTOR_DRIVER_CPP