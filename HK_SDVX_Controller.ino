#include <HID-Project.h>
#include <HID-Settings.h>
#include <FlexiTimer2.h>


class Button {
public:
  char keymap[100] = { 's', 'd', 'k', 'l', 'c', 'm' /*6:Enter 7:ESC*/ };
  char BT_A = keymap[0], BT_B = keymap[1], BT_C = keymap[2], BT_D = keymap[3], FX_L = keymap[4], FX_R = keymap[5];
private:
  ;
};

volatile byte pos;
volatile int right = 0, left = 0;
volatile int right2 = 0, left2 = 0;
//volatile int[2]right={0,0},left={0,0};


void setup() {
  FlexiTimer2::set(5, 1.0 / 1000000, TimerIRQ2);  //タイマー割り込み 引数1:時間 引数2:時間の単位 引数3:割り込む関数
  FlexiTimer2::start();                           //タイマ割込み実行
  Serial.begin(9600);
  for (int i = 0; i < 13; i++) {  //13pinすべてアップ
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  //keyFunc();
  String strout = "Right:" + String(right + 1000) + "," + "Left:" + String(left + 1000);
  String strout2 = "Right2:" + String(right2 + 1000) + "," + "Left2:" + String(left2 + 1000);


  //Serial.println(strout);
  Serial.println(strout2);
  //
  ReduseValue();
}

void ReduseValue() {  //ロータリーエンコーダーの変数を減らし続ける。0が下限
  right--;
  left--;
  if (right <= 0) {
    right = 0;
  }
  if (left <= 0) {
    left = 0;
  }


  right2--;
  left2--;
  if (right2 <= 0) {
    right2 = 0;
  }
  if (left2 <= 0) {
    left2 = 0;
  }
}

void keyFunc() {
  Button button;
  for (int i = 0; i < 6; i++) {
    if (!digitalRead(i) == HIGH) {
      NKROKeyboard.add(button.keymap[i]);
    } else if (!digitalRead(i) == LOW) {
      NKROKeyboard.remove(button.keymap[i]);
    }
  }

  if (right > 0) {
    NKROKeyboard.add('w');
  } else {
    NKROKeyboard.remove('w');
  }
  if (left > 0) {
    NKROKeyboard.add('q');
  } else {
    NKROKeyboard.remove('q');
  }
  //Serial.println(right);
  //Serial.println(left);
  NKROKeyboard.send();
}

// void ArrayIRQ(){
//   byte Array_currentWave[2];  //最新のロータリー信号
//   byte Array_oldWave[2];      //ひとつ前の信号
//     //-------
//     currentWave[0] = (!digitalRead(0) << 1) + !digitalRead(1);  //A相とB相の信号を 000000ABとして変数に代入
//   if (currentWave[0] == 3)
//     currentWave[0] = 2;
//   else if (currentWave[0] == 2)q
//     currentWave[0] = 3;                   //回転すると0 1 3 2となるので無理やり2を3、3を2にして0 1 2 3 にする
//   oldWave[0] = pos & B00000011;           //pos(curとoldが0000AB A'B'として入ってる変数)と＆演算して000000A'B'だけ代入する
//   pos = (oldWave[0] << 2) + currentWave[0];  //回転信号を0000AB A'B'として代入

// }

void TimerIRQ() {
  byte currentWave;  //最新のロータリー信号
  byte oldWave;      //ひとつ前の信号




  currentWave = (!digitalRead(10) << 1) + !digitalRead(11);  //A相とB相の信号を 000000ABとして変数に代入
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


void TimerIRQ2() {
  byte currentWave;  //最新のロータリー信号
  byte oldWave;      //ひとつ前の信号




  currentWave = (!digitalRead(10) << 1) + !digitalRead(11);  //A相とB相の信号を 000000ABとして変数に代入
  if (currentWave == 3)
    currentWave = 2;
  else if (currentWave == 2)
    currentWave = 3;                   //回転すると0 1 3 2となるので無理やり2を3、3を2にして0 1 2 3 にする
  oldWave = pos & B00000011;           //pos(curとoldが0000AB A'B'として入ってる変数)と＆演算して000000A'B'だけ代入する
  pos = (oldWave << 2) + currentWave;  //回転信号を0000AB A'B'として代入



  if (currentWave == 0 && oldWave == 1)  //反時計回りに回った時
    right2 -= 50;
  if (right2 < 0) {
    right2 = 0;
    left2 = 100;
  } else if (currentWave == 1 && oldWave == 2)  //反時計回りに回った時
    right2 -= 50;
  if (right2 < 0) {
    right2 = 0;
    left2 = 100;
  } else if (
    currentWave == 2 && oldWave == 3)  //反時計回りに回った時
    right2 -= 50;
  if (right2 < 0) {
    right2 = 0;
    left2 = 100;
  } else if (
    currentWave == 3 && oldWave == 0)  //反時計回りに回った時
  {
    right2 -= 50;
    if (right2 < 0) {
      right2 = 0;
      left2 = 100;
    }
  } else if (
    currentWave == 0 && oldWave == 3)  //時計回りに回った時
  {
    left2 -= 50;
    if (left2 < 0) {
      right2 = 100;
      left2 = 0;
    }
  } else if (
    currentWave > oldWave)  //時計回りに回った時条件2
  {
    left2 -= 50;
    if (left2 < 0) {
      right2 = 100;
      left2 = 0;
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