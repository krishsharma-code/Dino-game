/*
 *  MINI DINO  -  Arduino Uno + 16x2 I2C LCD
 *  ------------------------------------------------------------------
 *  A tiny Chrome-dino style game for a cheap 16x2 character LCD.
 *  The dino and the cactus are drawn with custom 5x8 characters
 *  (CGRAM). Press the KY-040 encoder button to jump over the cactus.
 *
 *  PARTS (all solderless, plug into breadboard / Uno sockets):
 *    Arduino Uno R3  |  16x2 I2C LCD (PCF8574)  |  KY-040 rotary encoder
 *    Passive piezo buzzer  |  solderless breadboard  |  jumper wires
 *
 *  WIRING:
 *    LCD    : VCC->5V, GND->GND, SDA->A4, SCL->A5
 *    KY-040 : SW->D3, VCC->5V, GND->GND     (DT->D4, CLK->D2 wired, unused)
 *    Buzzer : + -> D10, - -> GND
 *
 *  LIBRARY NEEDED (install from Arduino IDE > Library Manager):
 *    "LiquidCrystal I2C" by Frank de Brabander
 *
 *  NOTE: if the screen stays blank, change the address below
 *        from 0x27 to 0x3F (or run an I2C scanner).
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   // try 0x3F if 0x27 shows nothing

// ---- pins ----
#define SW      3       // KY-040 push switch  -> jump / start
#define BUZZER  10      // passive piezo buzzer

// ---- custom characters (5 wide x 8 tall) ----
// 1 = lit pixel, 0 = blank
byte dinoRunA[8] = {0b00110, 0b01111, 0b01110, 0b11111, 0b01111, 0b00110, 0b01010, 0b01000};
byte dinoRunB[8] = {0b00110, 0b01111, 0b01110, 0b11111, 0b01111, 0b00110, 0b01010, 0b00010};
byte cactusCh[8] = {0b00100, 0b00100, 0b10100, 0b10101, 0b11111, 0b00100, 0b00100, 0b00100};

// ---- game state ----
#define ST_START 0
#define ST_PLAY  1
#define ST_OVER  2
byte state = ST_START;

int  cactusCol = 15;          // cactus column (right edge = 15)
int  dinoRow   = 1;           // 1 = on the ground, 0 = in the air
bool jumping   = false;
unsigned long jumpStart = 0;
const int JUMP_MS = 450;      // how long a jump lasts

int  score = 0;
int  frame = 0;
unsigned long lastMove = 0;
const int MOVE_MS = 230;      // ms per cactus step (game speed)

volatile bool buttonPressed = false;

// cached values so we redraw only what changed (no flicker)
int prevCactus   = -1;
int prevDinoRow  = -1;
int prevDinoChar = -1;
int prevScore    = -1;

void onButton() { buttonPressed = true; }

void showStart() {
  lcd.clear();
  lcd.setCursor(2, 0); lcd.print("MINI DINO");
  lcd.setCursor(1, 1); lcd.print("Press to start");
  state = ST_START;
}

void startGame() {
  cactusCol = 15; dinoRow = 1; jumping = false; score = 0; frame = 0;
  prevCactus = -1; prevDinoRow = -1; prevDinoChar = -1; prevScore = -1;
  lcd.clear();
  lastMove = millis();
  state = ST_PLAY;
}

void gameOver() {
  tone(BUZZER, 125, 400);
  lcd.clear();
  lcd.setCursor(3, 0); lcd.print("GAME OVER");
  lcd.setCursor(4, 1); lcd.print("Score:"); lcd.print(score);
  state = ST_OVER;
}

void jump() {
  if (!jumping) {
    jumping = true;
    jumpStart = millis();
    tone(BUZZER, 700, 80);
  }
}

void handleButton() {
  if (state == ST_START)      startGame();
  else if (state == ST_PLAY)  jump();
  else                        showStart();
}

void renderPlay() {
  byte d = ((frame / 2) % 2) ? 1 : 0;   // alternate the two run frames

  // --- cactus ---
  if (cactusCol != prevCactus) {
    if (prevCactus >= 0 && prevCactus <= 15) { lcd.setCursor(prevCactus, 1); lcd.print(' '); }
    lcd.setCursor(cactusCol, 1);
    lcd.write(byte(2));
    prevCactus = cactusCol;
  }

  // --- dino ---
  if (dinoRow != prevDinoRow || d != prevDinoChar) {
    if (prevDinoRow >= 0) { lcd.setCursor(0, prevDinoRow); lcd.print(' '); }
    lcd.setCursor(0, dinoRow);
    lcd.write(d);
    prevDinoRow = dinoRow; prevDinoChar = d;
  }

  // --- score, top-right, redrawn only when it changes ---
  if (score != prevScore) {
    lcd.setCursor(12, 0); lcd.print("    ");
    lcd.setCursor(12, 0); lcd.print("S"); lcd.print(score);
    prevScore = score;
  }

  frame++;
}

void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(SW), onButton, FALLING);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, dinoRunA);
  lcd.createChar(1, dinoRunB);
  lcd.createChar(2, cactusCh);

  showStart();
}

void loop() {
  if (buttonPressed) { buttonPressed = false; handleButton(); }

  if (state == ST_PLAY) {
    // jump timing
    if (jumping && millis() - jumpStart >= JUMP_MS) jumping = false;
    dinoRow = jumping ? 0 : 1;

    // move the cactus one step left
    if (millis() - lastMove >= MOVE_MS) {
      lastMove = millis();
      cactusCol--;
      if (cactusCol < 0) {
        cactusCol = 15;      // passed safely -> new cactus, score up
        score++;
      } else if (cactusCol == 0 && dinoRow == 1) {
        gameOver();          // hit the cactus on the ground
      }
    }

    if (state == ST_PLAY) renderPlay();
  }

  delay(10);
}
