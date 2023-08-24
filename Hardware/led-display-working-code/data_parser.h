#include <Arduino.h>

String sendGET(String node) {
  String URL = "http://onem2m.iiit.ac.in/~/in-cse/in-name";
  String node_URL = URL + node;
  HTTPClient http;
  http.begin(node_URL);
  http.addHeader("X-M2M-Origin", "iiith_guest:iiith_guest");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.GET();
  String payload = "";
  if (httpCode > 0) {
    payload = http.getString();
    Serial.println("Data Recieved!");
    Serial.println(payload);
    DeserializationError error = deserializeJson(doc.payload);
    Serial.println("Data Parsing!");
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    Serial.println("========= DATA PARSING START =================");
    const char *m2m_cin_con = doc["con"];
    String conv_data = m2m_cin_con;
    Serial.println("DATA : ");
    Serial.print(conv_data);
    Serial.println("========= DATA PARSING SUCCESS ===============");
  }
  return conv_data;
}
// void show_aq(String data_URL){
//   HttpClient http;
//   // http://10.2.131.168:5000/aq
//   http.begin(data_URL);
//   http.addHeader("Content-Type", "application/json");
//   int httpCode = http.GET();
//   String payload = "";
//   if (httpCode > 0) {
//     payload = http.getString();
//     Serial.println("Data Recieved!");
//     Serial.println(payload);
//     DeserializationError error = deserializeJson(doc.payload);
//     Serial.println("Data Parsing!");
//     if (error) {
//       Serial.print("deserializeJson() failed: ");
//       Serial.println(error.c_str());
//       return;
//     }
//     Serial.println("========= DATA PARSING START =================");
//     const char *m2m_cin_con = doc["string"];
//     String conv_data = m2m_cin_con;
//     Serial.println("DATA : ");
//     Serial.print(conv_data);
//     Serial.println("========= DATA PARSING SUCCESS ===============");
//   }
// }


int parseArrayString(const char *inputString, double *array, int maxSize) {
  int count = 0;

  // Skipping the opening '[' character
  inputString++;

  // Skip the first element
  while (*inputString != ',' && *inputString != '\0') {
    inputString++;
  }
  if (*inputString == ',') {
    inputString++;
  }

  while (*inputString != '\0' && count < maxSize) {
    if (*inputString == ',') {
      inputString++;
      continue;
    }

    if (*inputString == ']') {
      break;
    }

    if (*inputString == ' ') {
      inputString++;
      continue;
    }

    if (*inputString == '\0') {
      break;
    }

    double num;
    if (sscanf(inputString, "%lf", &num) == 1) {
      array[count] = num;
      count++;
      while (*inputString != ',' && *inputString != ']' && *inputString != '\0') {
        inputString++;
      }
    }
    inputString++;
  }

  return count;
}





