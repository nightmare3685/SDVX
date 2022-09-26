#include <HID-Project.h>
#include <HID-Settings.h>
#include <FlexiTimer2.h>  //FlexiTimer2をインクルード
public class Button{
  char keymap[100] = {  's',  'd',  'k',  'l',  'c',  'm',};
  char BT_A = keymap[0], BT_B = keymap[1], BT_C = keymap[2], BT_D = keymap[3], FX_L = keymap[4], FX_R = keymap[5];
}

char BT_A = 's', BT_B = 'd', BT_C = 'k', BT_D = 'l', FX_L = 'c', FX_R = 'm';

volatile byte pos;
volatile int right = 0, left = 0;  //ここ配列にするとつまみにできると思う。
void setup() {
  FlexiTimer2::set(5, 1.0 / 1000000, TimerIRQ);  //タイマー割り込み 引数1:時間 引数2:時間の単位 引数3:割り込む関数
  FlexiTimer2::start();                          //タイマ割込み実行
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  //pinMode(10, INPUT_PULLUP);
  //pinMode(11, INPUT_PULLUP);
}

void loop() {
  //keyfunc();
  String strout = "Right:" + String(right + 1000) + "," + "Left:" + String(left + 1000);


  Serial.println(strout);
  //ロータリーエンコーダーの変数を減らし続ける。0が下限
  ReduseValue();
}

void ReduseValue() {
  right -= 1;
  left -= 1;
  if (right <= 0) {
    right = 0;
  }
  if (left <= 0) {
    left = 0;
  }
}
void keyfunc() {

  

  if (!digitalRead(0) == HIGH) {    NKROKeyboard.add(BT_A);  } 
  else if (!digitalRead(0) == LOW) {    NKROKeyboard.remove(BT_A);  }
  if (!digitalRead(1) == HIGH) { NKROKeyboard.add(BT_B); }
  else if (!digitalRead(1) == LOW) { NKROKeyboard.remove(BT_B); }
  if (!digitalRead(2) == HIGH) { NKROKeyboard.add(BT_C); }
  else if (!digitalRead(2) == LOW) { NKROKeyboard.remove(BT_C); }
  if (!digitalRead(3) == HIGH) { NKROKeyboard.add(BT_D); }
  else if (!digitalRead(3) == LOW) { NKROKeyboard.remove(BT_D); }
  if (!digitalRead(4) == HIGH) { NKROKeyboard.add(FX_L));  }
  else if (!digitalRead(4) == LOW) { NKROKeyboard.remove(FX_L); }
  if (!digitalRead(5)) == HIGH) {      NKROKeyboard.add(FX_R);    }
  else if (!digitalRead(5) == LOW) { NKROKeyboard.remove(FX_R); }
  /**/


  NKROKeyboard.send();
}

// void ArrayIRQ(){
//   byte Array_currentWave[2];  //最新のロータリー信号
//   byte Array_oldWave[2];      //ひとつ前の信号
//     //-------
//     currentWave[0] = (!digitalRead(0) << 1) + !digitalRead(1);  //A相とB相の信号を 000000ABとして変数に代入
//   if (currentWave[0] == 3)
//     currentWave[0] = 2;
//   else if (currentWave[0] == 2)
//     currentWave[0] = 3;                   //回転すると0 1 3 2となるので無理やり2を3、3を2にして0 1 2 3 にする
//   oldWave[0] = pos & B00000011;           //pos(curとoldが0000AB A'B'として入ってる変数)と＆演算して000000A'B'だけ代入する
//   pos = (oldWave[0] << 2) + currentWave[0];  //回転信号を0000AB A'B'として代入

// }

void TimerIRQ() {
  byte currentWave;  //最新のロータリー信号
  byte oldWave;      //ひとつ前の信号
  //



  currentWave = (!digitalRead(0) << 1) + !digitalRead(1);  //A相とB相の信号を 000000ABとして変数に代入
  if (currentWave == 3)
    currentWave = 2;
  else if (currentWave == 2)
    currentWave = 3;                   //回転すると0 1 3 2となるので無理やり2を3、3を2にして0 1 2 3 にする
  oldWave = pos & B00000011;           //pos(curとoldが0000AB A'B'として入ってる変数)と＆演算して000000A'B'だけ代入する
  pos = (oldWave << 2) + currentWave;  //回転信号を0000AB A'B'として代入



  if (currentWave == 0 && oldWave == 1)  //反時計回りに回った時
    right -= 50;
  if (right < 0) {
    right = 0;
    left = 100;
  } else if (currentWave == 1 && oldWave == 2)  //反時計回りに回った時
    right -= 50;
  if (right < 0) {
    right = 0;
    left = 100;
  } else if (
    currentWave == 2 && oldWave == 3)  //反時計回りに回った時
    right -= 50;
  if (right < 0) {
    right = 0;
    left = 100;
  } else if (
    currentWave == 3 && oldWave == 0)  //反時計回りに回った時
  {
    right -= 50;
    if (right < 0) {
      right = 0;
      left = 100;
    }
  } else if (
    currentWave == 0 && oldWave == 3)  //時計回りに回った時
  {
    left -= 50;
    if (left < 0) {
      right = 100;
      left = 0;
    }
  } else if (
    currentWave > oldWave)  //時計回りに回った時条件2
  {
    left -= 50;
    if (left < 0) {
      right = 100;
      left = 0;
    }
  }

  /*
     //これだと一つにできる？？？
         else if ((
currentWave == 0 && oldWave == 0)&&(
currentWave > oldWave)) //時計回りに回った時
         {
         left -= 50;
           if (left < 0) {
             right = 100;
             left = 0;
           }
         }
   */
}