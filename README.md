
# esp32 mqtt keyboard
## Overview
This project turns an ESP32 <b>S3</b> into a HID keyboard. The keyboard emulates an <b>USB keyboard</b>, not a bluetooth keyboard.
* only works with an ESP32 S3 (tested) or ESP32 S2 (not tested), because the original ESP32 does not support native USB OTG
* Keystrokes can be send with MQTT commands (e.g. topic "esp32_keyboard/cmnd/UP", payload doesn't matter)

## ESP32 S3 connections
The ESP32 S3 Supermini normally has one USB connectors:
* one labeled "USB": 
    * used to upload the firmware for the first time only, afterwards OTA should be used
    * used to connect the ESP32 S3 as a keyboard to your device




## Installation
I used PlatformIO IDE for installation and flashing. Please have a look at <a href="https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation">this guide</a>. It is as simple as:
* install VSCode (Visual Studio Code)
* install PlatformIO as an VSCode extension
* clone this repository or download it
* use "open folder" in VSCode to open this repository
* check settings in "config.h" and "config_override.h"
* upload to ESP32
If you use PlatformIO, selection of the board and the USB mode is automatically done in file "platformio.ini".


* the first time the esp32-s3 supermini might require the boot button to be pressed while inserting the s3 into the usb port on your development workstation for programming
* OTA was added for easy uploading of sketches, even when the device is already connected to a 'host' usb device as keyboard
* 2 platform.ini profiles have been added, modify the ip address in the profile esp32-ota for OTA updates to work

* as the ESP32-S3 supermini only has 1 usb-c port that will be used for the keyboard, logging should no longer be sent over the usb/serial port
* logging has been somewhat enhanced by using syslog logging, the syslog server can be configured in the config overrides
 

You can also use the ArduinoIDE. Here you have to:
* manually install the library "PubSubClient"
* use all the files from folder "src" and put them into a folder called "esp32-mqtt-keyboard-arduino"
* rename "main.cpp" to "esp32-mqtt-keyboard-arduino.ino"
* open this folder in the Arduino IDE
* choose an ESP32 S3 board, e.g. "ESP32-S3-USB-OTG"
* Tools - "Upload Mode": "UART0 / Hardware CDC"
* Tools - "USB Mode": "USB OTG"

  


