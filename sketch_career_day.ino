#include "Arduino_SensorKit.h"
#define BUTTON 4
#define BUZZER 5
#define LED 6

int buttonState = 0;
bool buttonAlreadyProcessed = false;
bool isLightOn = false;
unsigned long currentTime = 0;
unsigned long timer = 0;
long randomNumber;
int score = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  Oled.begin();
  Oled.setFlipMode(true);
  Oled.setFont(u8x8_font_chroma48medium8_r);
  randomSeed(analogRead(0));

  newGame();
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();

  // if the timer has elapsed, toggle the light, and set a new timer
  if (currentTime > timer) {
    toggleLight();
    setTimer();
  }

  if (buttonAlreadyProcessed) {
    if (!isButtonPressed()) {
      // the button has been released - re-enable it
      buttonAlreadyProcessed = false;
    }
  } else if (isButtonPressed()) {
    handleButtonPress();
  }
}

void handleButtonPress() {
  // when the button is pressed, if the light is on - add a point
  if (isLightOn) {
    addPoint();
  } else {  // else, when the light is off - end the game
    gameOver();
  }

  buttonAlreadyProcessed = true; // BUGFIX: don't count button presses more than once
}

void setTimer() {
  // pick a random number from 1000-4999 - how long to leave the light on in milliseconds
  randomNumber = random(1000, 5000);
  timer = millis() + randomNumber;
}

void turnLightOn() {
  isLightOn = true;
  digitalWrite(LED, HIGH);
}

void turnLightOff() {
  isLightOn = false;
  digitalWrite(LED, LOW);
}

void toggleLight() {
  if (isLightOn) {
    turnLightOff();
  } else {
    turnLightOn();
  }
}

bool isButtonPressed() {
  return digitalRead(BUTTON) == HIGH;
}

void addPoint() {
  // add a point
  score = score + 1;

  // update the screen
  displayScore();

  // play "ding" noise
  tone(BUZZER, 200);
  delay(100);
  noTone(BUZZER);
}

void newGame() {
  score = 0;
  displayScore();
}

void gameOver() {
  // show "Game Over" on the screen
  Oled.clearDisplay();
  Oled.setCursor(0, 33);
  Oled.print("Game Over! :(");
  Oled.refreshDisplay();

  // play "buzz" noise
  tone(BUZZER, 75);
  delay(1000);
  noTone(BUZZER);

  // start a new game
  newGame();
}

void displayScore() {
  Oled.clearDisplay();
  Oled.setCursor(0, 33);
  Oled.print(score);
  Oled.refreshDisplay();
}
