#include <FastLED.h>
//#include <NewPing.h>              //uncomment this line if use ultrasonic sensor
#include <Control_Surface.h>


//USBMIDI_Interface midi;                               // uncomment this line if use promicro as USB HID MIDI device as software controller
                                                        // and comment the line below                                                       
HardwareSerialMIDI_Interface midi {Serial};             // default serial midi for promini with 5pin midi din as hardware controller
                                                        // change "Serial" to "Serial1" if you using promicro with 5pin midi din as hardware controller
                                                      
//uncomment all of six line below if you using ultrasonic sensor (only works with promini)                                                     
//#define TRIGGER_PIN  18           // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN     19           // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define MAX_DISTANCE 30           // Maximum distance we want to ping, change this value if necessary (in centimeters)
//int midiMapped = 0;          
//int lastmidiMapped = 0;
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);           // NewPing setup of pins and maximum distance.

#define NUM_LEDS 10
#define DATA_PIN 9 
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];

int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000; 

CCPotentiometer potentiometers[]= {
  {A3, 11},    // pin address and CC address parameter
  {A2, 74},    // CC address parameter can be edited if you need specific CC address
  {A1, 73},
  {A0, 75}, 
  {A7, 5}      // A7 pin for promini fifth pot and A6 pin for promicro fifth pot
};

Button buttons[] {
  13, 10, 3, 8, 5, 12, 11, 2, 7, 6          // array of promini buttons
  //15, 10, 3, 8, 5, 14, 16, 2, 7, 6        // array of promicro buttons
};

Button selector = 0;         // Bank selector button
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

template <class T, size_t N> constexpr size_t len(const T (&)[N]) { return N; }

static_assert(len(buttons) == (len(leds)),"midifart");

void setup() {
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); 
  FastLED.setBrightness (100);  

  pinMode(18, OUTPUT);      // bank led indicator (promini only)
  pinMode(19, OUTPUT);
  
  Control_Surface.begin();
  midi.begin();
  for (auto &button : buttons)
  button.begin();
}

uint8_t gHue = 0; // rotating "base color" used synelon leds animation

void loop() {

  Control_Surface.loop();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  
  unsigned long currentMillis = millis(); 
  buttonState = selector.update() == Button::Falling;        // read the pushbutton selector:

  if (buttonState != lastButtonState) {                      //compare the buttonState to its previous state
    if (buttonState == selector.update() == Button::Rising) {
      buttonPushCounter++;
    }
  }

  lastButtonState = buttonState;                             // save the current state as the last state

  if (buttonPushCounter % 3 == 0) {                          // change value "3" to "2" if you just need two bank without sysex mode
    buttonPushCounter = 0;
    }

  switch (buttonPushCounter) {

    case 0 : 
    bank1();
    
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(18, ledState);
    digitalWrite(19, LOW);
  } 
    break;
    
    case 1 : 
    bank2();
    
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(19, ledState);
    digitalWrite(18, LOW);
  }
    break;
    
    case 2 : 
    sysExmode();
    
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(18, ledState);
    digitalWrite(19, ledState);
  }   
    break;
  }
}
    
  //uncomment all of seven line below if you using ultrasonic sensor (only works with promini)
  //float x  = sonar.ping_cm();                          //sensor read
  //midiMapped = map(x, 5, 28, 127, 0);                  //sensor map 5-28 cm converted to cc value 0-127 or 127-0
  //if (lastmidiMapped != midiMapped) {
  //       if (midiMapped >= 0 && midiMapped <= 127) 
  //       midi.sendControlChange(42, midiMapped);        
  //       lastmidiMapped = midiMapped;                  //save last cc value
  //}


void bank1() {
  // three constant parameter below very convenient if you need specific sound between bank to bank or working with MSB and LSB
  // just change the array value as you need
  
  const uint8_t cc[]    { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  const uint8_t val[]   { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };    
  const uint8_t pc[]    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      midi.sendControlChange(cc[i],val[i]);       // this command for sending CC folowed by Program Change
      midi.sendProgramChange(pc[i]);              // there are lot midi command like sending Pitcbend value and other 
      FastLED.clear();                            // just see Control Surface library documentation
      leds[i] = CRGB::Red;
      FastLED.show();
      //Serial.println(i); 
    }
  }
}

void bank2() {
  const uint8_t cc[]  { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  const uint8_t val[] { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }; 
  const uint8_t pc[]  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      midi.sendControlChange(cc[i],val[i]);
      midi.sendProgramChange(pc[i]);
      FastLED.clear();
      leds[i] = CRGB::Blue;
      FastLED.show();
      //Serial.println(i); 
    }
  }
}

void sysExmode() {                  // this SysEx message tested with Kurzweil PC3X in Quick Acces mode to replicate numeric keypad
                                    // please note this SysEx message does not work with other hardware
                                    // you need implement your own sysex message 
                                     
  uint8_t sysex[]                   {0xF0, 0x07, 0x40, 0x00, 0x10, 0xFF, 0xFF, 0xF7};
  const uint8_t sysex_values[]      {0x1F, 0x1E, 0x1D, 0x27, 0x26, 0x25, 0x2F, 0x2E, 0x2D, 0x36};
  const uint8_t sysex_start_value = 0x70;
  const uint8_t sysex_end_value   = 0x72;
  
  // Send a two SysEx message when the push button is pressed and released
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling || state == Button::Rising) {
      sysex[5] = state == Button::Falling ? sysex_start_value
                                          : sysex_end_value;
      sysex[6] = sysex_values[i];
      Control_Surface.send(sysex);
      FastLED.clear();
      leds[i] = CRGB::Green;
      FastLED.show();
    }
  }
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails p
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 100);
  FastLED.show();
}
