#include <Arduino.h>
#include <stdint.h>

#ifndef ROTARY_ENCODER_CPP
#define ROTARY_ENCODER_CPP
/** 
 *  Rotary encoder library for (optical) counters such as HW-201
 *  
 *  Copyright 2024 Skyfighter64 (https://github.com/Skyfighter64/)
 */


class RotaryEncoder
{
private:
  // digital IO pin number at which the sensor is connected
  int pin; 
  // nr of lines on the rotating disk
  int diskLines;
  // minimum consecutive number of times the measurement has to be the same to count (max. is 2* threshold)
  // (technically the actual threshold can be up to the double depending on the situation)
  int historyThreshold;
  
  
  int sensorHistory = 0;
  bool lastMeasurement = false;
  
  
  // total count of steps since beginning of measurement
  uint32_t count = 0;
  // difference to last value returned by either getCountDelta() or getCount()
  uint32_t delta = 0;

  /**
   * Function to measure the next sensor value.
   * @return: the measurement result (0/1) or -1 if no 
   * valid result was determined
   */
  int Measure()
  {
    //read pin, check if count needs increase, update history, update lastMeasurement if needed
    bool measurement = digitalRead(pin);
    // increase / decrease sensorHistory depending on the measurement
    sensorHistory += measurement ? 1 : -1;


    // check if enough consecutive 1's / 0's are reached
    // this is done for debouncing/smoothening the output
    if(sensorHistory >= historyThreshold)
    {
      sensorHistory = 0;
      return 1;
    }
    else if (sensorHistory < -historyThreshold)
    {
      sensorHistory = 0;
      return 0;
    }    

    // no result yet
    return -1;
  }

  
  /**
   * Constructor
   * 
   */
 public:
  RotaryEncoder(int pin, int diskLines, int historyThreshold)
  {
    this->pin = pin;
    this->diskLines = diskLines;
    this->historyThreshold = historyThreshold;

    // initialize sensor pin
    pinMode(pin, INPUT);
  }
  
  void Update()
  {
    int measurement = this->Measure();
    if(measurement == -1)
    {
      // do nothing
      return;
    }

    if (measurement && ! lastMeasurement)
    {
      //sensor measured a low and then a high
      count ++;
      delta ++;
    }

    lastMeasurement = (bool) measurement;
  }

  int getCount()
  {
    // reset delta
    delta = 0;
    return count;
  }
  /**
   * return the number of steps since the last call
   * of either getCount() or getCountDelta()
   */
  int getCountDelta()
  {
    // save delta for return and reset it
    uint32_t _delta = delta;
    delta = 0;
    return _delta;
  }

  /**
   * return the number of steps since the last call
   * of either getCount() or getCountDelta() without 
   * resetting the delta counter
   */
  int peekCountDelta()
  {
    return delta;
  }

  /** 
   *  return how many full rotations have been done since the beginning
   */
  int getRounds()
  {
    return count / diskLines;
  }

  /** 
   *  return how many steps (black lines of the encoder wheel) have been counted since last full rotation
   */
  int getSteps()
  {
    return count % diskLines;
  }

};

#endif //ROTARY_ENCODER_CPP