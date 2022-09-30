#include <HID-Project.h>
#include <HID-Settings.h>
#include <FlexiTimer2.h>

class Button
{
public:
  const char keymap[6] = {'s', 'd', 'k', 'l', 'c', 'm' /*6:Enter 7:ESC*/};
  const char VOL_L = 0, VOL_R = 1;
  const char VOL_LL = 'q', VOL_LR = 'w', VOL_RL = 'o', VOL_RR = 'p';

private:
  ;
};

volatile int posision[2] = {0, 0};
volatile int Arrayright[2] = {0, 0};
volatile int Arrayleft[2] = {0, 0};
Button button;

void setup()
{

  FlexiTimer2::set(5, 1.0 / 100000, TimerIRQ); //タイマー割り込み 引数1:時間 引数2:時間の単位 引数3:割り込む関数
  FlexiTimer2::start();                        //タイマ割込み実行
  Serial.begin(115200);
  for (int i = 0; i <= 13; i++)
  { // 13pinすべてアップ
    pinMode(i, INPUT_PULLUP);
  }
}

void loop()
{
  keyFunc();
  ReduseValue();
}

void ReduseValue()
{
  for (int i = 0; i < (sizeof(Arrayleft) / sizeof(int)); i++)
  {

    Arrayright[i]--;
    Arrayleft[i]--;
    if (Arrayright[i] <= 0)
    {
      Arrayright[i] = 0;
    }
    if (Arrayleft[i] <= 0)
    {
      Arrayleft[i] = 0;
    }
  }
}

void keyFunc()
{
  NKROKeyboard.begin();
  for (int i = 0; i < (sizeof(button.keymap) / sizeof(char)); i++)
  {
    if (!digitalRead(i) == HIGH)
    {
      NKROKeyboard.add(button.keymap[i]);
    }
    else if (!digitalRead(i) == LOW)
    {
      NKROKeyboard.remove(button.keymap[i]);
    }
  }

  Arrayright[button.VOL_L] > 0 ? NKROKeyboard.add(button.VOL_LR) : NKROKeyboard.remove(button.VOL_LR); //左のつまみが右回転
  Arrayright[button.VOL_R] > 0 ? NKROKeyboard.add(button.VOL_RR) : NKROKeyboard.remove(button.VOL_RR); //右のつまみが右回転
  Arrayleft[button.VOL_L] > 0 ? NKROKeyboard.add(button.VOL_LL) : NKROKeyboard.remove(button.VOL_LL);  //左のつまみが左回転
  Arrayleft[button.VOL_R] > 0 ? NKROKeyboard.add(button.VOL_RL) : NKROKeyboard.remove(button.VOL_RL);  //右のつまみが左回転

  NKROKeyboard.send();
  NKROKeyboard.end();
}

void TimerIRQ()
{
  int currentWave[2]; //最新のロータリー信号
  int OldWave[2];     //ひとつ前の信号

  currentWave[button.VOL_L] = (!digitalRead(10) << 1) + !digitalRead(11); // A相とB相の信号を 000000ABとして変数に代入
  currentWave[button.VOL_R] = (!digitalRead(12) << 1) + !digitalRead(13); // A相とB相の信号を 000000ABとして変数に代入

  for (int i = 0; i <= (sizeof(currentWave) / sizeof(int)); i++)
  {

    if (currentWave[i] == 3)
      currentWave[i] = 2;
    else if (currentWave[i] == 2)
      currentWave[i] = 3;                             //回転すると0 1 3 2となるので無理やり2を3、3を2にして0 1 2 3 にする
    OldWave[i] = posision[i] & B00000011;             // pos(curとoldが0000AB A'B'として入ってる変数)と＆演算して000000A'B'だけ代入する
    posision[i] = (OldWave[i] << 2) + currentWave[i]; //回転信号を0000AB A'B'として代入

    bool isleft =
        (currentWave[i] == 0 && OldWave[i] == 1) || (currentWave[i] == 1 && OldWave[i] == 2) || (currentWave[i] == 2 && OldWave[i] == 3) || (currentWave[i] == 3 && OldWave[i] == 0);
    bool isright =
        (currentWave[i] == 0 && OldWave[i] == 3) || (currentWave[i] == 1 && OldWave[i] == 0) || (currentWave[i] == 2 && OldWave[i] == 1) || (currentWave[i] == 3 && OldWave[i] == 2);

    if (isleft)
    { //反時計回りに回った時
      Arrayright[i] -= 50;
      if (Arrayright[i] < 0)
      {
        Arrayleft[i] = 100;
        Arrayright[i] = 0;
      }
    }
    else if (isright)
    { //時計回りに回った時
      Arrayleft[i] -= 50;
      if (Arrayleft[i] < 0)
      {
        Arrayleft[i] = 0;
        Arrayright[i] = 100;
      }
    }
  }
}
