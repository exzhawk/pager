#include <HID.h>         //include the basic HID infrastructure first
#include <Mouse.h>
#include <Keyboard.h>
#include <TinyUSB_Mouse_and_Keyboard.h>

const int pinA = 9;
const int pinB = 10;
volatile int16_t pos = 0;
boolean rotating = false;    // debounce management
volatile int16_t lastpos = 0;

void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), intA, CHANGE);
  Keyboard.begin();
}

void intA() {
  if ( rotating ) {
    delayMicroseconds(5000);
  }
  if ( digitalRead(pinA) == digitalRead(pinB)) {
    pos++;
  } else {
    pos--;
  }
  rotating = false;
}

void loop() {
  rotating = true;
  if (lastpos != pos) {
    if (lastpos > pos) {
      Keyboard.write(KEY_UP_ARROW);
    } else {
      Keyboard.write(KEY_DOWN_ARROW);
    }
    lastpos = pos;
  }
}
