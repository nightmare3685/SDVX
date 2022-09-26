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
    NKROKeyboard.add('w');
  } else if (!digitalRead(0) == LOW) {
    NKROKeyboard.remove('w');
  }
   NKROKeyboard.send();
}