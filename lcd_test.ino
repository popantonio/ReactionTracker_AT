#include <LiquidCrystal.h>

// LCD Keypad Shield pin mapping
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Built-in LED pin
const int LED_PIN = 13;

// Button codes
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int readLCDButtons() {
  int adcKeyIn = analogRead(A0);

  if (adcKeyIn > 1000) return btnNONE;
  if (adcKeyIn < 50)   return btnRIGHT;
  if (adcKeyIn < 200)  return btnUP;
  if (adcKeyIn < 400)  return btnDOWN;
  if (adcKeyIn < 600)  return btnLEFT;
  if (adcKeyIn < 850)  return btnSELECT;

  return btnNONE;
}

enum GameState {
  WAIT_START,
  RANDOM_WAIT,
  SHOW_GO,
  SHOW_RESULT,
  TOO_EARLY
};

GameState state = WAIT_START;

unsigned long waitStartTime = 0;
unsigned long goTime = 0;
unsigned long randomDelayMs = 0;
unsigned long reactionTime = 0;

bool lastButtonPressed = false;

bool anyButtonPressed() {
  return readLCDButtons() != btnNONE;
}

bool newButtonPress() {
  bool currentPressed = anyButtonPressed();
  bool result = false;

  if (currentPressed && !lastButtonPressed) {
    result = true;
  }

  lastButtonPressed = currentPressed;
  return result;
}

void showStartScreen() {
  digitalWrite(LED_PIN, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reaction Game");
  lcd.setCursor(0, 1);
  lcd.print("Press SELECT");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  lcd.begin(16, 2);
  randomSeed(analogRead(A1));

  showStartScreen();
}

void loop() {
  int button = readLCDButtons();

  switch (state) {
    case WAIT_START:
      if (button == btnSELECT && newButtonPress()) {
        digitalWrite(LED_PIN, LOW);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Get ready...");
        lcd.setCursor(0, 1);
        lcd.print("Wait for GO!");

        randomDelayMs = random(2000, 5001);
        waitStartTime = millis();
        state = RANDOM_WAIT;
      } else {
        newButtonPress();
      }
      break;

    case RANDOM_WAIT:
      if (newButtonPress()) {
        digitalWrite(LED_PIN, LOW);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Too early!");
        lcd.setCursor(0, 1);
        lcd.print("SELECT=retry");
        state = TOO_EARLY;
      } else if (millis() - waitStartTime >= randomDelayMs) {
        digitalWrite(LED_PIN, HIGH);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("GO NOW!");
        lcd.setCursor(0, 1);
        lcd.print("Press button!");

        goTime = millis();
        state = SHOW_GO;
      }
      break;

    case SHOW_GO:
      if (newButtonPress()) {
        digitalWrite(LED_PIN, LOW);

        reactionTime = millis() - goTime;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Time:");
        lcd.print(reactionTime);
        lcd.print(" ms");

        lcd.setCursor(0, 1);
        lcd.print("SELECT=again");

        state = SHOW_RESULT;
      }
      break;

    case SHOW_RESULT:
      if (button == btnSELECT && newButtonPress()) {
        showStartScreen();
        state = WAIT_START;
      } else {
        newButtonPress();
      }
      break;

    case TOO_EARLY:
      if (button == btnSELECT && newButtonPress()) {
        showStartScreen();
        state = WAIT_START;
      } else {
        newButtonPress();
      }
      break;
  }
}
