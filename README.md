
# yakvm - yet another kvm
## Overview
This setup enables remote control of a physical server, PC, or laptop using a lightweight KVM system built with an ESP32-S3 and a repurposed thin client. The system allows HDMI-over-IP video streaming and remote keystroke injection via MQTT.
## System Components and Data Flow

![Esp32-s3 Supermini](https://raw.githubusercontent.com/keerekeerweere/esp32-mqtt-keyboard/main/images/overview.png)

* Server, PC, or Laptop
 - HDMI Output is connected to a USB HDMI Capture Stick.
 - USB Input is provided by the ESP32-S3, which emulates a keyboard.

* USB HDMI Capture Stick
- Transfers the HDMI signal to the KVM device for processing and streaming.

* KVM Device (e.g., HP T520 Thin Client) Runs:
- uStreamer – Streams the HDMI video using the USB capture stick.
- NGINX – Serves an HTML5 page with the video stream and keystroke input.
- Mosquitto – MQTT broker that forwards keystrokes.
- Receives HDMI from the source machine and streams it to client browsers.
- Forwards keystrokes received from clients via MQTT.

* Client Device (Any HTML5-Capable Browser)
- Views the HDMI video stream served by the KVM device.
- Sends keyboard input via MQTT to the KVM's Mosquitto server.

* ESP32-S3
- Subscribes to the MQTT topic over Wi-Fi.
- Receives keystrokes from the KVM device.
- Emulates a USB keyboard, injecting input directly into the server, PC, or laptop.

## ESP32 S3 connections
The ESP32 S3 Supermini normally has one USB connectors:
* one labeled "USB": 
    * used to upload the firmware for the first time only, afterwards OTA should be used
    * used to connect the ESP32 S3 as a keyboard to your device

![Esp32-s3 Supermini](https://raw.githubusercontent.com/keerekeerweere/esp32-mqtt-keyboard/main/images/ESP32-S3-SuperMini.png)

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

  


