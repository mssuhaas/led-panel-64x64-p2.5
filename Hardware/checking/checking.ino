// #include "wd.h"
// #include "wf.h"
// #include "wn.h"
#include "aq.h"
// #include "srEM.h"
#include "wifi_connect.h"

// const char *ssid = "Test";
// const char *password = "123456789";

// float values[MAX_DATA_SIZE];
// String params[MAX_DATA_SIZE]; // Assuming max parameter name length is 10 characters
// String units[MAX_DATA_SIZE];  // Assuming max unit name length is 10 characters
// int dataCount = 0; // To keep track of the number of data points
// String string_data = "";
AsyncWebServer server(PORT);
DynamicJsonDocument docu(512);


void data_recieve(AsyncWebServerRequest *request, unsigned char *data, size_t len, size_t index, size_t total) {
  String stri;
  Serial.print("\n");
  for (int i = 0; i < len; i++) {
    stri += (char)data[i];
  }
  Serial.println("===============ONEM2M DATA RECIEVED=============================");
  Serial.print(stri);
  Serial.print("\n");
  DeserializationError error = deserializeJson(docu, stri);
  Serial.println("ONEM2M DATA PARSED");
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  const char *m2m_data_type = docu["type"];
  const char *m2m_cin_con = docu["data"];
  String conv_data;
  String conv_data_type;
  conv_data_type = m2m_data_type;
  conv_data = m2m_cin_con;

  Serial.print("DATA : ");
  Serial.println(conv_data);
  Serial.print("TYPE : ");
  Serial.println(conv_data_type);
  Serial.println("======== oneM2M DATA PARSED! ==========");

  // If the data required needs to be in the format of char* then use
  // data_type = conv_data_type;
  // data_string = conv_data;

  // data_status = true;
  // interrupt_enable = true;
  //char *c = const_cast<char *>(conv_data.c_str());
  // action(c);
  // send back a response!
  request->send(200, "text/plain", stri);
}


// void printData() {
//   Serial.println("Stored Data:");
//   for (int i = 0; i < dataCount; i++) {
//     Serial.print("Parameter: ");
//     Serial.print(params[i]);
//     Serial.print("\tValue: ");
//     Serial.print(values[i]);
//     Serial.print("\tUnits: ");
//     Serial.println(units[i]);
//   }
//   Serial.println("String =  ");
//   Serial.println(string_data);
// }



// void updateData(String jsonString) {
//   deserializeJson(doc, jsonString);

//   dataCount = doc["count"];

//   JsonArray paramsArray = doc["params"];
//   JsonArray valuesArray = doc["values"];
//   JsonArray unitsArray = doc["units"];
//   const char *mew = doc["string"];
//   // Serial.println(mew);
//   string_data = mew;


//   for (int i = 0; i < dataCount && i < MAX_DATA_SIZE; i++) {
//     values[i] = valuesArray[i];
//     params[i] = paramsArray[i].as<String>();
//     units[i] = unitsArray[i].as<String>();
  
//   }
//   printData();
// }

// void sendGET(String vertical) {
//   String url = "https://iiit-api.cringemonkey.tk/" + vertical;
//   Serial.println("IN sendGET");
//   HTTPClient http;
//   http.begin(url);
//   String payload = "";
//   int httpCode = http.GET();
//   Serial.print("HTTP : ");
//   Serial.println(httpCode);
//   if (httpCode > 0) {
//     payload = http.getString();
//     Serial.println("Data Recieved!");
//     Serial.println(payload);
//     updateData(payload);
//   } else {
//     Serial.println("REQUEST COULD NOT BE SENT!");
//   }
// }


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println("IN SETUP");
  wifi_init();
    server.on(
    "/",
    HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    data_recieve);
  server.begin();
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("IN LOOP!");  
  
    aq_sendGET();
    // wn_sendGET();
    // srEM_sendGET();
    // wd_sendGET();
    // wf_sendGET();
    delay(1000);
}





