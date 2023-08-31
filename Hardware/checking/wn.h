#include "v_init.h"
DynamicJsonDocument doc(512);
float wn_values[MAX_DATA_SIZE];
String wn_params[MAX_DATA_SIZE];
String wn_units[MAX_DATA_SIZE];
int wn_dataCount = 0;
String wn_string_data = "";

void wn_printData() {
  Serial.println("Stored Data:");
  for (int i = 0; i < wn_dataCount; i++) {
    Serial.print("Parameter: ");
    Serial.print(wn_params[i]);
    Serial.print("\tValue: ");
    Serial.print(wn_values[i]);
    Serial.print("\tUnits: ");
    Serial.println(wn_units[i]);
  }
  Serial.println("String =  ");
  Serial.println(wn_string_data);
}

void wn_updateData(String jsonString) {
  deserializeJson(doc, jsonString);

  wn_dataCount = doc["count"];

  JsonArray paramsArray = doc["params"];
  JsonArray valuesArray = doc["values"];
  JsonArray unitsArray = doc["units"];
  const char *mew = doc["string"];
  // Serial.println(mew);
  wn_string_data = mew;


  for (int i = 0; i < wn_dataCount && i < MAX_DATA_SIZE; i++) {
    wn_values[i] = valuesArray[i];
    wn_params[i] = paramsArray[i].as<String>();
    wn_units[i] = unitsArray[i].as<String>();
  
  }
  wn_printData();
}

void wn_sendGET(void) {
  String url = "https://iiit-api.cringemonkey.tk/wn" ;
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
    wn_updateData(payload);
  } else {
    Serial.println("REQUEST COULD NOT BE SENT!");
  }http.end();
}
