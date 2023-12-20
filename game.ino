#include <LiquidCrystal.h>
#include <LedControl.h>
const long toneDuration = 1000; // Change this to the desired duration of the tone in milliseconds
const long toneInterval = 2000; // Change this to the desired interval between tones in milliseconds

int buzzerPin = 3;
int gameScore = 0;
int highScore = 0;
int startTime2 = 0;
typedef struct Snake {
  int head[2];
  int body[40][2];
  int length;
  int direction[2];
} Snake;

typedef struct Apple {
  int rowPos;
  int colPos;
} Apple;

const int DATA_IN_PIN = 12;
const int CHIP_SELECT_PIN = 10;
const int CLOCK_PIN = 11;
LedControl ledControl = LedControl(DATA_IN_PIN, CLOCK_PIN, CHIP_SELECT_PIN, 0);

const int analogXPin = A1;
const int analogYPin = A0;

byte displayBuffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};

Snake gameSnake = {{1, 5}, {{0, 5}, {1, 5}}, 2, {1, 0}};
Apple gameApple = {(int)random(0, 8), (int)random(0, 8)};

float previousTime = 0;
float timer = 0;
float updateRate = 3;

int i, j;
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
int volume=0;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const byte Analog_X_pin = A0;  // Pin connected to X-axis of joystick
const byte Analog_Y_pin = A1;  // Pin connected to Y-axis of joystick
const byte Analog_Button_pin = 2; // Pin connected to button of joystick
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool inPMS = false;
bool startGame = false;
int currentMenuIndex = 0; // Index of the currently selected menu item

void showMenu() {
  // Clear LCD display
  

  // Display current menu item
  switch (currentMenuIndex) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Settings");
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("About");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Start game");
      break;
  }
}

void processMenuSelection() {
  inPMS=true;
  
  switch (currentMenuIndex) {
  case 0:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Volume ON/OFF");


  if (digitalRead(13) == HIGH) {
        volume = 1;
    }
        if (volume == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Volume is: ON");
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Volume is: OFF");
        }

  break;

    case 1:
      // Display message "This game was made today" on the LCD screen
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("This game was");
      lcd.setCursor(0, 1);
      lcd.print("made today.");
      break;

    case 2:
      startGame=true;
      lcd.clear();
      break;
  }
    inPMS=false;
}

void inMenu(){
  if(!startGame){
  unsigned long currentMillis = millis();
  
  // Check for joystick button press
  if (digitalRead(Analog_Button_pin) == LOW) {
    // Button pressed - select current menu item
    processMenuSelection();
  }

  // Scroll down menu if joystick is pushed down
  if ((analogRead(Analog_X_pin) < 200 || analogRead(Analog_X_pin) > 800)&& (currentMillis - previousMillis >= 500) && !inPMS) {
    previousMillis = currentMillis;
    currentMenuIndex = (currentMenuIndex + 1) % 3; // Increment menu index
    showMenu();
  }
  }
}

void setup() {
  
  lcd.begin(16, 2); // Set up LCD display
  lcd.clear();  
  pinMode(13,INPUT_PULLUP);
  Serial.begin(9600);
  displayWelcomeMessage();
  pinMode(buzzerPin,OUTPUT);
  pinMode(Analog_X_pin, INPUT);  
  pinMode(Analog_Y_pin, INPUT);
  pinMode(Analog_Button_pin, INPUT_PULLUP);
  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 8);
  ledControl.clearDisplay(0);
  nonBlockingBuzzer();
}

void loop() {
  inMenu();
  if (startGame) {
    playingGame();
    
  }
}


void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Raresh Game");

  unsigned long startTime = millis(); // Record the start time

  // Continue displaying the message for 3 seconds
  while (millis() - startTime < 3000) {
    // Your non-blocking code can go here if needed
  }

  lcd.clear();
}

void playingGame(){
  
  float deltaTime = calculateDeltaTime();
  timer += deltaTime;

  int xValue = 1023 - analogRead(analogXPin);
  int yValue = analogRead(analogYPin);

  if (xValue < 100 && gameSnake.direction[1] == 0) {
    gameSnake.direction[0] = 0;
    gameSnake.direction[1] = -1;
  } else if (xValue > 920 && gameSnake.direction[1] == 0) {
    gameSnake.direction[0] = 0;
    gameSnake.direction[1] = 1;
  } else if (yValue < 100 && gameSnake.direction[0] == 0) {
    gameSnake.direction[0] = -1;
    gameSnake.direction[1] = 0;
  } else if (yValue > 920 && gameSnake.direction[0] == 0) {
    gameSnake.direction[0] = 1;
    gameSnake.direction[1] = 0;
  }

  if (timer > 1000 / updateRate) {
    timer = 0;
    updateGame();
  }

  renderGame();
}
float calculateDeltaTime() {
  float currentTime = millis();
  float deltaTime = currentTime - previousTime;
  previousTime = currentTime;
  return deltaTime;
}

void resetDisplayBuffer() {
  for (int j = 0; j < 8; j++) {
    displayBuffer[j] = 0;
  }
}

void updateGame() {
  resetDisplayBuffer();

  int newHead[2] = {gameSnake.head[0] + gameSnake.direction[0], gameSnake.head[1] + gameSnake.direction[1]};

  if (newHead[0] == 8) {
    newHead[0] = 0;
  } else if (newHead[0] == -1) {
    newHead[0] = 7;
  } else if (newHead[1] == 8) {
    newHead[1] = 0;
  } else if (newHead[1] == -1) {
    newHead[1] = 7;
  }

  for (j = 0; j < gameSnake.length; j++) {
    if (gameSnake.body[j][0] == newHead[0] && gameSnake.body[j][1] == newHead[1]) {
      Serial.println(highScore);
      gameScore=0;
      
      delay(1000);
      gameSnake = {{1, 5}, {{0, 5}, {1, 5}}, 2, {1, 0}};
      gameApple = {(int)random(0, 8), (int)random(0, 8)};
      return;
    }
  }

  if (newHead[0] == gameApple.rowPos && newHead[1] == gameApple.colPos) {
    gameScore++;
    nonBlockingBuzzer();
    if(gameScore>highScore)
      highScore=gameScore;
    gameSnake.length = gameSnake.length + 1;
    gameApple.rowPos = (int)random(0, 8);
    gameApple.colPos = (int)random(0, 8);
  } else {
    removeFirstBodySegment();
  }

  gameSnake.body[gameSnake.length - 1][0] = newHead[0];
  gameSnake.body[gameSnake.length - 1][1] = newHead[1];

  gameSnake.head[0] = newHead[0];
  gameSnake.head[1] = newHead[1];

  for (j = 0; j < gameSnake.length; j++) {
    displayBuffer[gameSnake.body[j][0]] |= 128 >> gameSnake.body[j][1];
  }
  displayBuffer[gameApple.rowPos] |= 128 >> gameApple.colPos;
  printCurrentScoreAndHighScore();
}

void renderGame() {
  for (i = 0; i < 8; i++) {
    ledControl.setRow(0, i, displayBuffer[i]);
  }
}

void removeFirstBodySegment() {
  for (j = 1; j < gameSnake.length; j++) {
    gameSnake.body[j - 1][0] = gameSnake.body[j][0];
    gameSnake.body[j - 1][1] = gameSnake.body[j][1];
  }
}
void printCurrentScoreAndHighScore() {

  unsigned long startTimeMillis = millis(); // Store the start time in milliseconds
while(millis() - startTimeMillis < 100){}
  lcd.setCursor(0, 0);
  lcd.print("Current Score: ");
  lcd.print(gameScore);

  lcd.setCursor(0, 1);
  lcd.print("High Score: ");
  lcd.print(highScore);

}
void nonBlockingBuzzer() {
//to do
}

