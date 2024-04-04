#include <Arduino.h> // Arduino library
#include <RTClib.h> // Real Time Clock library
#include <Adafruit_GFX.h> // Display Master/General
#include <Adafruit_SSD1306.h> // OLED Display Specific
#include <string.h>

#define LED 12 // Alarm LED
#define piezo 9 // Alarm Speaker
#define snooze 7 // Snooze button on pin 7 (Turns off alarm sound and LED)
bool run = true; // bool to help snooze button turn off alarm

uint8_t alarmH = 15, alarmM = 15, alarmS = 0; // Wake up time 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin #

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

RTC_DS3231 rtc;

void setup()
{

  pinMode(LED, OUTPUT);
  pinMode(piezo, OUTPUT);
  pinMode(snooze, INPUT_PULLUP);

  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  

}
 
void loop()
{ 
  DateTime now = rtc.now();
  
  delay(1000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display time and date
  String date = "      " + String(now.hour()) + ":" +
               String(now.minute()) + " : " +
               String(now.second()) + "\n" + "\n" + "   " +
               "Alarm " + String(alarmH) + ":" + String(alarmM) + "\n" +  "   " + "Day / Month "
               + "\n" + "     " +  String(now.day()) + " / " + 
               String(now.month()) + "\n" + "      " +
               String(now.year());
  display.println(date);
  display.display();

if (now.hour() == alarmH && now.minute() == alarmM /*&& now.second() == alarmS*/) // Commented away "second" for longer alarm loop
{
  while(run)
  {
  if (digitalRead(snooze) == LOW) {
    run = false;
    String morning = " " + String("Snooze");
    display.println(morning);
    display.display();
  }
  else
  {
  digitalWrite(LED, HIGH);
  tone(piezo, 880, 500); // output pin, tone frequenzy, duration
  digitalWrite(LED, LOW);
  delay(400);
  digitalWrite(LED, HIGH);
  tone(piezo, 698, 500); // output pin, tone frequenzy, duration
  digitalWrite(LED, LOW);
  delay(400);
    }
  }
}
}