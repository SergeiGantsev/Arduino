#include <Mouse.h>
#include <Keyboard.h>

//Settings
int maxScrollDown = 10;
int maxTimeScrollDown = 15;
int maxTimeMouseClick = 10;
int maxRandomMove = 5;
int rightMove = 100;
int maxPause = 30;
int stepScroll = -1;

//var
bool fStart    = false;
bool buttonClick = false;
int curStep = 1;
//char ctrlKey = KEY_LEFT_GUI;//mac
char ctrlKey = KEY_LEFT_CTRL;// microsoft


//Scroll
int scrollDown = 10;
int curScrollDown = 1;
int timeScrollDown = 5;
long curTimerStep1 = 0;

//Click
long curTimerStep2 = 0;
int timeMouseClick = 10;

//Move
int curMove = 20;

//Right
int rightScrollDown = 10;

//Pause
int pause = 10;
long curTimerPause = 0;

bool fScenario = false;


void setup() {
  pinMode(6, OUTPUT);
  pinMode(4, INPUT);

  Mouse.begin();
  Keyboard.begin();
  Serial.begin(9600);

  randomSeed(analogRead(0));
}

void loop() {
  int buttonState = digitalRead(4);

  //Serial.println(String(buttonState));

  if (buttonState == HIGH) {
    if (not buttonClick) {
      buttonClick = true;
      fStart = not fStart;
    }
  }
  else {
    buttonClick = false;
  }

  if (fStart) {
    digitalWrite (6, HIGH);

    if (not fScenario) {
      curStep = 1;

      //Scrolling
      scrollDown = calcRandom(1, maxScrollDown);
      timeScrollDown = calcRandom(1, maxTimeScrollDown);
      curScrollDown = 1;

      //Left button press
      timeMouseClick = calcRandom(1, maxTimeMouseClick);

      //Right scroll
      rightScrollDown = calcRandom(1, maxScrollDown);

      //Pause
      pause = calcRandom(1, maxPause);

      fScenario = true;
    }
    //Работа сценария
    else {
      switch (curStep) {
        case 1:
          execScrollDown(scrollDown, stepScroll); break;
        case 2:
          execPause(); break;
        case 3:
          execMouseClick(); break;
        case 4:
          execPause(); break;
        case 5:
          execMouseRight(); break;
        case 6:
          execPause(); break;
        case 7:
          execScrollDown(rightScrollDown, stepScroll);
          break;
        case 8:
          execPause(); break;
        case 9:
          execMouseLeft(); break;
        case 10:
          execPause(); break;
        case 11:
          execKeyboardPress(); break;
        case 12:
          execPause(); break;
        default:
          fScenario = false; break;
      }
    }
  }
  else {
    digitalWrite (6, LOW);
    fScenario = false;
  }

  delay(100);
}

long calcRandom(int numStart, int numEnd) {
  return random(numStart, numEnd);
}

void execScrollDown(int sd, int scrollStep) {
  if (millis() - curTimerStep1 > timeScrollDown * 1000) {
    Serial.println("sd=" + String(sd) + "; curScrollDown=" + curScrollDown + "; timeScrollDown=" + timeScrollDown + "; curStep=" + curStep);

    Mouse.move(0, 0, scrollStep);
    curTimerStep1 = millis();

    curScrollDown = curScrollDown + 1;

    if (curScrollDown > sd) {
      curStep++;
      curScrollDown = 1;
    }
  }
}

void execMouseClick() {
  Mouse.click(MOUSE_LEFT);
  Serial.println("Left batton is pressed");

  curStep++;
}

void execMouseRight() {
  curMove = rightMove;// - calcRandom(1, maxRandomMove);
  Mouse.move(curMove, 0);
  Mouse.move(curMove, 0);
  Mouse.move(curMove, 0);
  
  //Mouse.click(MOUSE_RIGHT);
  //Keyboard.press(KEY_LEFT_ALT);
  //delay(100);
  //Keyboard.releaseAll();
  
  Serial.println("Right move; curMove=" + String(curMove));
  curStep++;
}

void execMouseLeft() {
  Mouse.move(-curMove, 0);
  Mouse.move(-curMove, 0);
  Mouse.move(-curMove, 0);
    
  Mouse.click(MOUSE_LEFT);
  Serial.println("Left move; curMove=" + String(-curMove));

  curStep++;
}

void execPause() {
  if (curTimerPause == 0) {
    curTimerPause = millis();
  }

  if (millis() - curTimerPause > pause * 1000) {
    Serial.println("Pause; pause=" + String(pause));
    curTimerPause = 0;
    curStep++;
  }
}

void execKeyboardPress() {
  Keyboard.press(ctrlKey);
  Keyboard.press('c');
  delay(100);
  Keyboard.releaseAll();

  Serial.println("Keyboard press");

  curStep++;
}
