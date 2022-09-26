#include <HID-Project.h>
#include <HID-Settings.h>

//char keymap[100] = {  's',  'd',  'k',  'l',  'c',  'm',};
char BT_A = 's', BT_B = 'd', BT_C = 'k', BT_D = 'l', FX_L = 'c', FX_R = 'm';


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
    NKROKeyboard.add(BT_A);
    NKROKeyboard.add(BT_B);
    NKROKeyboard.add(BT_C);
    NKROKeyboard.add(BT_D);
    NKROKeyboard.add(FX_L);
    NKROKeyboard.add(FX_R);
  } else if (!digitalRead(0) == LOW) {
    NKROKeyboard.remove(BT_A);
    NKROKeyboard.remove(BT_B);
    NKROKeyboard.remove(BT_C);
    NKROKeyboard.remove(BT_D);
    NKROKeyboard.remove(FX_L);
    NKROKeyboard.remove(FX_R);
  }
  NKROKeyboard.send();
}