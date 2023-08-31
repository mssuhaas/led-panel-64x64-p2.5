#include "v_init.h"
DynamicJsonDocument doc(512);
float wf_values[MAX_DATA_SIZE];
String wf_params[MAX_DATA_SIZE];
String wf_units[MAX_DATA_SIZE];
int wf_dataCount = 0;
String wf_string_data = "";

void wf_printData() {
  Serial.println("Stored Data:");
  for (int i = 0; i < wf_dataCount; i++) {
    Serial.print("Parameter: ");
    Serial.print(wf_params[i]);
    Serial.print("\tValue: ");
    Serial.print(wf_values[i]);
    Serial.print("\tUnits: ");
    Serial.println(wf_units[i]);
  }
  Serial.println("String =  ");
  Serial.println(wf_string_data);
}

void wf_updateData(String jsonString) {
  deserializeJson(doc, jsonString);

  wf_dataCount = doc["count"];

  JsonArray paramsArray = doc["params"];
  JsonArray valuesArray = doc["values"];
  JsonArray unitsArray = doc["units"];
  const char *mew = doc["string"];
  // Serial.println(mew);
  wf_string_data = mew;


  for (int i = 0; i < wf_dataCount && i < MAX_DATA_SIZE; i++) {
    wf_values[i] = valuesArray[i];
    wf_params[i] = paramsArray[i].as<String>();
    wf_units[i] = unitsArray[i].as<String>();
  
  }
  wf_printData();
}

void wf_sendGET(void) {
  String url = "https://iiit-api.cringemonkey.tk/wf" ;
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
    wf_updateData(payload);
  } else {
    Serial.println("REQUEST COULD NOT BE SENT!");
  }http.end();
}
