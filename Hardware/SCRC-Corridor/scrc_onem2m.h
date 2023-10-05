#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <string.h>
#include <ArduinoJson.h>
#include <PxMatrix.h>
#include <HTTPClient.h>

#include "data_parser.h"

#define buzzer 0

#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_LAT 22
#define P_OE 16
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


#define matrix_width 64
#define matrix_height 64

PxMATRIX display(matrix_width, matrix_height, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

uint16_t orange = display.color565(255,255,0);
uint16_t cyan = display.color565(255, 255, 255);

// 0, 191, 255 cyan

void t_clearDisplay() {
  display.fillRect(0, 1, 64, 10, display.color565(0, 0, 0));
}

void m_clearDisplay() {
  display.fillRect(0, 10, 64, 45, display.color565(0, 0, 0));
  
}

void b_clearDisplay() {
  display.fillRect(1, 55, 62, 8, display.color565(0, 0, 0));
}

void t_updateData(String text){
  t_clearDisplay();
  display.setCursor(19,2);
  display.setTextSize(1);
  display.setTextColor(display.color565(0, 191, 255));
  display.print(text);

}

void m_updateData(bool blink = true){
  m_clearDisplay();
    
  display.drawLine(0, 1, 64, 1, cyan);
  display.drawLine(0, 0, 0, 63, cyan);
  display.drawLine(63, 0, 63, 63, cyan);
  display.drawLine(0, 63, 64, 63, cyan);
  display.drawLine(0, 53, 64, 53, cyan);
  // display.print("CELEBRATES 25 YEARS OF IIITH");
  display.setTextColor(display.color565(255, 170, 255));
  // display.setCursor(2,13);
  display.setTextWrap(true);
  display.setCursor(2,13);
  display.print(" WELCOMES");
  display.setCursor(2, 23);
  // display.setTextColor(orange);
  display.print("   YOU    ");
  // display.setCursor(2,32);
  // display.print("JANMASTAMI ");
  
    // display.setTextSize(2);
    // // display.setTextColor(display.color565(153,0,153));
    // display.setTextColor(orange);
    // display.print("25");
    // display.setTextSize(1);
    // display.setCursor(24, 27);
    // display.print(" YEARS");
  blink = !blink;

}

void m_show_text(String textse){
  m_clearDisplay();

  display.setCursor(0, 12);
  display.setTextWrap(true);
  display.setTextColor(display.color565(255,255,255));
  display.setTextSize(1);
  display.print(textse);
  delay(2000);
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


// // =================================================================================================================================== // 
// // =================================================       TEST        =============================================================== //
// // =================================================================================================================================== // 

// void m_updateData_new(String lOne, String lTwo, String lThree, String lFour){
//   m_clearDisplay();

//   Serial.println(lOne);
//   Serial.println(lTwo);
//   Serial.println(lThree);
//   Serial.println(lFour);

//   display.drawLine(0, 1, 64, 1, cyan);
//   display.drawLine(0, 0, 0, 63, cyan);
//   display.drawLine(63, 0, 63, 63, cyan);
//   display.drawLine(0, 63, 64, 63, cyan);
//   display.drawLine(0, 53, 64, 53, cyan);
//   display.setTextColor(display.color565(255, 170, 255));
//   display.setCursor(0,13);
//   display.setTextWrap(false);
//   display.print(lOne);
//   display.setCursor(0, 23);
//   display.setTextColor(orange);
//   display.print(lTwo);
//   display.setCursor(0,32);
//   display.print(lThree);
//   display.setTextSize(1);
//   display.setTextColor(display.color565(255, 170, 255));
//   display.setCursor(0, 42);
//   display.print(lFour);
    
//   // display.print("CELEBRATES 25 YEARS OF IIITH");
 
// }

// void update_m_data(AsyncWebServerRequest *request, unsigned char *data, size_t len, size_t index, size_t total) {
//   String stri;
//   Serial.print("\n");
//   for (int i = 0; i < len; i++) {
//     stri += (char)data[i];
//   }
//   Serial.println("===============ONEM2M DATA RECIEVED=============================");
  
//   Serial.println(stri);

//   DeserializationError error = deserializeJson(doc, stri);
//   Serial.println("ONEM2M DATA Converted!");
//   if (error) {
//     Serial.print("deserializeJson() failed: ");
//     Serial.println(error.c_str());
//     return;
//   }
//   const char *one = doc["one"];
//   const char *two = doc["two"];
//   const char *three = doc["three"];
//   const char *four = doc["four"];
//   String s_one = one;
//   String s_two = two;  
//   String s_three = three;  
//   String s_four = four;
//   Serial.print("one : ");
//   Serial.println(s_one);
//   Serial.print("two : ");
//   Serial.println(s_two);
//   Serial.print("three : ");
//   Serial.println(s_three);
//   Serial.print("four : ");
//   Serial.println(s_four);
//   Serial.println("======== oneM2M DATA PARSED! ==========");
//   m_updateData_new(s_one, s_two, s_three, s_four);
// request->send(200, "text/plain", stri);
// }


// 13:25:20.567 -> MAC ADDRESS : C8:F0:9E:9C:4A:98
// 


