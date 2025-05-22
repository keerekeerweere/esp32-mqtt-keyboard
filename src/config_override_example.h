#undef WIFI_SSID
#undef WIFI_PASSWORD
#undef UNIQUE_DEVICE_NAME
#undef MQTT_SERVER

#define WIFI_SSID            "YourWifiSSID"              // override here
#define WIFI_PASSWORD        "YourWifiPassword"          // override here

#define UNIQUE_DEVICE_NAME   "esp32_keyboard"            // override here
#define MQTT_SERVER          "IPAddressOfYourBroker"     // override here
#define MQTT_SERVER_PORT     1883                       // override it in file "config_override.h"
#define MQTT_USER            "YourEspMqttUsername"                         // override it in file "config_override.h"
#define MQTT_PASS            "YourEspMqttPassword"                         // override it in file "config_override.h"

#define LOGGING_ID 1
#define SYSLOG_SERVER "syslog.example.com"
#define SYSLOG_PORT 514
