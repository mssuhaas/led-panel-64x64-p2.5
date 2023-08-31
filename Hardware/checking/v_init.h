#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <string.h>
#include <ArduinoJson.h> 
#include <HTTPClient.h>

#define MAX_DATA_SIZE 12
#define PORT 8100
// String vert[] = {"aq","srEM","wd","wf","wn"};



DynamicJsonDocument doc(512);
