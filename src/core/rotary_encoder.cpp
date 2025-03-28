#include <Arduino.h>
#include <stdint.h>
#include "FunctionalInterrupt.h"

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

  // interrupt routine
  void IRAM_ATTR isr() 
  {
    // increase count and delta
    count ++;
    delta ++;
  }

public: 
  /**
   * Constructor
   * 
   */
  RotaryEncoder(int pin, int diskLines, int historyThreshold)
  {
    this->pin = pin;
    this->diskLines = diskLines;
    this->historyThreshold = historyThreshold;

    // initialize sensor pin
    pinMode(pin, INPUT);
    // add interrupt to pin
    attachInterrupt(pin, std::bind(&RotaryEncoder::isr, this), RISING); 
  }
  /**
   * destructor
   */
  ~RotaryEncoder()
  {
    // detach the interrupt for this object
    detachInterrupt(pin);
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