#include <FastLED.h>
#include <Control_Surface.h>
#include "Keyboard.h"


USBMIDI_Interface midi;                                 // uncomment jika menggunakan promicro sebagai USB HID MIDI sebagai software controller
                                                        // uncomment line dibawah ini jika controller di fungsikan sebagai hardware controller                                            
//HardwareSerialMIDI_Interface midi {Serial};           // setting awal serial midi untuk promini yang digunakan dengan 5pin midi din sebagai hardware controller
                                                        // ubah "Serial" ke "Serial1" jika menggunakan promicro dengan 5pin midi din sebagai hardware controller

#define NUM_LEDS 10
#define DATA_PIN 9 
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];

CCPotentiometer potentiometers[]= {
  {A3, 11},             // parameter pin potentiometer arduino dan midi CC yang digunakan
  {A2, 74},             // midi CC bisa di ubah sesuai yang di inginkan
  {A1, 73},
  {A0, 72}, 
  {A6, 71}              // A7 pin untuk promini potentiometer ke-5 dan A6 pin untuk promicro potentiometer ke-5  
};

Button buttons[] {
  //13, 10, 3, 8, 5, 12, 11, 2, 7, 6            // array pin dari promini buttons
  15, 10, 3, 8, 5, 14, 16, 2, 7, 6              // array pin dari promicro buttons
};

Button selector = 0;              // Bank selector button
int buttonPushCounter = 0;        // Selector button counter
int buttonState = 0;              // posisi button
int lastButtonState = 0;          // posisi button sebelumnya

template <class T, size_t N> constexpr size_t len(const T (&)[N]) { return N; }

static_assert(len(buttons) == (len(leds)),"midifart");

void setup() {
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); 
  FastLED.setBrightness (100);  
  
  Control_Surface.begin();
  //midi.begin();
  for (auto &button : buttons)
  button.begin();
  Keyboard.begin();
}

void loop() {

  Control_Surface.loop();
  
  buttonState = selector.update() == Button::Falling;           // Update pushbutton selector:

  if (buttonState != lastButtonState) {                         // Pembanding status button dengan status sebelumnya
    if (buttonState == selector.update() == Button::Rising) {
      buttonPushCounter++;
    }
  }

  lastButtonState = buttonState;                                
  if (buttonPushCounter % 4 == 0) {                             // Ganti value "3" ke "2" jika hanya menggunakan 2 bank tanpa sysex mode
    buttonPushCounter = 0;
    }

  switch (buttonPushCounter) {

    case 0 : 
    command1();
    

    break;
    
    case 1 : 
    command2();
    

    break;
    
    case 2 : 
    tonemode();
    
 
    break;

    case 3 : 
    ccmode();
    
 
    break;

    case 4 : 
    sysExmode();
    
 
    break;
  }
}


void ccmode() {
  // ketiga constant parameter dibawah sangat berguna jika digunakan untuk mengirim beberapa command secara bersamaan contohnya MSB dan LSB
  // just change the array value as you need
  
  const uint8_t cc[]    { 12, 13, 14, 15, 16, 17, 18, 19, 28, 29 };
  const uint8_t val[]   { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };    
  //const uint8_t pc[]    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      
      Control_Surface.sendControlChange(cc[i],val[i]);            // Command untuk mengirim midi CC dengan nilai spesifik
      //Control_Surface.sendProgramChange(pc[i]);                 // Kunjungi dokumentasi Control Surface library untuk informasi lebih lanjut
      FastLED.clear();                                 
      leds[i] = CRGB::Red;
      FastLED.show();
      //Serial.println(i); 
    }
  }
}

void tonemode() {
  //const uint8_t cc[]  { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  //const uint8_t val[] { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }; 
  //const uint8_t pc[]  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  const MIDIAddress noteAddress[]  {{MIDI_Notes::C(4), CHANNEL_1},
                                  {MIDI_Notes::D(4), CHANNEL_1},
                                  {MIDI_Notes::E(4), CHANNEL_1},
                                  {MIDI_Notes:: F_(4), CHANNEL_1},
                                  {MIDI_Notes::G(4), CHANNEL_1},
                                  {MIDI_Notes::A(4), CHANNEL_1},
                                  {MIDI_Notes::B(4), CHANNEL_1},
                                  {MIDI_Notes::C(5), CHANNEL_1},
                                  {MIDI_Notes::D(5), CHANNEL_1},
                                  {MIDI_Notes::E(5), CHANNEL_1}};
  const uint8_t onvelocity[]  { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
  //const uint8_t offvelocity[]  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      //Control_Surface.sendControlChange(cc[i],val[i]);
      //Control_Surface.sendProgramChange(pc[i]);
      Control_Surface.sendNoteOn(noteAddress[i], onvelocity[i]);         // send a note dengan spesifik value
      FastLED.clear();
      leds[i] = CRGB::Blue;
      FastLED.show();
      //Serial.println(i); 
    } else if (state == Button::Rising) {
      Control_Surface.sendNoteOff(noteAddress[i], onvelocity[i]); 
      }
  }
}

void sysExmode() {                            // this SysEx message tested with Kurzweil PC3X in Quick Acces mode to replicate numeric keypad
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

void command1() {
  //shortcut key binding
  //copy,     paste,      cut,     snipping tool,      delete
  //undo,     redo,       home,    refresh,            enter

  uint8_t Nokey = 0x00; 
  uint8_t Ctrl = KEY_LEFT_CTRL;
  uint8_t Shift = KEY_LEFT_SHIFT; 
  uint8_t Enter = KEY_RETURN; 
  uint8_t Win = KEY_LEFT_GUI; 
  uint8_t Del = KEY_DELETE;
  uint8_t Space = ' ';
  
  uint8_t command1[]    { Ctrl,     Ctrl,     Ctrl,      Win,       KEY_DELETE,     Ctrl,     Ctrl,     KEY_HOME ,     KEY_F5,     Enter };                       
  uint8_t command2[]    { 'c',      'v',      'x',       Shift,     Nokey,          'z',      'y',      Nokey,         Nokey,      Nokey };
  uint8_t command3[]    { Nokey,    Nokey,    Nokey,     's',       Nokey,          Nokey,    Nokey,    Nokey,         Nokey,      Nokey };

  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {

      Keyboard.press (command1[i]);
      Keyboard.press (command2[i]);
      Keyboard.press (command3[i]);
      Keyboard.releaseAll();
      FastLED.clear();                                 
      leds[i] = CRGB::Purple;
      FastLED.show();
      //Serial.println(i); 
    }
  }
}

void command2() {
  //shortcut key binding
  //copy,   paste,      toggle view,    quantize setting,   quantize
  //undo,   duplicate,  consolidate,    play,               record

  uint8_t Nokey = 0x00; 
  uint8_t Ctrl = KEY_LEFT_CTRL;
  uint8_t Shift = KEY_LEFT_SHIFT; 
  uint8_t Enter = KEY_RETURN; 
  uint8_t Win = KEY_LEFT_GUI; 
  uint8_t Del = KEY_DELETE;
  uint8_t Space = ' ';
   
  uint8_t command1[]    { Ctrl,     Ctrl,     KEY_TAB,      Ctrl,     Ctrl,     Ctrl,     Ctrl,     Ctrl,     Space,        KEY_F9 };                       
  uint8_t command2[]    { 'c',      'v',      Nokey,        Shift,    'u',      'z',      'd',      'j',      Nokey,      Nokey };
  uint8_t command3[]    { Nokey,    Nokey,    Nokey,        'u',      Nokey,    Nokey,    Nokey,    Nokey,    Nokey,      Nokey };

  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {

      Keyboard.press (command1[i]);
      Keyboard.press (command2[i]);
      Keyboard.press (command3[i]);
      Keyboard.releaseAll();
      FastLED.clear();                                 
      leds[i] = CRGB::Cyan;
      FastLED.show();
      //Serial.println(i); 
    }
  }
}
