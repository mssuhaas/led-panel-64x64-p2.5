#include "Arduino.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <string.h>
#include <ArduinoJson.h>

WiFiClient client;

String date;

#define NUM_MAX 4
#define LINE_WIDTH 16
#define ROTATE  90
#define PORT 6001
#define buzzer 22
// for NodeMCU 1.0
#define DIN_PIN 15  // D8
#define CS_PIN  2  // D7
#define CLK_PIN 4  // D6

#include "max7219.h"
#include "fonts.h"

AsyncWebServer server(PORT);
StaticJsonDocument<512> doc;


bool interrupt_enable;
char *data;

// =======================================================================
// CHANGE YOUR CONFIG HERE:
// =======================================================================
const char* ssid     = "esw-m19@iiith";     // SSID of local network
const char* password = "e5W-eMai@3!20hOct";   // Password on network
float utcOffset = 5.5; // Time Zone setting
// =======================================================================

String conv_data;
char *c;

void wifi_init(){
  Serial.print("Connecting WiFi ");
  WiFi.begin(ssid, password);
  printStringWithShift("Connecting ",120);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected: "); Serial.println(WiFi.localIP());
}


void setup() 
{
  Serial.begin(115200);
  initMAX7219();
  wifi_init();
  sendCmdAll(CMD_SHUTDOWN,1);
 // sendCmdAll(CMD_INTENSITY,10); // Adjust the brightness between 0 and 15
   server.on(
    "/",
    HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    data_recieve);
      server.begin();

  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);
  Serial.print("MAC ADDRESS : ");
  Serial.println(WiFi.macAddress());
  
}
// =======================================================================
#define MAX_DIGITS 16
byte dig[MAX_DIGITS]={0};
byte digold[MAX_DIGITS]={0};
byte digtrans[MAX_DIGITS]={0};
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
int dx=0;
int dy=0;
byte del=0;
int h,m,s;
long localEpoc = 0;
long localMillisAtUpdate = 0;
const char *m2m_cin_con;

// =======================================================================
void loop()
{
  // Serial.print("IN LOOP");
  if (!interrupt_enable){
    if(updCnt<=0) { // every 10 scrolls, ~450s=7.5m
    updCnt = 10;
    Serial.println("Getting data ...");
    printStringWithShift("  Getting data",15);
   
    getTime();
    Serial.println("Data loaded");
    clkTime = millis();
  }
 
  if(millis()-clkTime > 20000 && !del && dots) { // clock for 15s, then scrolls for about 30s
    printStringWithShift(date.c_str(),40);
   delay(3000);
    updCnt--;
    clkTime = millis();
  }
  if(millis()-dotTime > 500) {
    dotTime = millis();
    dots = !dots;
  }
  updateTime();
  showAnimClock();
    
  // Adjusting LED intensity.
  // 12am to 6am, lowest intensity 0
  if ( (h == 0) || ((h >= 1) && (h <= 6)) ) sendCmdAll(CMD_INTENSITY, 0);
  // 6pm to 11pm, intensity = 2
  else if ( (h >=18) && (h <= 23) ) sendCmdAll(CMD_INTENSITY, 2);
  // max brightness during bright daylight
  else sendCmdAll(CMD_INTENSITY, 10);
  }
  else{
    for(int i=0;i<2;i++)
    {
      Serial.println("working");
      sound_buzzer(4,400);
      clr();
      printStringWithShift(m2m_cin_con,120);
      refreshAll();
      sound_buzzer(4,400);
      delay(1000);
    }
    interrupt_enable = !interrupt_enable;
  }
}
// =======================================================================

void showSimpleClock()
{
  dx=dy=0;
  clr();
  showDigit(h/10,  4, dig7x16);
  showDigit(h%10,  12, dig7x16);
  showDigit(m/10, 21, dig7x16);
  showDigit(m%10, 29, dig7x16);
  showDigit(s/10, 38, dig7x16);
  showDigit(s%10, 46, dig7x16);
  setCol(19,dots ? B00100100 : 0);
  setCol(36,dots ? B00100100 : 0);
  refreshAll();
}

// =======================================================================

void showAnimClock()
{
  
  byte digPos[4]={1,8,17,25};
  int digHt = 12;
  int num = 4; 
  int i;
  if(del==0) {
    del = digHt;
    for(i=0; i<num; i++) digold[i] = dig[i];
    dig[0] = h/10 ? h/10 : 10;
    dig[1] = h%10;
    dig[2] = m/10;
    dig[3] = m%10;
    for(i=0; i<num; i++)  digtrans[i] = (dig[i]==digold[i]) ? 0 : digHt;
  } else
    del--;
  
  clr();
  for(i=0; i<num; i++) {
    if(digtrans[i]==0) {
      dy=0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy = -digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;
    }
  }
  dy=0;
  setCol(15,dots ? B00100100 : 0);
  refreshAll();
 delay(30);
}

// =======================================================================

void showDigit(char ch, int col, const uint8_t *data)
{
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<8*NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;
    }
}

// =======================================================================

void setCol(int col, byte v)
{
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<8*NUM_MAX)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}

// =======================================================================

int showChar(char ch, const uint8_t *data)
{
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

// =======================================================================

void printCharWithShift(unsigned char c, int shiftDelay) {
  Serial.print((char)c);
  
  if (c < ' ' || c > '~'+25) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}

// =======================================================================

void printStringWithShift(const char* s, int shiftDelay){
  Serial.println("IN printStringWithShift Function ================");
  while (*s) {
    // Serial.println("Inside While");
    printCharWithShift(*s, shiftDelay);
    s++;
  }
}

// void printStringWithShift(const String& s, int shiftDelay) {
//   // Serial.println("");
//   for (size_t i = 0; i < s.length(); i++) {
//     printCharWithShift(s.charAt(i), shiftDelay);
//   }
// }

// =======================================================================



void getTime()
{
  WiFiClient client;
  if (!client.connect("www.google.com", 80)) {
    Serial.println("connection to google failed");
    return;
  }

  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") +
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    //Serial.println(".");
    repeatCounter++;
    
  }

  String line;
  client.setNoDelay(false);
  while(client.connected() && client.available()) {
    line = client.readStringUntil('\n');
    line.toUpperCase();
    if (line.startsWith("DATE: ")) {
      date = "     "+line.substring(6, 17);
      h = line.substring(23, 25).toInt();
      m = line.substring(26, 28).toInt();
      s = line.substring(29, 31).toInt();
      localMillisAtUpdate = millis();
      localEpoc = (h * 60 * 60 + m * 60 + s);
      
    }
  }
  client.stop();
}

// =======================================================================

void updateTime()
{
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  long epoch = fmod(round(curEpoch + 3600 * utcOffset + 86400L), 86400L);
  h = ((epoch  % 86400L) / 3600) % 24;
  m = (epoch % 3600) / 60;
  s = epoch % 60;
}

// =======================================================================

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
  m2m_cin_con = doc["data"];
  // String conv_data;
  // conv_data = m2m_cin_con;
  // clr();
  // refreshAll();
  // printStringWithShift(doc["data"],150);


  Serial.print("DATA : ");
  Serial.println(conv_data);
  Serial.println("======== oneM2M DATA PARSED! ==========");
  c = const_cast<char *>(conv_data.c_str());

  interrupt_enable = true;
  
  request->send(200, "text/plain", stri);
}
// ===============================================================================================

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







