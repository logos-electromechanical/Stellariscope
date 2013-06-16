#include <SPI.h>

// pin definitions
const int latch     = P1_0;  // latch signal 
const int col0      = P2_3;  // column select bit 0
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
const uint8_t off[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// typedef

struct dyad {
  uint8_t a;
  uint8_t b;
  uint8_t c;
};

// other definitions
#define BRIGHT (0x555)
#define DELAY  (25)

// global variables
uint8_t hcnt = 2;
uint8_t vcnt = 1;

// function prototypes
void writeCol (int column, int red, int grn, int blu);
void writeRow (int row, int red, int grn, int blu);
void selectCol (int col);
void pushBytes (const uint8_t *bytes, uint8_t len, int rowBlock);
void selectRow (int rowBlock);
uint16_t strobeLatch (void);
struct dyad getDyad (uint16_t a, uint16_t b);

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
  digitalWrite (hcnt_out, LOW);
  digitalWrite (vcnt_out, LOW);
  digitalWrite (clkadd0, LOW);
  digitalWrite (clkadd1, LOW);
  digitalWrite (clkadd2, LOW);
  // initialize SPI:
  SPI.begin(); 
  //SPI.setBitOrder(LSBFIRST);
  // initialize serial
  //Serial.begin(9600);
}

void loop() {
 int i, j; 
 for (i = 0; i < 8; i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, BRIGHT, 0, 0);
   }
 }
 for (i = 0; i < 8; i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, 0, BRIGHT, 0);
   }
 }
 for (i = 0; i < 8; i++) {
   for (j = 0; j < DELAY; j++) {
     writeCol(i, 0, 0, BRIGHT);
   }
 }
 for (i = 0; i < 8; i++) {
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
  int i, j, k, l;
  struct dyad mine;
  //Serial.print("Column: ");
  //Serial.println(column);
  for (i = 0; i < 8; i++) {
    selectCol(i);
    for (j = 0; j < vcnt; j++) {
      selectRow(j);
      for (l = hcnt; l > 0; l--) {  
        if (i == (l*column)) {
          mine = getDyad(blu, blu);
          for (k = 0; k < 4; k++) {
            SPI.transfer(mine.a);
            SPI.transfer(mine.b);
            SPI.transfer(mine.c);
          }
          mine = getDyad(grn, grn);
          for (k = 0; k < 4; k++) {
            SPI.transfer(mine.a);
            SPI.transfer(mine.b);
            SPI.transfer(mine.c);
          }
          mine = getDyad(red, red);
          for (k = 0; k < 4; k++) {
            SPI.transfer(mine.a);
            SPI.transfer(mine.b);
            SPI.transfer(mine.c);
          }
        } else {
          pushBytes(off, 36, j);
        }
      }
    }
    strobeLatch();
  }
  
  return;
}

void writeRow (int row, int red, int grn, int blu) {
  int i, j, k, l;
  struct dyad mine;
  selectRow(0);
  for (i = 0; i < 8; i++) {
    selectCol(i);
    for (j = 0; j < vcnt; j++) {
      selectRow(j);
      for (k = hcnt; k > 0; k--) {    
        for (l = 0; l < 4; l++) {
          if ((row-(2*l)) == 1) {
            mine = getDyad(0, blu);
          } else if ((row-(2*l)) == 0) {
            mine = getDyad(blu, 0);
          } else {
            mine = getDyad(0, 0);
          }
          SPI.transfer(mine.a);
          SPI.transfer(mine.b);
          SPI.transfer(mine.c);
        }
        for (l = 3; l >= 0 ; l--) {
          if ((row-(2*l)) == 0) {
            mine = getDyad(0, grn);
          } else if ((row-(2*l)) == 1) {
            mine = getDyad(grn, 0);
          } else {
            mine = getDyad(0, 0);
          }
          SPI.transfer(mine.a);
          SPI.transfer(mine.b);
          SPI.transfer(mine.c);
        }
        for (l = 0; l < 4; l++) {
          if ((row-(2*l)) == 1) {
            mine = getDyad(0, red);
          } else if ((row-(2*l)) == 0) {
            mine = getDyad(red, 0);
          } else {
            mine = getDyad(0, 0);
          }
          SPI.transfer(mine.a);
          SPI.transfer(mine.b);
          SPI.transfer(mine.c);
        }
      }
    }
    strobeLatch();
  }
  return;
}

uint16_t strobeLatch (void) {
  static uint8_t h = 1;
  static uint8_t v = 1;
  uint8_t lastv, lasth;
  
  //lastv = digitalRead(vcnt_in);
  //lasth = digitalRead(hcnt_in);
  digitalWrite(latch, HIGH);
  digitalWrite(latch, LOW);
  /*if (digitalRead(vcnt_in) == lastv) {
    v++;
  } else {
  }
  if (digitalRead(hcnt_in) == lasth) {
    h++;
  } else {
  }*/
  
  return ((hcnt << 8) | vcnt);

}

void selectCol (int col) {
  int i;
 i = col;
 if (i & 0x01) {
      digitalWrite(col0, HIGH);
    } else {
      digitalWrite(col0, LOW);
    }
    if (i & 0x02) {
      digitalWrite(col1, HIGH);
    } else {
      digitalWrite(col1, LOW);
    }
    if (i & 0x04) {
      digitalWrite(col2, HIGH);
    } else {
      digitalWrite(col2, LOW);
    }
    delay(1);
  
}

void pushBytes (const uint8_t *bytes, uint8_t len, int rowBlock) {
 int i;
 selectRow(rowBlock);
 for (i = 0; i < len; i++) {
  SPI.transfer(bytes[i]); 
 }
 
}


void selectRow (int rowBlock) {
 rowBlock = 0;
 if (rowBlock & 1) {
   digitalWrite(clkadd0, HIGH);
 } else {
    digitalWrite(clkadd0, LOW);
 }
 if (rowBlock & 2) {
   digitalWrite(clkadd1, HIGH);
 } else {
   digitalWrite(clkadd1, LOW);
 }
 if (rowBlock & 4) {
   digitalWrite(clkadd2, HIGH);
 } else {
    digitalWrite(clkadd2, LOW);
 }
}

struct dyad getDyad (uint16_t a, uint16_t b) {
  struct dyad val;
  
  val.a = (uint8_t)(a >> 4);
  val.b = (uint8_t)(((a << 4) & 0xf0) | ((b >> 8) & 0x0f));
  val.c = (uint8_t)(b & 0xff);
  
  return val;
}
