#include "v_init.h"

float aq_values[MAX_DATA_SIZE];
String aq_params[MAX_DATA_SIZE];
String aq_units[MAX_DATA_SIZE];
int aq_dataCount = 0;
String aq_string_data = "";

void aq_printData() {
  Serial.println("Stored Data:");
  for (int i = 0; i < aq_dataCount; i++) {
    Serial.print("Parameter: ");
    Serial.print(aq_params[i]);
    Serial.print("\tValue: ");
    Serial.print(aq_values[i]);
    Serial.print("\tUnits: ");
    Serial.println(aq_units[i]);
  }
  Serial.println("String =  ");
  Serial.println(aq_string_data);
}

void aq_updateData(String jsonString) {
  deserializeJson(doc, jsonString);

  aq_dataCount = doc["count"];

  JsonArray paramsArray = doc["params"];
  JsonArray valuesArray = doc["values"];
  JsonArray unitsArray = doc["units"];
  const char *mew = doc["string"];
  // Serial.println(mew);
  aq_string_data = mew;


  for (int i = 0; i < aq_dataCount && i < MAX_DATA_SIZE; i++) {
    aq_values[i] = valuesArray[i];
    aq_params[i] = paramsArray[i].as<String>();
    aq_units[i] = unitsArray[i].as<String>();
  
  }
  aq_printData();
}

  
void aq_sendGET(void) {
  String AQ = "aq";
  String url = "https://iiit-api.cringemonkey.tk/" + AQ ;
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
    aq_updateData(payload);
  } else {
    Serial.println("REQUEST COULD NOT BE SENT!");
  }
  http.end();
}






