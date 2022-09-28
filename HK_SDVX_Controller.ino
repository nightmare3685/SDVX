#include <HID-Project.h>
#include <HID-Settings.h>
#include <FlexiTimer2.h>


class Button {
public:
  char keymap[100] = { 's', 'd', 'k', 'l', 'c', 'm' /*6:Enter 7:ESC*/ };
  char BT_A = keymap[0], BT_B = keymap[1], BT_C = keymap[2], BT_D = keymap[3], FX_L = keymap[4], FX_R = keymap[5];
  char VOL_L = 0, VOL_R = 1;
private:
  ;
};

volatile byte pos;
volatile byte pos2[2];
volatile int right = 0, left = 0;
volatile int right2 = 0, left2 = 0;
volatile int Arrayright[2] = { 0, 0 };
volatile int Arrayleft[2] = { 0, 0 };
Button button;

void setup() {
  FlexiTimer2::set(5, 1.0 / 1000000, TimerIRQ);  //タイマー割り込み 引数1:時間 引数2:時間の単位 引数3:割り込む関数
  FlexiTimer2::start();                          //タイマ割込み実行
  Serial.begin(9600);
  for (int i = 0; i <= 13; i++) {  //13pinすべてアップ
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  keyFunc();
  ReduseValue();
}
void ReduseValue() {
  for (int i = 0; i < (sizeof(Arrayleft) / sizeof(int)); i++) {

    Arrayright[i]--;
    Arrayleft[i]--;
    if (Arrayright[i] <= 0) {
      Arrayright[i] = 0;
    }
    if (Arrayleft[i] <= 0) {
      Arrayleft[i] = 0;
    }
  }
}

void keyFunc() {

  for (int i = 0; i < 6; i++) {
    if (!digitalRead(i) == HIGH) {
      NKROKeyboard.add(button.keymap[i]);
    } else if (!digitalRead(i) == LOW) {
      NKROKeyboard.remove(button.keymap[i]);
    }
  }

  if (Arrayright[button.VOL_L] > 0) {  //左のつまみが右回転
    NKROKeyboard.add('w');
  } else {
    NKROKeyboard.remove('w');
  }
  if (Arrayleft[button.VOL_L] > 0) {
    NKROKeyboard.add('q');  //左のつまみが左回転
  } else {
    NKROKeyboard.remove('q');
  }


  if (Arrayright[button.VOL_R] > 0) {  //右のつまみが右回転
    NKROKeyboard.add('p');
  } else {
    NKROKeyboard.remove('p');
  }

  if (Arrayleft[button.VOL_R] > 0) {  //右のつまみが左回転
    NKROKeyboard.add('o');
  } else {
    NKROKeyboard.remove('o');
  }

  NKROKeyboard.send();
}



void TimerIRQ() {
  byte currentWave[2];  //最新のロータリー信号
  byte oldWave[2];      //ひとつ前の信号


  currentWave[button.VOL_L] = (!digitalRead(10) << 1) + !digitalRead(11);  //A相とB相の信号を 000000ABとして変数に代入
  currentWave[button.VOL_R] = (!digitalRead(12) << 1) + !digitalRead(13);  //A相とB相の信号を 000000ABとして変数に代入

  for (int i = 0; i <= (sizeof(currentWave) / sizeof(int)); i++) {

    if (currentWave[i] == 3)
      currentWave[i] = 2;
    else if (currentWave[i] == 2)
      currentWave[i] = 3;                          //回転すると0 1 3 2となるので無理やり2を3、3を2にして0 1 2 3 にする
    oldWave[i] = pos2[i] & B00000011;              //pos(curとoldが0000AB A'B'として入ってる変数)と＆演算して000000A'B'だけ代入する
    pos2[i] = (oldWave[i] << 2) + currentWave[i];  //回転信号を0000AB A'B'として代入

    bool isleft =
      (currentWave[i] == 0 && oldWave[i] == 1) || (currentWave[i] == 1 && oldWave[i] == 2) || (currentWave[i] == 2 && oldWave[i] == 3) || (currentWave[i] == 3 && oldWave[i] == 0);
    bool isright =
      (currentWave[i] == 0 && oldWave[i] == 3) || (currentWave[i] == 1 && oldWave[i] == 0) || (currentWave[i] == 2 && oldWave[i] == 1) || (currentWave[i] == 3 && oldWave[i] == 2);
    if (isleft) {  //反時計回りに回った時
      Arrayleft[i] = 100;
      Arrayright[i] = 0;
    } else if (isright) {  //時計回りに回った時
      Arrayleft[i] = 0;
      Arrayright[i] = 100;
    }
  }
}