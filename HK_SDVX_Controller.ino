#include <HID-Project.h>
#include <HID-Settings.h>



void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
}

void loop() {
  keyfunc();
}


void keyfunc() {

  Serial.println(!digitalRead(0));

  if (!digitalRead(0) == HIGH) {
    NKROKeyboard.add('s');
    NKROKeyboard.add('d');
    NKROKeyboard.add('k');
    NKROKeyboard.add('l');
    NKROKeyboard.add('c');
    NKROKeyboard.add('m');
  } else if (!digitalRead(0) == LOW) {
    NKROKeyboard.remove('s');
    NKROKeyboard.remove('d');
    NKROKeyboard.remove('k');
    NKROKeyboard.remove('l');
    NKROKeyboard.remove('c');
    NKROKeyboard.remove('m');
  }
  NKROKeyboard.send();
}