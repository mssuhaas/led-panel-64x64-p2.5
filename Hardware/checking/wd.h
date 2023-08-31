#include "v_init.h"
DynamicJsonDocument doc(512);
float wd_values[MAX_DATA_SIZE];
String wd_params[MAX_DATA_SIZE];
String wd_units[MAX_DATA_SIZE];
int wd_dataCount = 0;
String wd_string_data = "";

void wd_printData() {
  Serial.println("Stored Data:");
  for (int i = 0; i < wd_dataCount; i++) {
    Serial.print("Parameter: ");
    Serial.print(wd_params[i]);
    Serial.print("\tValue: ");
    Serial.print(wd_values[i]);
    Serial.print("\tUnits: ");
    Serial.println(wd_units[i]);
  }
  Serial.println("String =  ");
  Serial.println(wd_string_data);
}

void wd_updateData(String jsonString) {
  deserializeJson(doc, jsonString);

  wd_dataCount = doc["count"];

  JsonArray paramsArray = doc["params"];
  JsonArray valuesArray = doc["values"];
  JsonArray unitsArray = doc["units"];
  const char *mew = doc["string"];
  // Serial.println(mew);
  wd_string_data = mew;


  for (int i = 0; i < wd_dataCount && i < MAX_DATA_SIZE; i++) {
    wd_values[i] = valuesArray[i];
    wd_params[i] = paramsArray[i].as<String>();
    wd_units[i] = unitsArray[i].as<String>();
  
  }
  wd_printData();
}

void wd_sendGET(void) {
  String url = "https://iiit-api.cringemonkey.tk/wd" ;
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
    wd_updateData(payload);
  } else {
    Serial.println("REQUEST COULD NOT BE SENT!");
  }http.end();
}
