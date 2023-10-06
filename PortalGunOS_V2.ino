#include <FastLED.h>

// Pins:
#define LED_BAT_LOW 10  // LED indicating low battery
#define LOW_BAT 14      // LOW indicates low battery
#define BUTTON_COL1 5   // HIGH indicates orange button press
#define BUTTON_COL2 7   // HIGH indicates blue button press
#define RGBLED_PIN 2    // Data pin for WS2812B LEDs

#define LED_COUNT 16    // Number of parallel WS2812B LEDs
CRGB leds[LED_COUNT];   // Array representing all WS2812B LEDs

// Global variables:
const int color1[3] = {255, 80, 0};    // Orange (r,g,b)
const int color2[3] = {0, 150, 255};   // Blue   (r,g,b)
int currentColor[] = {color1[0], color1[1], color1[2]};

void setup() {
  // Setup Serial Communication:
  Serial.begin(9600);

  // Setup WS2812B LEDs:
  FastLED.addLeds<WS2812B, RGBLED_PIN, GRB>(leds, LED_COUNT);

  // Setup Pins:
  pinMode(LED_BAT_LOW, OUTPUT);
  pinMode(LOW_BAT, INPUT);
  pinMode(BUTTON_COL1, INPUT_PULLUP);
  pinMode(BUTTON_COL2, INPUT_PULLUP);
}

void loop() {
  // Check which button is pressed:
  bool button1Pressed = (digitalRead(BUTTON_COL1) == LOW);
  bool button2Pressed = (digitalRead(BUTTON_COL2) == LOW);

  // Change WSB2812B color accordingly:
  if(button1Pressed && !button2Pressed){
    currentColor[0] = color1[0];
    currentColor[1] = color1[1];
    currentColor[2] = color1[2];
  }
  else if(!button1Pressed && button2Pressed){
    currentColor[0] = color2[0];
    currentColor[1] = color2[1];
    currentColor[2] = color2[2];
  }
  fill_solid(leds, LED_COUNT, CRGB(currentColor[0], currentColor[1], currentColor[2]));
  FastLED.show();

  //Battery Charge LED logic:
  if(digitalRead(LOW_BAT) == LOW){
    analogWrite(LED_BAT_LOW, 64);
  }
  else{
    analogWrite(LED_BAT_LOW, 0);
  }

  // Serial Debug comms:
  Serial.print("Btn1: ");
  Serial.print(button1Pressed);
  Serial.print("\tBtn2: ");
  Serial.println(button2Pressed);
}
