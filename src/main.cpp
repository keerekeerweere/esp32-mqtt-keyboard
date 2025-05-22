#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "wifiCommunication.h"
#include "mqtt.h"
#include "keyboard.h"




#include <Elog.h>

#define MYLOG 0
#define ELOG_SYSLOG_ENABLE 1

#if defined(useOTAUpdate)
  // https://github.com/SensorsIot/ESP32-OTA
  #include "OTA.h"
  #if !defined(useOTA_RTOS)
    #include <ArduinoOTA.h>
  #endif
#endif

#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup(){}
void loop(){}
#else

unsigned long previousMillis10000Cycle = 0;
unsigned long interval10000Cycle = 10000;

void logger_setup(void) {
    Logger.configureSyslog(SYSLOG_SERVER, SYSLOG_PORT, UNIQUE_DEVICE_NAME); // Syslog server IP, port and device name
    Logger.registerSyslog(LOGGING_ID, ELOG_LEVEL_DEBUG, ELOG_FAC_USER, UNIQUE_DEVICE_NAME); // ...and syslog. Set the facility to user
}


void setup() {
  Serial.begin(115200);

  logger_setup();


  wifi_setup();
  wifi_enable();

  mqtt_setup();

  keyboard_setup();
}



void loop() {

  mqtt_loop();

  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis10000Cycle) >= interval10000Cycle) {
    previousMillis10000Cycle = currentMillis;
    mqtt_publish_tele();
  }

  // Sometimes initial WiFi connection fails and never recovers. In that case, restart ESP
  if ((currentMillis > 5000) && !WiFi.isConnected()) {
    ESP.restart();
  }

  #if defined(useOTAUpdate)
  if ((!OTAstarted) && WiFi.isConnected()) {
    OTA_setup("ESP32MQTTKEYBOARD");
    ArduinoOTA.begin();
    OTAstarted = true;
  }
  #endif
  #if defined(useOTAUpdate) && !defined(useOTA_RTOS)
  // If you do not use FreeRTOS, you have to regulary call the handle method
  ArduinoOTA.handle();
  #endif
}

#endif /* ARDUINO_USB_MODE */
