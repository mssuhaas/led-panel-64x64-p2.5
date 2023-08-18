
#include <PxMatrix.h>



#define buzzer 0

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 16
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define matrix_width 64
#define matrix_height 64

bool data_status = true;
String data_string = "C++ is not fun";

uint8_t display_draw_time = 60;  //30-70 is usually fine

PxMATRIX display(matrix_width, matrix_height, P_LAT, P_OE, P_A, P_B, P_C);

uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8] = { myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK };



void IRAM_ATTR display_updater() {
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable) {
  if (is_enable) {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  } else {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

unsigned long last_draw = 0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB) {
  uint16_t text_length = text.length();
  display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
  display.setTextSize(1);
  display.setRotation(0);
  display.setTextColor(display.color565(colorR, colorG, colorB));

  // Asuming 5 pixel average character width
  for (int xpos = matrix_width; xpos > -(matrix_width + text_length * 6); xpos--) {
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



void setup() {
  // wifi_init();
  Serial.begin(115200);
  Serial.println("Started!");
  pinMode(buzzer, OUTPUT);
  display_update_enable(true);
  display.begin(8);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("LOOP STARTS");
  // scroll_text(1, 50, "AREY ENTRA IDHI!", 96, 96, 250);
  // display.clearDisplay();
  // delay(1000);
  if (data_status) {
    for (int j = 0; j < 2; j++) {
      scroll_text(1, 50, data_string, 96, 96, 250);
      sound_buzzer(2,150);
      delay(700);
    }
    data_status = false;
  } else {
    scroll_text(1, 50, "Welcome to Smart City Living Lab!", 96, 96, 250);
    display.clearDisplay();
  }
}

void sound_buzzer(uint8_t times, uint8_t delay_time) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzer, HIGH);
    delay(delay_time);
  }
}
