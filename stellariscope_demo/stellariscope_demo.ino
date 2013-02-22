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

void setup() {
  pinMode (latch, OUTPUT);
  pinMode (col0, OUTPUT);
  pinMode (col1, OUTPUT);
  pinMode (col2, OUTPUT);
  pinMode (clkadd0, OUTPUT);
  pinMode (clkadd1, OUTPUT);
  pinMode (clkadd2, OUTPUT);
  pinMode (hcnt_out, OUTPUT);
  pinMode (vcnt_out, OUTPUT);
  pinMode (hcnt_in, OUTPUT);
  pinMode (vcnt_in, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
}

void loop() {
  int i, j;
  digitalWrite(clkadd0, LOW);
  digitalWrite(clkadd1, LOW);
  digitalWrite(clkadd2, LOW);
  for (i = 0; i < 8; i++) {
    digitalWrite(latch, LOW);
    for (j = 0; j < 18; j++) {
      SPI.transfer(0xaa);
    }
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
    digitalWrite(latch, HIGH);
  }
}
