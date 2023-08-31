#include "v_init.h"
DynamicJsonDocument doc(512);
float srEM_values[MAX_DATA_SIZE];
String srEM_params[MAX_DATA_SIZE];
String srEM_units[MAX_DATA_SIZE];
int srEM_dataCount = 0;
String srEM_string_data = "";

void srEM_printData() {
  Serial.println("Stored Data:");
  for (int i = 0; i < srEM_dataCount; i++) {
    Serial.print("Parameter: ");
    Serial.print(srEM_params[i]);
    Serial.print("\tValue: ");
    Serial.print(srEM_values[i]);
    Serial.print("\tUnits: ");
    Serial.println(srEM_units[i]);
  }
  Serial.println("String =  ");
  Serial.println(srEM_string_data);
}

void srEM_updateData(String jsonString) {
  deserializeJson(doc, jsonString);

  srEM_dataCount = doc["count"];

  JsonArray paramsArray = doc["params"];
  JsonArray valuesArray = doc["values"];
  JsonArray unitsArray = doc["units"];
  const char *mew = doc["string"];
  // Serial.println(mew);
  srEM_string_data = mew;


  for (int i = 0; i < srEM_dataCount && i < MAX_DATA_SIZE; i++) {
    srEM_values[i] = valuesArray[i];
    srEM_params[i] = paramsArray[i].as<String>();
    srEM_units[i] = unitsArray[i].as<String>();
  
  }
  srEM_printData();
}

void srEM_sendGET(void) {
  String url = "https://iiit-api.cringemonkey.tk/srEM" ;
  Serial.println("IN sendGET");
  HTTPClient http;
  http.begin(url);
  String payload = "";
  int httpCode = http.GET();
  Serial.print("HTTP : ");
  Serial.println(httpCode);
  if (httpCode > 0) {
    payload = http.getString();
    Serial.println("Data Recieved!");
    Serial.println(payload);
    srEM_updateData(payload);
  } else {
    Serial.println("REQUEST COULD NOT BE SENT!");
  }http.end();
}
