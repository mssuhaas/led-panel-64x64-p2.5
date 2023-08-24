/****************************************************************

        Created By: Modadugu Surya Suhaas
        August 2023

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



String data_params[] = {"PM2.5","PM10","Temp"," rH","AQI"};
double data_values[] = {8.72,41.39,34.39,30.53,41.39};
String string_data = "PM2.5 : 8.72, PM10 : 41.39, Temperature : 34.39, Relative Humidity : 30.69, AQI : 41.39";
String units[] = {"ug/m3","ug/m3","C","%Rh",""};

// WiFi credentials
// const char *ssid = "SCRC_LAB_IOT";
// const char *password = "Scrciiith@123";
const char *ssid = "IIIT-Guest";
const char *password = "I%GR#*S@!";

// space required by the packages recieved
StaticJsonDocument<512> doc;

// hosted on port
AsyncWebServer server(PORT);

bool data_status = false;
bool data_start = false;
bool interrupt_enable = false;

String data_string = "Starting Up!";
String data_type = "CMD";

unsigned long start_time = 0;


uint8_t display_draw_time = 60; // 30-70 is usually fine


// PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

void IRAM_ATTR display_updater()
{
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable)
{
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

void wifi_init()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
// void scrollDisplayClear(){
//   display.fillRect(0, 55, 64, 9, display.color565(0, 0, 0));
// }

unsigned long last_draw = 0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  uint16_t text_length = text.length();
  display.setTextWrap(false); // we don't wrap text so it scrolls nicely
  display.setTextSize(1);
  display.setRotation(0);
  if(!data_start){
    //m_clearDisplay();
      m_updateData("PM2.5","PM10",32.0,228,"mg/l","mg/l");
  }
  int index = 0;

  // Asuming 5 pixel average character width
  for (int xpos = matrix_width; xpos > -(matrix_width + text_length * 5); xpos--)
  {

    if(interrupt_enable){
      interrupt_enable = false;
      break;
    } 

    display.setTextColor(display.color565(colorR, colorG, colorB));
    if((data_status)&&(data_start)){
      m_clearDisplay();
      // b_clearDisplay();
      display.setCursor(0,matrix_height/2-5);

    }
    else{
      b_clearDisplay();
    }    
    display.setTextColor(display.color565(colorR, colorG, colorB));
    display.setCursor(xpos, ypos);
    display.println(text);
    delay(scroll_delay);
    yield();
    if ((xpos%80==0)&&(!data_status)){
      if(index>=4){
        index=0;
      }
      if(data_start){
      m_clearDisplay();
      }
      m_updateData(data_params[index],data_params[index+1],data_values[index],data_values[index+1],units[index],units[index+1]);
      index++;
    }
    delay(scroll_delay / 5);
    yield();
  }
}


void data_recieve(AsyncWebServerRequest *request, unsigned char *data, size_t len, size_t index, size_t total)
{
  String stri;
  Serial.print("\n");
  for (int i = 0; i < len; i++)
  {
    stri += (char)data[i];
  }
  Serial.println("===============ONEM2M DATA RECIEVED=============================");
  Serial.print(stri);
  Serial.print("\n");

  DeserializationError error = deserializeJson(doc, stri);
  Serial.println("ONEM2M DATA PARSED");
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  // JsonObject m2m_cin = doc["m2m:sgn"]["m2m:nev"]["m2m:rep"]["m2m:cin"];
  // const char *m2m_cin_con = m2m_cin["con"];
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

  // If the data required needs to be in the format of string then use
  // action(conv_data);

  // If the data required needs to be in the format of char* then use
  data_type = conv_data_type;
  data_string = conv_data;

  data_status = true;
  interrupt_enable = true;

  char *c = const_cast<char *>(conv_data.c_str());
  // action(c);
  // send back a response!
  request->send(200, "text/plain", stri);
}

void show_aq(String data_URL){
  HTTPClient http;
  // http://10.2.131.168:80/aq
  http.begin(data_URL);
  // http.addHeader("Content-Type", "application/json");
  String payload = "";
  int httpCode = http.GET();
  if (httpCode > 0) {
    payload = http.getString();
    Serial.println("Data Recieved!");
    Serial.println(payload);
    DeserializationError error = deserializeJson(doc, payload);
    Serial.println("Data Parsing!");
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    Serial.println("========= DATA PARSING START =================");
    // data_values = doc["values"];
    // data_params = doc["params"];
    // string_data = doc["string"];
    
    const char *m2m_cin_con = doc["string"];
    String conv_data = m2m_cin_con;
    Serial.println("DATA : ");
    Serial.print(conv_data);
    Serial.println("========= DATA PARSING SUCCESS ===============");
  }
  else{
    Serial.println(httpCode);
    Serial.println("ERROR! NOT FOUND");
  }
}

void setup()
{
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
  delay(2000);
  display.clearDisplay();
  m_updateData("CO2","NH3",10.2,0.885,"mg/l","ug/l");
  t_updateData("SCRC");
  // char data_string[] = "[1692599580, 198.4, 310614.9, 25878.2, 411.0, 405.8, 411.3, 50.06, 2.0000, 238.3, 36.0, 8578.8, 236.3, 36.3, 8577.6, 234.5, 36.1, 8465.4, 752.4, 5.8, 4363.9, 768.9, 5.9, 4536.5, 752.3, 5.9, 4438.5, 747.2, 2.8, 2093.2, 751.7, 6.1, 4585.3, 759.1, 3.0, 2277.3]";
  // double daas[20];
  // int count = parseArrayString(data_string,daas,100);
  // for(int i=0;i<count;i++)
  //   Serial.printf("%.2lf ",daas[i]);
  // show_aq("http://10.2.131.168/aq");
}


void loop()
{
  Serial.println("STARTED LOOP");
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
  }
//  && (data_type == "CMD")
  if (data_status)
  {
    for (int j = 0; j < 2; j++)
    {
      display.setTextWrap(true);
      // display.setTextColor(display.color565(255, 0, 0));
      // display.setTextSize(1);
      // display.print("ATTENTION!");
      data_start = true;
      sound_buzzer(5, 350);
      // display.fillRect(0, 10, 64, 45, display.color565(0, 0, 0));
      // scroll_text(matrix_height / 2 - 5, 50, data_string, 96, 96, 250);
      m_show_text(data_string);
      // m_show_text();
      // m_updateData("CO2","NH3",10.2,0.885,"mg/l","ug/l");
      delay(700);
    }
    data_status = false;
    data_start = false;
  }
  else
  {
    // scroll_text(matrix_height / 2 - 5, 50, "Welcome to Smart City Living Lab!", 255, 0, 255);
    // show_aq("http://10.2.131.168/aq");
    scroll_text(56, 50, string_data, 0,191,255);
    b_clearDisplay();
  }
}






// String URL = "http://onem2m.iiit.ac.in/~/in-cse/in-name";

// String AQ = "/AE-AQ/AQ-KH00-00";
// String SL = "/AE-SL/SL-VN03-00";
// String srAC = "/AE-SR/SR-AC/SR-AC-KH00-00";
// String srAQ = "/AE-SR/SR-AQ/SR-AQ-KH95-03";
// String srEM = "/AE-SR/SR-EM/SR-EM-KH04-00";
// String WE = "/AE-WE/WE-VN04-00";
// String WD = "/AE-WM/WM-WD/WMM-WD-VN01-01";
// String WF = "/AE-WM/WM-WF/WM-WF-VN01-00";
// String WN = "/WE/WN/WN-L001-03"

// // String EM = "/AE-EM/EM-NC-VN90-01";
// // String srOC = "/AE-SR/SR-OC/SR-OC-GW-KH95-00";

// String sendGET(String node)
// {
//   String node_URL = URL + node + "/Data/la";
//   HTTPClient http;
//   http.begin(node_URL);
//   http.addHeader("X-M2M-Origin", "iiith_guest:iiith_guest");
//   http.addHeader("Content-Type", "application/json");
//   int httpCode = http.GET();
//   String payload = "";
//   if (httpCode > 0)
//   {
//     payload = http.getString();
//     Serial.println("Data Recieved!");
//     Serial.println(payload);
//     DeserializationError error = deserializeJson(doc.payload);
//     Serial.println("Data Parsing!");
//     if (error)
//     {
//       Serial.print("deserializeJson() failed: ");
//       Serial.println(error.c_str());
//       return;
//     }
//     Serial.println("========= DATA PARSING START =================");
//     const char *m2m_cin_con = doc["con"];
//     String conv_data = m2m_cin_con;
//     Serial.println("DATA : ");
//     Serial.print(conv_data);
//     Serial.println("========= DATA PARSING SUCCESS ===============");
//     return conv_data;
//   }
// }
