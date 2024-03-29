#include <FastLED.h>
#include <Control_Surface.h>


USBMIDI_Interface midi;                                 // uncomment jika menggunakan promicro sebagai USB HID MIDI sebagai software controller
                                                        // uncomment line dibawah ini jika controller di fungsikan sebagai hardware controller                                            
//HardwareSerialMIDI_Interface midi {Serial};           // setting awal serial midi untuk promini yang digunakan dengan 5pin midi din sebagai hardware controller
                                                        // ubah "Serial" ke "Serial1" jika menggunakan promicro dengan 5pin midi din sebagai hardware controller

#define NUM_LEDS 10
#define DATA_PIN 9 
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];

CCPotentiometer potentiometers[]= {
  {A3, 75},             // parameter pin potentiometer arduino dan midi CC yang digunakan
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
  FastLED.setBrightness (40); 
  FastLED.clear();  
  delay(50);
  fill_solid(leds, NUM_LEDS, CRGB::Aqua);
  FastLED.show();
 
  
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
  if (buttonPushCounter % 4 == 0) {                             
    buttonPushCounter = 0;
    }

  switch (buttonPushCounter) {

    case 0 : 
    tonemode1();
    FastLED.setBrightness (20); 
    fill_solid(leds, NUM_LEDS, CRGB::Aqua);
    FastLED.show();    
    

    break;
    
    case 1 : 
    tonemode2();
    FastLED.setBrightness (40); 
    fill_solid(leds, NUM_LEDS, CRGB::Olive);
    FastLED.show();    

    break;
    
    case 2 : 
    tonemode3();
    FastLED.setBrightness (30); 
    fill_solid(leds, NUM_LEDS, CRGB::Crimson);
    FastLED.show();    
 
    break;

    case 3 : 
    tonemode4();
    FastLED.setBrightness (30); 
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();    
 
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
    }
  }
}

void tonemode1() {
  //const uint8_t cc[]  { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  //const uint8_t val[] { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }; 
  //const uint8_t pc[]  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  const MIDIAddress noteAddress[]  {{MIDI_Notes::C(3), CHANNEL_1},
                                  {MIDI_Notes::Db(3), CHANNEL_1},
                                  {MIDI_Notes::D(3), CHANNEL_1},
                                  {MIDI_Notes::Eb(3), CHANNEL_1},
                                  {MIDI_Notes::E(3), CHANNEL_1},
                                  {MIDI_Notes::F_(3), CHANNEL_1},
                                  {MIDI_Notes::Gb(3), CHANNEL_1},
                                  {MIDI_Notes::G(3), CHANNEL_1},
                                  {MIDI_Notes::Ab(3), CHANNEL_1},
                                  {MIDI_Notes::A(3), CHANNEL_1}};
  const uint8_t onvelocity[]  { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
  //const uint8_t offvelocity[]  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      //Control_Surface.sendControlChange(cc[i],val[i]);
      //Control_Surface.sendProgramChange(pc[i]);
      Control_Surface.sendNoteOn(noteAddress[i], onvelocity[i]);         // send a note dengan spesifik value
    } else if (state == Button::Rising) {
      Control_Surface.sendNoteOff(noteAddress[i], onvelocity[i]); 
      }
  }
}

void tonemode2() {
  //const uint8_t cc[]  { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  //const uint8_t val[] { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }; 
  //const uint8_t pc[]  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  const MIDIAddress noteAddress[] {{MIDI_Notes::Bb(3), CHANNEL_1},
                                  {MIDI_Notes::B(3), CHANNEL_1},
                                  {MIDI_Notes::C(4), CHANNEL_1},
                                  {MIDI_Notes::Db(4), CHANNEL_1},
                                  {MIDI_Notes::D(4), CHANNEL_1},
                                  {MIDI_Notes::Eb(4), CHANNEL_1},
                                  {MIDI_Notes::E(4), CHANNEL_1},
                                  {MIDI_Notes::F_(4), CHANNEL_1},
                                  {MIDI_Notes::Gb(4), CHANNEL_1},
                                  {MIDI_Notes::G(4), CHANNEL_1}};
  const uint8_t onvelocity[]  { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
  //const uint8_t offvelocity[]  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      //Control_Surface.sendControlChange(cc[i],val[i]);
      //Control_Surface.sendProgramChange(pc[i]);
      Control_Surface.sendNoteOn(noteAddress[i], onvelocity[i]);         // send a note dengan spesifik value
    } else if (state == Button::Rising) {
      Control_Surface.sendNoteOff(noteAddress[i], onvelocity[i]); 
      }
  }
}

void tonemode3() {
  //const uint8_t cc[]  { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  //const uint8_t val[] { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }; 
  //const uint8_t pc[]  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  const MIDIAddress noteAddress[]  {{MIDI_Notes::Ab(4), CHANNEL_1},
                                  {MIDI_Notes::A(4), CHANNEL_1},
                                  {MIDI_Notes::Bb(4), CHANNEL_1},
                                  {MIDI_Notes::B(4), CHANNEL_1},
                                  {MIDI_Notes::C(5), CHANNEL_1},
                                  {MIDI_Notes::Db(5), CHANNEL_1},
                                  {MIDI_Notes::D(5), CHANNEL_1},
                                  {MIDI_Notes::Eb(5), CHANNEL_1},
                                  {MIDI_Notes::E(5), CHANNEL_1},
                                  {MIDI_Notes::F_(5), CHANNEL_1}};
  const uint8_t onvelocity[]  { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
  //const uint8_t offvelocity[]  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      //Control_Surface.sendControlChange(cc[i],val[i]);
      //Control_Surface.sendProgramChange(pc[i]);
      Control_Surface.sendNoteOn(noteAddress[i], onvelocity[i]);         // send a note dengan spesifik value 
    } else if (state == Button::Rising) {
      Control_Surface.sendNoteOff(noteAddress[i], onvelocity[i]); 
      }
  }
}

void tonemode4() {
  //const uint8_t cc[]  { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
  //const uint8_t val[] { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }; 
  //const uint8_t pc[]  { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
  const MIDIAddress noteAddress[]  {{MIDI_Notes::Gb(5), CHANNEL_1},
                                  {MIDI_Notes::G(5), CHANNEL_1},
                                  {MIDI_Notes::Ab(5), CHANNEL_1},
                                  {MIDI_Notes::A(5), CHANNEL_1},
                                  {MIDI_Notes::Bb(5), CHANNEL_1},
                                  {MIDI_Notes::B(5), CHANNEL_1},
                                  {MIDI_Notes::C(6), CHANNEL_1},
                                  {MIDI_Notes::Db(6), CHANNEL_1},
                                  {MIDI_Notes::D(6), CHANNEL_1},
                                  {MIDI_Notes::Eb(6), CHANNEL_1}};
  const uint8_t onvelocity[]  { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
  //const uint8_t offvelocity[]  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
  for (uint8_t i = 0; i < len(buttons); ++i) {
    auto state = buttons[i].update();
    if (state == Button::Falling) {
      //Control_Surface.sendControlChange(cc[i],val[i]);
      //Control_Surface.sendProgramChange(pc[i]);
      Control_Surface.sendNoteOn(noteAddress[i], onvelocity[i]);         // send a note dengan spesifik value 
    } else if (state == Button::Rising) {
      Control_Surface.sendNoteOff(noteAddress[i], onvelocity[i]); 
      }
  }
}
