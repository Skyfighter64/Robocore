# Robocore

This is the Firmware for my DIY-Modded differential drive vacuum robot.
It is currently developed for ESP32-WROOM Dev Kit in combination with PlatformIO but can be adapted to other platforms and environments aswell.

Features:
- Dual H-Bridge motor driver with variable speed

## Requirements:
- PlatformIO 
- ESP32 Platform
- Arduino framework

## Notes
- All positions, distances are measured as integer values representing millimeter precision


## SPIFFS
The web server uses the spi ffs file system to store the web page. To build and update the ffs go to 
- platformio > Platform > Build Filesystem Image 
and
- platformio > Platform > Upload Filesystem Image 

The file system files are stored in the `/data` folder