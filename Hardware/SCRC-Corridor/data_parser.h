#include <Arduino.h>

int dataCount;

DynamicJsonDocument doc(512);

#define MAX_DATA_SIZE 12

int first = 0;
String data_params[MAX_DATA_SIZE] = {"PM2.5","PM10","Temp"," rH","AQI"};
double data_values[MAX_DATA_SIZE] = {8.72,41.39,34.39,30.53,41.39};
String units[MAX_DATA_SIZE] = {"ug/m3","ug/m3","C","%Rh",""};
String string_data = "PM2.5 : 8.72, PM10 : 41.39, Temperature : 34.39, Relative Humidity : 30.69, AQI : 41.39";


void printData() {
  Serial.println("Stored Data:");
  for (int i = 0; i < dataCount; i++) {
    Serial.print("Parameter: ");
    Serial.print(data_params[i]);
    Serial.print("\tValue: ");
    Serial.print(data_values[i]);
    Serial.print("\tUnits: ");
    Serial.println(units[i]);
  }
  Serial.print("String : ");
  Serial.println(string_data);
}

void updateData(String jsonString) {
  // DynamicJsonDocument doc(512);
  deserializeJson(doc, jsonString);

  dataCount = doc["count"];

  JsonArray paramsArray = doc["params"];
  JsonArray valuesArray = doc["values"];
  JsonArray unitsArray = doc["units"];
  const char *mew = doc["string"];
  string_data = mew;
  for (int i = 0; i < dataCount && i < MAX_DATA_SIZE; i++) {
    data_values[i] = valuesArray[i];
    data_params[i] = paramsArray[i].as<String>();
    units[i] = unitsArray[i].as<String>();
  }
  printData();
}

void sendGET(String vertical) {
  String url = "https://iiit-api.suryasuhaas.in/" + vertical ;
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
    updateData(payload);
  } else {
    Serial.println("REQUEST COULD NOT BE SENT!");
  }
  http.end();
}

// 10.2.201.147