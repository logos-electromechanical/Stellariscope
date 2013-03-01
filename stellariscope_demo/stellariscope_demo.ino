#include <SPI.h>

// pin definitions
const int latch     = P1_0;  // latch signal 
const int col0      = P1_2;  // column select bit 0
const int col1      = P1_3;  // column select bit 1
const int col2      = P1_4;  // column select bit 2
const int clkadd0   = P2_0;  // module row select bit 0
const int clkadd1   = P2_1;  // module row select bit 1
const int clkadd2   = P2_2;  // module row select bit 2
const int hcnt_out  = P2_6;  // horizontal count output
const int vcnt_out  = P1_6;  // vertical count output
const int hcnt_in   = P2_5;  // horizontal count return
const int vcnt_in   = P2_4;  // vertical count return

// column sets
const uint8_t null[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// other definitions
#define BRIGHT (0x22)
#define DELAY  (500)

// global variables
uint8_t hcnt = 1;
uint8_t vcnt = 1;

// function prototypes
void writeCol (int column, int red, int grn, int blu);
void writeRow (int row, int red, int grn, int blu);
void selectCol (int col);
void pushBytes (uint8_t *bytes, uint8_t len, int rowBlock);
uint16_t strobeLatch (void);

// program

void setup() {
  // initialize pins
  pinMode (latch, OUTPUT);
  pinMode (col0, OUTPUT);
  pinMode (col1, OUTPUT);
  pinMode (col2, OUTPUT);
  pinMode (clkadd0, OUTPUT);
  pinMode (clkadd1, OUTPUT);
  pinMode (clkadd2, OUTPUT);
  pinMode (hcnt_out, OUTPUT);
  pinMode (vcnt_out, OUTPUT);
  pinMode (hcnt_in, INPUT);
  pinMode (vcnt_in, INPUT);
  digitalWrite (latch, LOW);
  digitalWrite (hcnt_out, HIGH);
  digitalWrite (vcnt_out, HIGH);
  digitalWrite (clkadd0, LOW);
  digitalWrite (clkadd1, LOW);
  digitalWrite (clkadd2, LOW);
  // initialize SPI:
  SPI.begin(); 
}

void loop() {
 int i, j; 
 for (i = 0; i < (8 * hcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, BRIGHT, 0, 0);
   }
 }
 for (i = 0; i < (8 * hcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, 0, BRIGHT, 0);
   }
 }
 for (i = 0; i < (8 * hcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, 0, 0, BRIGHT);
   }
 }
 for (i = 0; i < (8 * hcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, BRIGHT, BRIGHT, BRIGHT);
   }
 }
 for (i = 0; i < (8 * vcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeRow(i, BRIGHT, 0, 0);
   }
 }
 for (i = 0; i < (8 * vcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeRow(i, 0, BRIGHT, 0);
   }
 }
 for (i = 0; i < (8 * vcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeRow(i, 0, 0, BRIGHT);
   }
 }
 for (i = 0; i < (8 * vcnt); i++) {
   for (j = 0; j < DELAY; j++) {
     writeRow(i, BRIGHT, BRIGHT, BRIGHT);
   }
 }
}


void writeCol (int column, int red, int grn, int blu) {
  int i, j;
  for (i = 0; i < vcnt; i++) {
    
  }
  return;
}

void writeRow (int row, int red, int grn, int blu) {
  return;
}

uint16_t strobeLatch (void) {
  static uint8_t h = 1;
  static uint8_t v = 1;
  
  return ((hcnt << 8) | vcnt);

}

void selectCol (int col) {
  int i;
 i = col % 8;
 if (i & 1) {
      digitalWrite(col0, HIGH);
    } else {
      digitalWrite(col0, LOW);
    }
    if (i & 2) {
      digitalWrite(col1, HIGH);
    } else {
      digitalWrite(col1, LOW);
    }
    if (i & 4) {
      digitalWrite(col2, HIGH);
    } else {
      digitalWrite(col2, LOW);
    }
  
}
void pushBytes (uint8_t *bytes, uint8_t len, int rowBlock) {
 int i;
 for (i = 0; i < len; i++) {
  SPI.transfer(bytes[i]); 
 }
 
}
