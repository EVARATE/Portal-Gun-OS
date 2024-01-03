#include <FastLED.h>

/* The following code is ready for upload without any changes necessary.
   You can however tweak various things via the '#define' values below and
   the two 'const int color1[3] = {...}' values under 'Global variables'
   if you like.

   This way you can easily change the color and flickering behaviour of
   the LEDs.
  */


// Pins:
#define LED_BAT_LOW 10        // LED indicating low battery
#define LOW_BAT 14            // LOW indicates low battery
#define BUTTON_COL1 5         // HIGH indicates orange button press
#define BUTTON_COL2 7         // HIGH indicates blue button press
#define RGBLED_PIN 2          // Data pin for WS2812B LEDs

// LEDs:
#define LED_COUNT 16          // Number of parallel WS2812B LEDs
CRGB leds[LED_COUNT];         // Array representing all WS2812B LEDs

// Enable Serial debug communication:
#define ENABLE_DEBUG_COMMS 0
/* Will send data about LED brightness and buttons being pushed
   back to the Arduino IDE.
   Set this to 0 for the final upload since serial communication
   affects the speed of your code and flickering                  */

// Settings for flickering LEDs (via brownian noise). Values in range [0, 255]:
#define ENABLE_FLICKERING 1
#define BROWNIAN_STEP 4       // How sudden the brightness can change.
#define MIN_BRIGHTNESS 100    // The flickering brightness will move
#define MAX_BRIGHTNESS 230    // between these two values.
#define BROWNIAN_START (MAX_BRIGHTNESS + MIN_BRIGHTNESS) / 2

// Global variables:
const int color1[3] = {255, 80, 0};    // Orange (r,g,b)
const int color2[3] = {0, 150, 255};   // Blue   (r,g,b)
int currentColor[] = {color1[0], color1[1], color1[2]};

float brownian = BROWNIAN_START;
float brightness = 1.0;       // Don't change this if flickering is enabled

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

    /* Calculate resulting brightness: */
    brownian = constrain(brownian, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    brightness = brownian / 255.0;
  }
  #if ENABLE_DEBUG_COMMS        /* Shows brightness and if brownian gets out of range: */
  Serial.print(brightness);
  Serial.print(" ");
  #endif
  #endif

  // Update LEDs:
  fill_solid(leds, LED_COUNT, CRGB(currentColor[0] * brightness, currentColor[1] * brightness, currentColor[2] * brightness));
  FastLED.show();

  //Battery Charge LED logic:
  if(digitalRead(LOW_BAT) == LOW){
    analogWrite(LED_BAT_LOW, 64);
  }
  else{
    analogWrite(LED_BAT_LOW, 0);
  }

  #if ENABLE_DEBUG_COMMS        /* Shows a reference line in the Serial Plotter: */
  Serial.println(1);
  #endif
}
