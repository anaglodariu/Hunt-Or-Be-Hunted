#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// pins for matrix module
#define MAX_DEVICES 1  // Number of MAX7219 modules you're using
#define DATA_IN_PIN 11   // Pin connected to MAX7219 data in
#define CLK_PIN 13       // Pin connected to MAX7219 clock
#define CS_PIN 10      // Pin connected to MAX7219 chip select

// pins for joystick
#define VERT_PIN A0
#define HORZ_PIN A1
#define SW_PIN  2

// pins for buzzer
#define BUZZER_PIN 9

const unsigned char epd_bitmap_sword [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 
	0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 
	0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 
	0x00, 0x00, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
	0x00, 0x0c, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x04, 0x30, 0x00, 0x00, 0x02, 0x60, 0x00, 0x00, 
	0x03, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0e, 0x60, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
	0x38, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_chest [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8, 0x18, 0x41, 0x82, 0x18, 
	0x18, 0x41, 0x82, 0x18, 0x18, 0x41, 0x82, 0x18, 0x18, 0x41, 0x82, 0x18, 0x18, 0x41, 0x82, 0x18, 
	0x18, 0x47, 0xe2, 0x18, 0x1f, 0xfc, 0x3f, 0xf8, 0x1f, 0xfc, 0x3f, 0xf8, 0x18, 0x47, 0xe2, 0x18, 
	0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 
	0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 
	0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 0x18, 0x40, 0x02, 0x18, 0x1f, 0xff, 0xff, 0xf8, 
	0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_monster [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 
	0x00, 0x30, 0x0c, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80, 
	0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x03, 0xc1, 0x83, 0xc0, 0x07, 0xc9, 0x93, 0xe0, 
	0x3f, 0xd5, 0xab, 0xfc, 0x3f, 0xc1, 0x83, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x38, 0x7f, 0xfe, 0x1c, 
	0x38, 0x7f, 0xfe, 0x1c, 0x38, 0x3f, 0xfc, 0x1c, 0x38, 0x0f, 0xf0, 0x1c, 0x38, 0x03, 0xc0, 0x1c, 
	0x38, 0x07, 0xe0, 0x1c, 0x38, 0x1f, 0xf8, 0x1c, 0x38, 0x1f, 0xf8, 0x1c, 0x38, 0x1f, 0xf8, 0x1c, 
	0x38, 0x1f, 0xf8, 0x1c, 0x38, 0x1f, 0xf8, 0x1c, 0x38, 0x1c, 0x38, 0x1c, 0x38, 0x1c, 0x38, 0x1c, 
	0x38, 0x1c, 0x38, 0x1c, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x1c, 0x38, 0x00
};

const unsigned char epd_bitmap_princess [] PROGMEM = {
	0x00, 0x1f, 0xf0, 0x00, 0x00, 0x30, 0x0e, 0x00, 0x00, 0x60, 0x03, 0x00, 0x00, 0x80, 0x00, 0x80, 
	0x00, 0x80, 0x00, 0x80, 0x00, 0x8f, 0xfc, 0x80, 0x00, 0x90, 0x02, 0x80, 0x00, 0x92, 0x0a, 0x80, 
	0x00, 0x92, 0x0a, 0x80, 0x00, 0x92, 0x0a, 0x80, 0x01, 0x10, 0x82, 0x80, 0x01, 0x10, 0x62, 0x80, 
	0x01, 0x08, 0x04, 0x80, 0x01, 0x3f, 0x3e, 0x80, 0x01, 0x45, 0x19, 0x80, 0x01, 0x48, 0xe5, 0x80, 
	0x01, 0x48, 0x05, 0x80, 0x01, 0x38, 0x06, 0x80, 0x01, 0x24, 0x0a, 0x80, 0x02, 0x17, 0xfa, 0x80, 
	0x02, 0x0f, 0xfc, 0x80, 0x02, 0x10, 0x04, 0x80, 0x02, 0x10, 0x04, 0x80, 0x02, 0x20, 0x02, 0x80, 
	0x02, 0x20, 0x02, 0x80, 0x02, 0x20, 0x02, 0x80, 0x02, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 
	0x00, 0xc0, 0x03, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x7f, 0xfe, 0x00
};

const unsigned char epd_bitmap_hero [] PROGMEM = {
	0x00, 0x2a, 0x80, 0x00, 0x00, 0x55, 0x40, 0x00, 0x00, 0xa8, 0x20, 0x00, 0x01, 0x53, 0x90, 0x00, 
	0x01, 0x24, 0x48, 0x00, 0x00, 0x98, 0x28, 0x00, 0x00, 0xa0, 0x08, 0x00, 0x00, 0x20, 0x10, 0x00, 
	0x00, 0x48, 0x48, 0x00, 0x00, 0x48, 0x48, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x20, 0x10, 0x00, 
	0x00, 0x13, 0x20, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x07, 0xa0, 0x00, 0x00, 0x08, 0x50, 0x00, 
	0x00, 0x14, 0x60, 0x00, 0x00, 0x13, 0x90, 0x00, 0x00, 0x21, 0x08, 0x00, 0x00, 0x28, 0x28, 0x00, 
	0x00, 0x49, 0x24, 0x00, 0x00, 0xc8, 0x24, 0x00, 0x00, 0x8f, 0xe4, 0x00, 0x00, 0x78, 0x3c, 0x00, 
	0x00, 0x28, 0x28, 0x00, 0x00, 0x19, 0x30, 0x00, 0x00, 0x09, 0x20, 0x00, 0x00, 0x09, 0x20, 0x00, 
	0x00, 0x05, 0x40, 0x00, 0x00, 0x05, 0x40, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x07, 0xc0, 0x00
};


const unsigned char epd_bitmap_rip [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x01, 0x81, 0x81, 0x80, 
	0x01, 0x81, 0x81, 0x80, 0x06, 0x01, 0x80, 0x60, 0x06, 0x01, 0x80, 0x60, 0x06, 0x01, 0x80, 0x60, 
	0x18, 0x01, 0x80, 0x18, 0x18, 0x01, 0x80, 0x18, 0x18, 0x01, 0x80, 0x18, 0x18, 0x07, 0xe0, 0x18, 
	0x18, 0x01, 0x80, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 
	0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 
	0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 
	0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x1f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8
};



// variables for calibrating joystick
int horzMin = 15;
int horzMid = 543;
int horzMax = 1023;
int vertMin = 15;
int vertMid = 556;
int vertMax = 1023;

// world
char visited[8][8];

// position of the hero
byte playerX = 0;
byte playerY = 0;

// position of the monster
byte monsterX = 0;
byte monsterY = 0;

// position of the chest
byte chestX = 0;
byte chestY = 0;

// last move before hitting monster
byte lastMove = 0;

// is hero armed
bool armed = false;

// counter for timer
volatile unsigned long int timer = 0;
// counter for flickering led on current position of player
volatile int timer1 = 0;
// print seconds to the OLED screen
volatile int lastSecond = 0;
// time in seconds
volatile int seconds = 0;

MD_MAX72XX mx = MD_MAX72XX(MAX_DEVICES, DATA_IN_PIN, CLK_PIN, CS_PIN);

volatile bool hitMonster = false;
unsigned long interval = 10000;
unsigned long current = 0;
unsigned long prev = 0;
bool flag = false;
byte direction = 4;

// function prototypes
byte getDirection(int horz, int vert);
void createWorld();
void drawWorld();
void setTimer0();
void initOled();
void displayOnScreen();
void dangerZone();
void chestZone();
void checkHazard();
void checkLoot();
void heroDies();
void wonGame();

void(* reset) (void) = 0;

bool showSword = false;
bool showPrincess = false;
bool showRip = false;
bool showChest = false;
bool showMonster = false;
bool showHero = false;
bool killed = false;
bool won = false;
unsigned long int momentWhenKilled = 0;
unsigned long int momentWhenWon = 0;

void heroDies() {
  showRip = true;
  for (int i = 0; i <= 1000; i++) {
    if (timer1 == i) {
      tone(BUZZER_PIN, 2000 - i * 2, 1);
    }
  }
}

void wonGame() {
  if (timer1 <= 50) {
    tone(BUZZER_PIN, 1397, 50);
  } else if (timer1 <= 100) {
    tone(BUZZER_PIN, 1397, 50);
  } else if (timer1 <= 150) {
    tone(BUZZER_PIN, 1397, 100);
  } else if (timer1 <= 250) {
    tone(BUZZER_PIN, 1175, 100);
  } else if (timer1 <= 350) {
    tone(BUZZER_PIN, 1319, 100);
  } else if (timer1 <= 450) {
    tone(BUZZER_PIN, 1397, 400);
  }
}

void displayOnScreen() {
  // clear the buffer
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(2);

  display.setCursor(96,15);
  display.print(seconds);

  if (showPrincess) {
    display.drawBitmap(0, 0, epd_bitmap_hero, 32, 32, WHITE);
    display.drawBitmap(32, 0, epd_bitmap_princess, 32, 32, WHITE);
    won = true;
  } else if (showSword) {
    display.drawBitmap(0, 0, epd_bitmap_hero, 32, 32, WHITE);
    display.drawBitmap(32, 0, epd_bitmap_sword, 32, 32, WHITE);
    showSword = false;
  } else if (showRip) {
    display.drawBitmap(0, 0, epd_bitmap_rip, 32, 32, WHITE);
    showRip = true;
  } else if (showHero && !showMonster && !showChest) {
    display.drawBitmap(0, 0, epd_bitmap_hero, 32, 32, WHITE);
    showHero = false;
  } else if (showHero && !showMonster && showChest) {
    display.drawBitmap(0, 0, epd_bitmap_hero, 32, 32, WHITE);
    display.drawBitmap(32, 0, epd_bitmap_chest, 32, 32, WHITE);
    showChest = false;
    showHero = false;
  } else if (showMonster && showHero && !showChest) {
    display.drawBitmap(0, 0, epd_bitmap_hero, 32, 32, WHITE);
    display.drawBitmap(32, 0, epd_bitmap_monster, 32, 32, WHITE);
    display.display();
    display.clearDisplay();
    showHero = false;
    showMonster = false;
  } else if (showMonster && showHero && showChest) {
    display.drawBitmap(0, 0, epd_bitmap_hero, 32, 32, WHITE);
    display.drawBitmap(32, 0, epd_bitmap_monster, 32, 32, WHITE);
    display.drawBitmap(64, 0, epd_bitmap_chest, 32, 32, WHITE);
    display.display();
    display.clearDisplay();
    showChest = false;
    showHero = false;
    showMonster = false;
  }
  display.display();
}

void initOled() {
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // don't proceed, loop forever
  }
}

void setTimer0() {
  TCCR0A=(1<<WGM01);    //Set the CTC mode   
  OCR0A=0xF9; //Value for ORC0A for 1ms 
  
  TIMSK0|=(1<<OCIE0A);   //Set  the interrupt request
  sei(); //Enable interrupt
  
  TCCR0B|=(1<<CS01);    //Set the prescale 1/64 clock
  TCCR0B|=(1<<CS00);
}

void setup() {
  mx.begin();  // Initialize the MAX7219 library

  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SW_PIN), handleSwPress, FALLING);

  // initialize the random number generator with 
  // an unpredictable seed
  randomSeed(analogRead(A3));

  mx.control(MD_MAX72XX::INTENSITY, 0);
  mx.setPoint(7 - playerX, 7 - playerY, 1);

  Serial.begin(9600);
  setTimer0();
  initOled();

  createWorld();
  dangerZone();
}

void loop() {
  if (!killed && timer >= 60000) { // after 1 minute
    timer = 0;
    killed = true;
  } else if (killed && momentWhenKilled + 1000 < timer) {
    reset();
  } else if (won && momentWhenWon + 1000 < timer) {
    reset();
  }

  if (timer1 >= 1000) {
    timer1 = 0;
    flag = flag == false ? true : false;
  }

  int vert = analogRead(VERT_PIN);
  int horz = analogRead(HORZ_PIN);
  if (!flag) {
    direction = getDirection(horz, vert);
    if (direction == 0) {
      // move player up
      Serial.println("up");
      if (playerX < 7) {
        playerX++;
        lastMove = 0;
      }
    } else if (direction == 1) {
      // move player down
      Serial.println("down");
      if (playerX > 0) {
        playerX--;
        lastMove = 1;
      }
    } else if (direction == 2) {
      // move player right
      Serial.println("right");
      if (playerY < 7) {
        playerY++;
        lastMove = 2;
      }
    } else if (direction == 3) {
      // move player left
      Serial.println("left");
      if (playerY > 0) {
        playerY--;
        lastMove = 3;
      }
    } else {
      Serial.println("wanna shot sth");
      if (!armed && hitMonster) {
        hitMonster = false;
        // die
        killed = true;
        momentWhenKilled = timer;
        Serial.println("not armed");
      } else if (hitMonster) {
        hitMonster = false;
        Serial.println("hit monster");
        Serial.println(lastMove);
        if (lastMove == 0) {
          // hit up
          Serial.println("last move up");
          if (playerX < 7) {
            if (monsterX == playerX + 1 && monsterY == playerY) {
              Serial.println("hit up");
              showPrincess = true;
              momentWhenWon = timer;
            } else {
              // miss and die
              killed = true;
              momentWhenKilled = timer;
              Serial.println("hit up and miss");
            }
          }
        } else if (lastMove == 1) {
          // hit down
          Serial.println("last move down");
          if (playerX > 0) {
            if (monsterX == playerX - 1 && monsterY == playerY) {
              Serial.println("hit down");
              showPrincess = true;
              momentWhenWon = timer;
            } else {
              killed = true;
              momentWhenKilled = timer;
              Serial.println("hit down and miss");
            }
          } 
        } else if (lastMove == 2) {
          // hit right
          Serial.println("last move right");
          if (playerY < 7) {
            if (monsterX == playerX && monsterY == playerY + 1) {
              Serial.println("hit right");
              showPrincess = true;
              momentWhenWon = timer;
            } else {
              killed = true;
              momentWhenKilled = timer;
              Serial.println("hit right and miss");
            }
          }
        } else if (lastMove == 3) {
          // hit left
          Serial.println("last move left");
          if (playerY > 0) {
            if (monsterX == playerX && monsterY == playerY - 1) {
              Serial.println("hit left");
              showPrincess = true;
              momentWhenWon = timer;
            } else {
              killed = true;
              momentWhenKilled = timer;
              Serial.println("hit left and miss");
            }
          }
        }
      }
    }
    visited[playerX][playerY] = 1;
    flag = flag == false ? true : false;
  }

  if (won) wonGame();
  if (killed) heroDies();
  drawWorld();
  dangerZone();
  chestZone();
  checkHazard();
  checkLoot();
  displayOnScreen();
  showHero = true;
}

// center vert 512 horz 512
// left vert 512 horz 0 - 3
// down vert 1023 horz 512 - 1
// up vert 0 horz 512 - 0
// right vert 512 horz 1023 - 2
byte getDirection(int horz, int vert) {
  int sum = horz + vert;
  if (sum > 1523) {
    if (horz > vert) {
      return 2; // right
    } else if (vert > horz) {
      return 1; // down
    }
  } else if (sum < 700) {
    if (horz > vert) {
      return 0; // up
    } else if (vert > horz) {
      return 3; // left
    }
  }
  return 4; // center
}

void handleSwPress() {
  hitMonster = true;
}

void drawWorld() {
  // light the leds that were already visited
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i != playerX && j != playerY) {
        mx.setPoint(7 - i, 7 - j, visited[i][j]); 
      }
    }
  }

  if (timer1 >= 500 && timer1 < 2000) {
    mx.setPoint(7 - playerX, 7 - playerY, 1);
  } else {
    mx.setPoint(7 - playerX, 7 - playerY, 0);
  }
}

void createWorld() {
  // initialize world
  // add chest
  chestX = random(3, 8);
  chestY = random(3, 8);
  Serial.println(chestX);
  Serial.println(chestY);
  // create one monster
  while ((monsterX == 0 && monsterY == 0) || 
    (monsterX == chestX && monsterY == chestY)) {
    monsterX = random(0, 8);
    monsterY = random(0, 8);
  }
  Serial.println(monsterX);
  Serial.println(monsterY);
}

void dangerZone() {
  if (won) return;
  if (killed) return;
  if ((playerX + 1 == monsterX && playerY == monsterY) ||
      (playerX - 1 == monsterX && playerY == monsterY) ||
      (playerY + 1 == monsterY && playerX == monsterX) ||
      (playerY - 1 == monsterY && playerX == monsterX)) {
    
    if (timer1 <= 50) {
      tone(BUZZER_PIN, 6000, 50);
    } else if (timer1 <= 100) {
      tone(BUZZER_PIN, 5000, 50);
    } else if (timer1 <= 150) {
      tone(BUZZER_PIN, 4000, 50);
    }
    showMonster = true;
  }
}

void chestZone() {
  if (won) return;
  if (armed) return;
  if ((playerX + 1 == chestX && playerY == chestY) ||
      (playerX - 1 == chestX && playerY == chestY) ||
      (playerY + 1 == chestY && playerX == chestX) ||
      (playerY - 1 == chestY && playerX == chestX)) {
    
    if (timer1 <= 50) {
      tone(BUZZER_PIN, 1000, 50);
    } else if (timer1 <= 100) {
      tone(BUZZER_PIN, 2000, 50);
    } else if (timer1 <= 150) {
      tone(BUZZER_PIN, 3000, 50);
    }
    showChest = true;
  }
}

void checkHazard() {
  if (!killed && playerX == monsterX && playerY == monsterY) {
    killed = true;
    momentWhenKilled = timer;
    showRip = true;
  } else if (killed && playerX == monsterX && playerY == monsterY) {
    heroDies();
  }
}

void checkLoot() {
  if (!armed && playerX == chestX && playerY == chestY) {
    armed = true;
    showSword = true;
  } else if (armed && playerX == chestX && playerY == chestY) {
    showSword = true;
  }
}

ISR(TIMER0_COMPA_vect) {    //This  is the interrupt request
  timer++;
  timer1++;
  seconds = timer / 1000;
  if (seconds != lastSecond) {
    lastSecond = seconds;
  }
}
