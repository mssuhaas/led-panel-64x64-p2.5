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

#define PORT 8100
#define BAUD 115200

#define buzzer 0

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 21
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define matrix_width 64
#define matrix_height 64

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
String data_string = "Starting Up!";
String data_type = "CMD";

uint8_t display_draw_time = 60; // 30-70 is usually fine

PxMATRIX display(matrix_width, matrix_height, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);
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

unsigned long last_draw = 0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  uint16_t text_length = text.length();
  display.setTextWrap(false); // we don't wrap text so it scrolls nicely
  display.setTextSize(1);
  display.setRotation(0);
  display.setTextColor(display.color565(colorR, colorG, colorB));

  // Asuming 5 pixel average character width
  for (int xpos = matrix_width; xpos > -(matrix_width + text_length * 5); xpos--)
  {
    display.setTextColor(display.color565(colorR, colorG, colorB));
    display.clearDisplay();
    display.setCursor(xpos, ypos);
    display.println(text);
    delay(scroll_delay);
    yield();

    delay(scroll_delay / 5);
    yield();
  }
}

char *extract_data_string_parameters(const char *xmlString)
{
  const char *startTag = "<str name=\"Data String Parameters\" val=\"";
  const char *endTag = "\"/>";
  const char *nameStart = strstr(xmlString, startTag);

  if (nameStart != NULL)
  {
    nameStart += strlen(startTag);
    const char *valueStart = nameStart;
    const char *valueEnd = strstr(valueStart, endTag);

    if (valueEnd != NULL)
    {
      int length = valueEnd - valueStart;
      char *result = (char *)malloc(length + 1); // +1 for null-terminator

      if (result != NULL)
      {
        strncpy(result, valueStart, length);
        result[length] = '\0'; // Null-terminate the string
        return result;
      }
    }
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
  char *c = const_cast<char *>(conv_data.c_str());
  // action(c);
  // send back a response!
  request->send(200, "text/plain", stri);
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
  // display.begin(32,CLK,MOSI,MISO,SS);
  // 1/32 display
  display.setTextWrap(true);
  display.setTextSize(0);
  display.setTextColor(display.color565(120, 55, 250));
  display.setCursor(0, 0);
  display.print(WiFi.localIP());
  delay(2000);
  display.clearDisplay();
}

void loop()
{
  Serial.println("STARTED LOOP");
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
  }

  if (data_status && (data_type == "CMD"))
  {
    for (int j = 0; j < 2; j++)
    {
      display.setTextWrap(true);
      display.setTextColor(display.color565(255, 0, 0));
      display.setTextSize(2);
      display.print("WARNING!");
      // display.drawRGBBitmap()
      sound_buzzer(5, 350);
      scroll_text(matrix_height / 2 - 5, 50, data_string, 96, 96, 250);
      delay(700);
    }
    data_status = false;
  }
  else
  {
    scroll_text(matrix_height / 2 - 5, 50, "Welcome to Smart City Living Lab!", 255, 0, 255);
    display.clearDisplay();
  }
}

void sound_buzzer(uint8_t times, uint8_t delay_time)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(delay_time);
    digitalWrite(buzzer, LOW);
    delay(delay_time);
  }
}

String URL = "http://onem2m.iiit.ac.in/~/in-cse/in-name/";

void sendGET(String node)
{
  HTTPClient http;
  http.begin(nodeURL);
  http.addHeader("X-M2M-Origin", "iiith_guest:iiith_guest");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.GET();
  String payload = "";
  if (httpCode > 0)
  {
    payload = http.getString();
    Serial.println("Data Recieved!");
    Serial.println(payload);
    DeserializationError error = deserializeJson(doc.payload);
    Serial.println("Data Parsing!");
    if (error)
    {
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
}