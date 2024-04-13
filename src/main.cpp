#include <Arduino.h>

#define DEBUG 1

#define LED_COUNT 30
#define LED_PIN 26

char ssid[] = "TeamSpirit - 2.4G";
char pass[] = "embedded206";

#define BLYNK_TEMPLATE_ID "TMPL3uNxY-Zg7"
#define BLYNK_TEMPLATE_NAME "EMBEDDED 206 V2"
#define BLYNK_AUTH_TOKEN "6T0qo7V8IPfWbAtLTYnCGi7JRoFTXBk4"

#define BLYNK_PRINT Serial

uint8_t brightness = 100;
uint16_t speed = 10;
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
uint8_t mode = 0;

unsigned long prev_millis = 0; // vairable to save the last executed time

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

BLYNK_WRITE(V0) { brightness = param.asInt(); }
BLYNK_WRITE(V1) { red = param.asInt(); }
BLYNK_WRITE(V2) { green = param.asInt(); }
BLYNK_WRITE(V3) { blue = param.asInt(); }
BLYNK_WRITE(V5) { speed = param.asInt(); }
BLYNK_WRITE(V4)
{
  int pinVal = param.asInt();
  if (pinVal == 1 && mode == 0)
  {
    mode = 1;
  }
  else if (pinVal == 1 && mode == 1)
  {
    mode = 2;
  }
  else if (pinVal == 1 && mode == 2)
  {
    mode = 0;
  }
  if (DEBUG)
  {
    Serial.println("Mode : " + String(mode));
  }
}

void clear(){
  strip.clear(); // Set all pixel colors to 'off'
  strip.show();
}
void rainbow()
{
  unsigned long currentMillis = millis(); // Get the current time

  static long firstPixelHue = 0; // Declare firstPixelHue as static to retain its value between function calls
  strip.setBrightness(brightness);
  if (currentMillis - prev_millis >= speed)
  {                              // Check if wait milliseconds have passed
    prev_millis = currentMillis; // Save the last time a new color was displayed

    for (int i = 0; i < strip.numPixels(); i++)
    {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }

    strip.show();

    firstPixelHue += 256; // Increment the hue for the next iteration
    if (firstPixelHue >= 5 * 65536)
    {
      firstPixelHue = 0; // Restart the rainbow cycle when it reaches the end
    }
  }
}

void solid()
{
  for (int i = 0; i < LED_COUNT; i++)
  { // For each pixel...
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show(); // Send the updated pixel colors to the hardware.
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  strip.begin();
  strip.show();
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  if (mode == 0){solid();}
  else if (mode == 1){rainbow();} 
  else if (mode ==2){clear();}
}
