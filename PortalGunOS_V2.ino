#include <FastLED.h>

/* The following code is ready for upload without any changes necessary.
   You can however tweak various things via the '#define' values below and
   the two 'const int color1[3] = {...}' values under 'Global variables'
   if you like.

   This way you can easily change the color and flickering behaviour of
   the LEDs.
  */

// Pins:
#define LED_BAT_RED 10        // Pin to red RGB LED (Charge Indicator)
#define LED_BAT_GREEN 11      // Pin to green RGB LED (Charge Indicator)
#define BAT_VOLTAGE 14        // Analog Pin reading the battery voltage
#define BUTTON_COL1 5         // HIGH indicates orange button press
#define BUTTON_COL2 7         // HIGH indicates blue button press
#define RGBLED_PIN 2          // Data pin for WS2812B LEDs

// LEDs:
#define LED_COUNT 16          // Number of connected WS2812B LEDs
CRGB ledArray[LED_COUNT];     // Array representing all WS2812B LEDs
const int color1[3] = {255, 80, 0};    // Orange (r,g,b)
const int color2[3] = {0, 150, 255};   // Blue   (r,g,b)
int currentColor[3] = {0, 0, 0};       // Initial color before any button is pressed (r,g,b)

// Settings for flickering LEDs (via brownian noise). Values in range [0, 255]:
#define ENABLE_FLICKERING 1
#define BROWNIAN_STEP 5       // How sudden the brightness can change.
#define MIN_BRIGHTNESS 150    // The flickering brightness will move
#define MAX_BRIGHTNESS 230    // between these two values.
#define BROWNIAN_START (MAX_BRIGHTNESS + MIN_BRIGHTNESS) / 2
float brownian = BROWNIAN_START;
float brightness = 1.0;       // Don't change this if flickering is enabled

// Enable Serial debug communication:
#define ENABLE_DEBUG_COMMS 0

// Set this to 0 if you are only using a single (red) LED for the charge indicator on pin 'LED_BAT_RED'
#define SET_CHARGE_INDICATOR_RGB 1

// Other variables:
unsigned long previousMillis = 0;

// Variables for smoothing battery voltage reading:
const int batNumReadings = 20;
const int loopSkipCount = 50;
int batteryReadings[batNumReadings];
int batteryReadIndex = 0;
int batteryTotal = 0;
int batteryAverage = 0;
int loopCounter = 0;

void setup() {
  // Setup Serial Communication:
  Serial.begin(9600);

  // Setup WS2812B LEDs:
  FastLED.addLeds<WS2812B, RGBLED_PIN, GRB>(ledArray, LED_COUNT);

  // Setup Pins:
  pinMode(LED_BAT_RED, OUTPUT);
  #if SET_CHARGE_INDICATOR_RGB
  pinMode(LED_BAT_GREEN, OUTPUT);
  #endif

  pinMode(BAT_VOLTAGE, INPUT);
  pinMode(BUTTON_COL1, INPUT_PULLUP);
  pinMode(BUTTON_COL2, INPUT_PULLUP);

  // Fill batteryReadings[] with zeros:
  for(int i = 0; i < batNumReadings; i++){
    batteryReadings[i] = 0;
  }

}

void loop() {
  // Check which button is pressed:
  bool button1Pressed = (digitalRead(BUTTON_COL1) == LOW);
  bool button2Pressed = (digitalRead(BUTTON_COL2) == LOW);

  #if ENABLE_DEBUG_COMMS        /* Shows which buttons are pressed: */
  Serial.print(button1Pressed);
  Serial.print(" ");
  Serial.print(button2Pressed);
  Serial.print(" ");
  #endif

  // Change WSB2812B color accordingly:
  if(button1Pressed && !button2Pressed){
    currentColor[0] = color1[0];
    currentColor[1] = color1[1];
    currentColor[2] = color1[2];
    #if ENABLE_FLICKERING
    brightness = 1.0;
    #endif
  }
  else if(!button1Pressed && button2Pressed){
    currentColor[0] = color2[0];
    currentColor[1] = color2[1];
    currentColor[2] = color2[2];
    #if ENABLE_FLICKERING
    brightness = 1.0;
    #endif
  }

  #if ENABLE_FLICKERING
  else{
    // Randomly change LED brightness via brownian noise:
    brownian += float(random(-BROWNIAN_STEP + 1, BROWNIAN_STEP));

    // Calculate resulting brightness: 
    brownian = constrain(brownian, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    brightness = brownian / 255.0;
  }
  #if ENABLE_DEBUG_COMMS        /* Shows brightness: */
  Serial.print(brightness);
  Serial.print(" ");
  #endif
  #endif

  // Update LEDs:
  fill_solid(ledArray, LED_COUNT, CRGB(currentColor[0] * brightness, currentColor[1] * brightness, currentColor[2] * brightness));
  FastLED.show();

  // Smoothen battery readings:
  if(loopCounter >= loopSkipCount){
    loopCounter = 0;
    batteryTotal -= batteryReadings[batteryReadIndex];
    batteryReadings[batteryReadIndex] = analogRead(BAT_VOLTAGE);
    batteryTotal += batteryReadings[batteryReadIndex];
    batteryReadIndex++;
    batteryAverage = float(batteryTotal) / batNumReadings;

    if(batteryReadIndex >= batNumReadings){
      batteryReadIndex = 0;
    }
  }


  #if SET_CHARGE_INDICATOR_RGB
  /* Standard charge indicator logic for an RGB led (only using red and green)
  Charge Percentage:
            5.0V ^= 1023
    100% ^= 4.2V ^= 859
     85% ^= 4.1V ^= 822
      0% ^= 3.0V ^= 614

    > 90% green   (839)
    > 20% yellow  (663) blinking
    < 20% red     (663) blinking
  */

  const int batBrightnessRed = 64;
  const int batBrightnessGreen = 16;
  
  if(batteryAverage >= 822){
    analogWrite(LED_BAT_GREEN, batBrightnessGreen);
    analogWrite(LED_BAT_RED, 0);
    }
    else if (batteryAverage >= 663) {
      unsigned long timeSinceLastLEDUpdate = millis() - previousMillis;
      static bool ledIsOn = false;
      if (ledIsOn && (timeSinceLastLEDUpdate >= 100)) {
        analogWrite(LED_BAT_RED, 0);
        analogWrite(LED_BAT_GREEN, 0);
        previousMillis = millis();
        ledIsOn = false;
      } else if (!ledIsOn && (timeSinceLastLEDUpdate >= 4000)) {
        analogWrite(LED_BAT_RED, batBrightnessRed);
        analogWrite(LED_BAT_GREEN, batBrightnessGreen);
        previousMillis = millis();
        ledIsOn = true;
      }
    }
    else {
      unsigned long timeSinceLastLEDUpdate = millis() - previousMillis;
      static bool ledIsOn = false;
      if (ledIsOn && (timeSinceLastLEDUpdate >= 1000)) {
        analogWrite(LED_BAT_RED, 0);
        previousMillis = millis();
        ledIsOn = false;
      } else if (!ledIsOn && (timeSinceLastLEDUpdate >= 1000)) {
        analogWrite(LED_BAT_RED, batBrightnessRed);
        previousMillis = millis();
        ledIsOn = true;
      }
      analogWrite(LED_BAT_GREEN, 0);
  }

  

  #endif
  #if (not SET_CHARGE_INDICATOR_RGB)

  /* Charge Logic if only a single red LED is used:
  Charge Percentage:
          5.0V ^= 1023
  100% ^= 4.2V ^= 859
    0% ^= 3.0V ^= 614

    > 85% (822) always on   
    > 20% (663) blinking occasionally
    < 20% (663) blinking fast
  */

  const int batBrightnessRed = 64;
  unsigned long timeSinceLastLEDUpdate = millis() - previousMillis;
  int batteryVoltage = batteryAverage;

  if(batteryAverage >= 822){
      analogWrite(LED_BAT_RED, batBrightnessRed);
    }
  else if (batteryAverage >= 663){
    unsigned long timeSinceLastLEDUpdate = millis() - previousMillis;
    static bool ledIsOn = false;
    if(ledIsOn && (timeSinceLastLEDUpdate >= 100)){
      analogWrite(LED_BAT_RED, 0);
      previousMillis = millis();
      ledIsOn = false;
    }
    else if (!ledIsOn && (timeSinceLastLEDUpdate >= 4000)){
      analogWrite(LED_BAT_RED, batBrightnessRed);
      previousMillis = millis();
      ledIsOn = true;
    }
  }
  else{
    unsigned long timeSinceLastLEDUpdate = millis() - previousMillis;
    static bool ledIsOn = false;
    if(ledIsOn && (timeSinceLastLEDUpdate >= 1000)){
      analogWrite(LED_BAT_RED, 0);
      previousMillis = millis();
      ledIsOn = false;
    }
    else if (!ledIsOn && (timeSinceLastLEDUpdate >= 1000)){
      analogWrite(LED_BAT_RED, batBrightnessRed);
      previousMillis = millis();
      ledIsOn = true;
    }
  }
  #endif

  loopCounter++;

  #if ENABLE_DEBUG_COMMS
  float batteryPercentage = 1.0 + 0.004082 * float(batteryAverage - 859);
  Serial.print(batteryPercentage);
  Serial.println(1); // Shows a reference line in the Serial Plotter:
  #endif
}
