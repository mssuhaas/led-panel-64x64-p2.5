#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <string.h>
#include <ArduinoJson.h>
#include <PxMatrix.h>
#include <HTTPClient.h>

#include "data_parser.h"


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

PxMATRIX display(matrix_width, matrix_height, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);


void t_clearDisplay() {
  display.fillRect(0, 0, 64, 10, display.color565(0, 0, 0));
}

void m_clearDisplay() { 
  display.fillRect(0, 10, 64, 45, display.color565(0, 0, 0));
}

void b_clearDisplay() {
  display.fillRect(0, 55, 64, 10, display.color565(0, 0, 0));
}

void t_updateData(String text){
  t_clearDisplay();
  display.setCursor(19,1);
  display.setTextSize(1);
  display.setTextColor(display.color565(0,191,255));
  display.print(text);
  display.drawLine(0, 9, 64, 9, display.color565(255, 255, 255));
}

void m_updateData(String paramOne, String paramTwo, double dataOne, double dataTwo, String unitOne, String unitTwo){
  m_clearDisplay();
  display.drawRect(0, 10, 64, 45,display.color565(255, 255, 255)); 
  display.drawLine(31, 10, 31, 54, display.color565(255, 255, 255));
  display.drawLine(32, 10, 32, 54, display.color565(255, 255, 255));
  display.setTextSize(0);
  display.setTextColor(display.color565(255, 170, 20));
  display.setCursor(1, 12);
  display.drawLine(1, 20, 62, 20, display.color565(255, 255, 255));
  display.print(paramOne);
  display.setCursor(34, 12);
  display.print(paramTwo);
  display.setTextSize(1);
  display.setTextColor(display.color565(110, 250, 20));
  display.setCursor(1,30);
  display.printf("%.1lf",dataOne);
  display.setCursor(34, 30);
  display.printf("%.1lf",dataTwo);
  display.setTextColor(display.color565(255, 255, 255));
  display.setCursor(3,43);
  display.print(unitOne);
  display.setCursor(35,43);
  display.print(unitTwo);
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



