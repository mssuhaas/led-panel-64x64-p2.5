/****************************************************************

        Created By: Modadugu Surya Suhaas
        August 2023
        Node: SCRC Corridor

******************************************************************/
// Wi-Fi
#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <string.h>
#include <ArduinoJson.h>
#include <PxMatrix.h>
#include <HTTPClient.h>
#include "scrc_onem2m.h"

#define PORT 8100
#define BAUD 115200

String verticals[] = { "aq","wd", "wf", "wn","srEM" };
int num_verticals = 5;
int curr_vertical = 0;

bool wd_status = false;

// WiFi credentials
// const char *ssid = "SCRC_LAB_IOT";
// const char *password = "Scrciiith@123";

const char *ssid = "esw-m19@iiith";
const char *password = "e5W-eMai@3!20hOct";
// #define WIFI_SSID 
// #define WIFI_PASSWORD "e5W-eMai@3!20hOct"

// const char *ssid = "Test";
// const char *password = "123456789";

// space required by the packages recieved


// hosted on port
AsyncWebServer server(PORT);

bool data_status = false;
bool data_start = false;
bool interrupt_enable = false;

String data_string = "Starting Up!";
String data_type = "";




uint8_t display_draw_time = 60;  // 30-70 is usually fine


void IRAM_ATTR display_updater() {
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}


// =================================================================================================================================== // 
// =================================================       TEST        =============================================================== //
// =================================================================================================================================== // 

void m_updateData_new(String lOne, String lTwo, String lThree, String lFour){
  m_clearDisplay();

  Serial.println(lOne);
  Serial.println(lTwo);
  Serial.println(lThree);
  Serial.println(lFour);

  display.drawLine(0, 1, 64, 1, cyan);
  display.drawLine(0, 0, 0, 63, cyan);
  display.drawLine(63, 0, 63, 63, cyan);
  display.drawLine(0, 63, 64, 63, cyan);
  display.drawLine(0, 53, 64, 53, cyan);
  display.setTextColor(display.color565(255, 170, 255));
  display.setCursor(2,13);
  display.setTextWrap(false);
  display.print(lOne);
  display.setCursor(2, 23);
  display.setTextColor(orange);
  display.print(lTwo);
  display.setCursor(2,32);
  display.print(lThree);
  display.setTextSize(1);
  display.setTextColor(display.color565(255, 170, 255));
  display.setCursor(2, 42);
  display.print(lFour);
    
  // display.print("CELEBRATES 25 YEARS OF IIITH");
 
}

void update_m_display(AsyncWebServerRequest *request, unsigned char *data, size_t len, size_t index, size_t total) {
  String stri;
  Serial.print("\n");
  for (int i = 0; i < len; i++) {
    stri += (char)data[i];
  }
  Serial.println("===============ONEM2M DATA RECIEVED=============================");
  
  Serial.println(stri);

  DeserializationError error = deserializeJson(doc, stri);
  Serial.println("ONEM2M DATA Converted!");
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  const char *one = doc["one"];
  const char *two = doc["two"];
  const char *three = doc["three"];
  const char *four = doc["four"];
  String s_one = one;
  String s_two = two;  
  String s_three = three;  
  String s_four = four;
  Serial.print("one : ");
  Serial.println(s_one);
  Serial.print("two : ");
  Serial.println(s_two);
  Serial.print("three : ");
  Serial.println(s_three);
  Serial.print("four : ");
  Serial.println(s_four);
  Serial.println("======== oneM2M DATA PARSED! ==========");
  m_updateData_new(s_one, s_two, s_three, s_four);
request->send(200, "text/plain", stri);
}

void data_recieve(AsyncWebServerRequest *request, unsigned char *data, size_t len, size_t index, size_t total) {
  String stri;
  Serial.print("\n");
  for (int i = 0; i < len; i++) {
    stri += (char)data[i];
  }
  Serial.println("===============ONEM2M DATA RECIEVED=============================");
  Serial.print(stri);
  Serial.print("\n");

  DeserializationError error = deserializeJson(doc, stri);
  Serial.println("ONEM2M DATA PARSED");
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  const char *m2m_data_type = doc["type"];
  const char *m2m_cin_con = doc["data"];
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
  data_type = conv_data_type;
  data_string = conv_data;

  data_status = true;
  interrupt_enable = true;
  //char *c = const_cast<char *>(conv_data.c_str());
  // action(c);
  // send back a response!
  request->send(200, "text/plain", stri);
}

void display_update_enable(bool is_enable) {
  if (is_enable) {
    timer = timerBegin(0, 90, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  } else {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

void wifi_init() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC ADDRESS : ");
  Serial.println(WiFi.macAddress());
}


void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB) {
  uint16_t text_length = text.length();
  display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
  display.setTextSize(1);
  display.setRotation(0);

  // Asuming 5 pixel average character width
  for (int xpos = matrix_width; xpos > -(matrix_width + text_length * 5); xpos--) {
    if (interrupt_enable) {
      interrupt_enable = false;
      break;
    }
    display.setTextColor(display.color565(colorR, colorG, colorB));
    if ((data_status) && (data_start)) {
      // m_clearDisplay();
      // b_clearDisplay();
      display.setCursor(0, matrix_height / 2 - 5);
    } else {
      b_clearDisplay();
    }
    display.setTextColor(display.color565(colorR, colorG, colorB));
    display.setCursor(xpos, ypos);
    display.println(text);
  display.drawLine(0, 0, 0, 63, cyan);
  display.drawLine(63, 0, 63, 63, cyan);
  display.drawLine(0, 63, 64, 63, cyan);
    delay(scroll_delay);
    yield();
    delay(scroll_delay / 5);
    yield();
  }

}



void setup() {
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

  server.on(
    "/update",
    HTTP_POST,
    [](AsyncWebServerRequest *request){},
    NULL,
    update_m_display);
    
  server.begin();
  pinMode(buzzer, OUTPUT);
  display_update_enable(true);
  Serial.println("BEGIN DISPLAY");
  display.begin(32);
  display.setFastUpdate(true);
  Serial.println("DISPLAY INIT OVER");
  display.setTextWrap(true);
  display.setTextSize(0);
  display.setTextColor(display.color565(120, 55, 250));
  display.setCursor(0, 0);
  display.print(WiFi.localIP());
  sendGET(verticals[curr_vertical++]);
  // updateAQ();
  // sendGET("aq");
  delay(2000);
  display.clearDisplay();
  // m_updateData("CO2","NH3",10.2,0.885,"mg/l","ug/l");
  int index = 0;
  display.drawLine(0, 0, 0, 63, cyan);
  m_updateData(true);
  // index++;
  t_updateData("SCRC");
  display.drawLine(0, 0, 64, 0, cyan);
}


void loop() {
  Serial.println("STARTED LOOP");
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
  }
  //  && (data_type == "CMD")
  if ((data_status) && (data_type == "CMD")) {
    sendGET(data_string);
    Serial.print("DEBUG :: ");
    Serial.println(data_string);
    data_status = false;
  }
  if (data_status) {
    for (int j = 0; j < 2; j++) {
      display.setTextWrap(true);
      data_start = true;
      sound_buzzer(5, 350);
      // m_show_text(data_string);
      display.fillRect(0, 10, 64, 45, display.color565(0, 0, 0));
      scroll_text(matrix_height / 2 - 5, 50, data_string, 96, 96, 250);
      // scroll_text()
      delay(700);
    }
    data_status = false;
    data_start = false;

  } else {
    scroll_text(55, 50, string_data, 0,255,0);
    
    b_clearDisplay();
  }
  if( curr_vertical >= 5){
  curr_vertical = 0;
  }
  
  sendGET(verticals[curr_vertical]);
  curr_vertical++;
  // sendGET(verticals[0]);
}




