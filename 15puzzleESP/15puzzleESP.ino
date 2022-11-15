
#include <TFT_eSPI.h>
#include "logo.h"
#define N 4

TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite sprite   = TFT_eSprite(&lcd);

int box = 42;
int posX[4];
int posY[4];

int taken[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};

int fx = 0;
int fy = 0;
int freeX[4] = { -100};
int freeY[4] = { -100};
int movableN = 0;
int chosenF = 0;

int numbers[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 0, 14, 15}};
int numbersFinish[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}};
int numbersTaken[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int numbersTakenS[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int random1 = 0;
int random2 = 0;
int temp = 0;

bool finish = 0;
bool search = 0;
int n = 0;

int pos[2] = {0, 0};
int left = 2;
int top = 36;
bool finded = 0;
int moves = 0;
int sec = 0;

unsigned short color1=0xEEF8;
unsigned short color2=0x332E;
unsigned short color3=0x0228;
unsigned short color4=0xD2D8;
unsigned short color5= 0xFE86;

int deb1 = 0;
int deb2 = 0;

int correct=0;
void setup(void)
{
  pinMode(0, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  lcd.init();
  lcd.setSwapBytes(true);
  lcd.setTextColor(color1,TFT_BLACK);
  
  sprite.createSprite(170, 170);
  sprite.setTextDatum(4);
  
  for (int i = 0; i < 4; i++)
  {
    posX[i] = i * box;
    posY[i] = posX[i];
  }
  gameStart();

     ledcSetup(0, 10000, 8);
     ledcAttachPin(38, 0);
     ledcWrite(0, 200);
}

long currentTime = 0;

void loop()
{
  if (digitalRead(0) == 0)
  {
    if (deb1 == 0)
    {
      deb1 = 1;
      chosenF++;
      if (chosenF == movableN)
        chosenF = 0;
      moves++;
      lcd.drawString(String(moves), 140, 300,2);
      delay(20);
    }
  } else {
    deb1 = 0;
  }

  if (digitalRead(14) == 0)
  {
    if (deb2 == 0)
    {
      deb2 = 1;
      moves++;
      taken[freeY[chosenF]][freeX[chosenF]] = 0;
      taken[fy][fx] = 1;
      numbers[fy][fx] = numbers[freeY[chosenF]][freeX[chosenF]];
      numbers[freeY[chosenF]][freeX[chosenF]] = 0;
      chosenF = 0;
      lcd.drawString(String(moves), 140, 300,2);
      delay(20);
      findMovable();
    }
  } else {
    deb2 = 0;
  }

  checkFinish();

  if (finish == 0)
    if (currentTime + 1000 < millis())
    {
      currentTime = millis();
      sec++;
      lcd.drawString(String(sec), 140, 250,2);
     
    }
 drawSprite();
}

void drawSprite()
{

  sprite.setTextColor(color1, color2);
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextDatum(1);
  if (finish == 1)
  {
    sprite.fillRoundRect(0, 0, 170, 170, 5, 0xE6AE);
    sprite.setTextColor(TFT_PURPLE, 0xE6AE);
   
    sprite.drawString("Well Done!", 40, 20,2);
    sprite.drawString("Moves: " + String(moves), 40, 60,2);
    sprite.drawString("time: " + String(sec), 40, 100,2);
  }


  if (finish == 0)
    for (int j = 0; j < 4; j++)
      for (int i = 0; i < 4; i++)
        if (taken[j][i] == 1) {
          sprite.fillRoundRect(posX[i], posY[j], box - 3, box - 3, 3, color3);
          sprite.fillCircle(posX[i] + 20, posY[j] + 20, 14, color2);
          sprite.drawString(String(numbers[j][i]), posX[i] + 20, posY[j] + 12, 2);
          if (numbers[j][i] == numbersFinish[j][i])
            sprite.fillCircle(posX[i] + 6, posY[j] + 6, 3, color4);
        }
        else {
          sprite.fillRoundRect(posX[i], posY[j], box - 4, box - 4, 5, TFT_BLACK);

        }

  sprite.drawRoundRect(freeX[chosenF]*box, freeY[chosenF]*box, box - 3, box - 3, 5, TFT_WHITE);
  sprite.pushSprite(left, top);
  
}

void checkFinish()
{
  correct=0;
  finish = 1;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (finish == 1)
        if (numbers[i][j] != numbersFinish[i][j])
          finish = 0;

    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    if((numbers[i][j] == numbersFinish[i][j]))
    correct++;
}

void shufle()
{
  for (int i = 0; i < 16; i++)
    numbersTaken[i] = numbersTakenS[i];

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      taken[i][j] = 1;


  for (int s = 0; s < 200; s++)
  {
    random1 = random(0, 16);
    random2 = random(0, 16);
    temp = numbersTaken[random1];
    numbersTaken[random1] = numbersTaken[random2];
    numbersTaken[random2] = temp;

  }

  int r = 0;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
      numbers[i][j] = numbersTaken[r];

      if (numbersTaken[r] == 0) {
        taken[i][j] = 0;

      }
      r++;
    }
  delay(50);
  findMovable();
}

void findMovable()
{
  fx = 0;
  fy = 0;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (taken[i][j] == 0) {
        fx = j;
        fy = i;
      }

  int u = 0;
  bool findedNear = 0;

  if (fx == 0 & fy == 0)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 2;
      freeX[0] = fx + 1;
      freeY[0] = fy;

      freeX[1] = fx;
      freeY[1] = fy + 1;
    }


  if (fx == 3 & fy == 3)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 2;
      freeX[0] = fx - 1;
      freeY[0] = fy;

      freeX[1] = fx;
      freeY[1] = fy - 1;
    }

  if (fx == 3 & fy == 0)
    if (findedNear == 0)
    {
      findedNear == 1;
      movableN = 2;
      freeX[0] = fx - 1;
      freeY[0] = fy;

      freeX[1] = fx;
      freeY[1] = fy + 1;
    }

  if (fx == 0 & fy == 3)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 2;
      freeX[0] = fx;
      freeY[0] = fy - 1;

      freeX[1] = fx + 1;
      freeY[1] = fy;
    }




  if (fx > 0 & fx<3  & fy>0 & fy < 3)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 4;
      freeX[0] = fx - 1;
      freeY[0] = fy;

      freeX[1] = fx;
      freeY[1] = fy - 1;

      freeX[2] = fx + 1;
      freeY[2] = fy;

      freeX[3] = fx;
      freeY[3] = fy + 1;
    }


  if (fx > 0 & fx < 3  & fy == 0)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 3;
      freeX[0] = fx - 1;
      freeY[0] = fy;
      freeX[1] = fx;
      freeY[1] = fy + 1;
      freeX[2] = fx + 1;
      freeY[2] = fy;
    }

  if (fx > 0 & fx < 3  & fy == 3)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 3;
      freeX[0] = fx - 1;
      freeY[0] = fy;
      freeX[1] = fx;
      freeY[1] = fy - 1;
      freeX[2] = fx + 1;
      freeY[2] = fy;
    }

  if (fy > 0 & fy < 3  & fx == 0)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 3;
      freeX[0] = fx;
      freeY[0] = fy - 1;
      freeX[1] = fx + 1;
      freeY[1] = fy;
      freeX[2] = fx;
      freeY[2] = fy + 1;
    }

  if (fy > 0 & fy < 3  & fx == 3)
    if (findedNear == 0)
    {
      findedNear = 1;
      movableN = 3;
      freeX[0] = fx;
      freeY[0] = fy - 1;
      freeX[1] = fx - 1;
      freeY[1] = fy;
      freeX[2] = fx;
      freeY[2] = fy + 1;
    }

}

void gameStart()
{
  sec = 0;
  moves = 0;
  finish = 0;
     lcd.fillScreen(TFT_BLACK);

     lcd.pushImage(132,0,32,32,logo);
     
     
     lcd.drawString("RESET: New Game",4,224);
     lcd.drawString("LEFT: Select tile",4,236);
     lcd.drawString("RIGHT: move tile",4,248);
     lcd.drawString("to empty place!",4,260);
     
     lcd.drawString("15 Puzzle",4,4,4);
     
     lcd.setTextColor(color5,TFT_BLACK);
     lcd.drawString("How to play?",4,206,2);
     lcd.drawRoundRect(posX[2]+30,230,box*2-30,box,4,color5);
     lcd.drawString("TIME",posX[2]+16+30,234);

     lcd.drawRoundRect(posX[2]+30,276,box*2-30,box,4,color5);
     lcd.drawString("MOVES",posX[2]+30+14,280);

     lcd.pushImage(4,276,40,40,move);
     lcd.pushImage(54,276,40,40,check);

     lcd.setTextDatum(4);
     lcd.setTextColor(color4,TFT_BLACK);
     lcd.drawString(String(correct)+"/16   ",140,218,2);
    
    lcd.setTextColor(color1,TFT_BLACK);
    
    
  shufle();
  while (isSolvable(numbers) == 0)
  {
    shufle();
  }
}

int getInvCount(int arr[])
{
  int inv_count = 0;
  for (int i = 0; i < N * N - 1; i++)
  {
    for (int j = i + 1; j < N * N; j++)
    {
      // count pairs(arr[i], arr[j]) such that
      // i < j but arr[i] > arr[j]
      if (arr[j] && arr[i] && arr[i] > arr[j])
        inv_count++;
    }
  }
  return inv_count;
}

// find Position of blank from bottom
int findXPosition(int puzzle[N][N])
{
  // start from bottom-right corner of matrix
  for (int i = N - 1; i >= 0; i--)
    for (int j = N - 1; j >= 0; j--)
      if (puzzle[i][j] == 0)
        return N - i;
}

bool isSolvable(int puzzle[N][N])
{
  // Count inversions in given puzzle
  int invCount = getInvCount((int*)puzzle);

  // If grid is odd, return true if inversion
  // count is even.
  if (N & 1)
    return !(invCount & 1);

  else     // grid is even
  {
    int pos = findXPosition(puzzle);
    if (pos & 1)
      return !(invCount & 1);
    else
      return invCount & 1;
  }
}
