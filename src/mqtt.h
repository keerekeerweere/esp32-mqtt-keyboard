#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include "debug_log.h"



void mqtt_setup(void);
void mqtt_loop(void);
bool mqtt_publish_tele(void);
uint32_t decodeUtf8(const uint8_t* data, size_t length, size_t startIndex = 1);
