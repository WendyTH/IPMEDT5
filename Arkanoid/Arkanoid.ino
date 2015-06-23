#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
 
#define melodyPin 3


#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega // 8 on Uno)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

const uint8_t left = 5, right = 4; // Buttons
uint8_t ballx = 5, bally = 14; // De locatie van de bal als het spel begint
int speedx = -1, speedy = -1; // Ball's snelheid en beweging
uint8_t bar = 5; // Bar (het gedeelte dat we bewegen) Dit is de y-as, hij zit op de laagste LED rij
uint8_t board [][32] = {{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Board 1:brick, 0: geen brick
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
uint8_t numBricks = 186; // count of bricks

void setup() {
  matrix.begin();
  pinMode(right, INPUT);

  pinMode(left, INPUT);
 pinMode(3, OUTPUT);//buzzer
}



// Tekent de steentjes (bricks) op het display
void paintBricks() {
  for (uint8_t i = 0; i < 32; i++) {
    for (uint8_t j = 0; j < 32; j++) {
      if (board[i][j] == 1) {
        if (i % 2 == 0)
          matrix.drawPixel(j, i, matrix.Color333(4, 0, 7)); // Consider: hits to break block depending on the colour
        else
          matrix.drawPixel(j, i, matrix.Color333(7, 4, 0));
      }
    }
  }
}

// Teken de bar op het scherm
void paintBar() {
  matrix.drawPixel(bar + 1, 15, matrix.Color333(0, 0, 7)); //(y, x, color)
  matrix.drawPixel(bar + 2, 15, matrix.Color333(0, 0, 7));
  matrix.drawPixel(bar, 15, matrix.Color333(0, 0, 7)); // middelste pixel van de bar
  matrix.drawPixel(bar - 2, 15, matrix.Color333(0, 0, 7));
  matrix.drawPixel(bar - 1, 15, matrix.Color333(0, 0, 7));
}

// Teken het balletje op het scherm
void paintBall() {
  matrix.drawPixel(ballx, bally, matrix.Color333(5, 0, 0));
}

// Checken op collisions met bovenkant, zijkanten en onderkant
void checkCollisions() {
  if (numBricks > 0) { //als de bricks/steentjes meer dan 0 is, doe dit

    // Onderkant
    if (bally == 14) {

      // Kijken of de bar hier is, zo niet dan is speler game over
      if (ballx == bar || ballx == bar + 1 || ballx == bar + 2 || ballx == bar - 1 || ballx == bar - 2) {
        speedy = -1;
      }
      else {
        gameOver();
      }
    }
    // Bovenkant van het scherm
    if (bally == 0) {
      if (board[bally][ballx]) {
        numBricks--;
      }
      speedy = 1;
    }
    // Zijkanten van het scherm
    if ( ballx == 0 || ballx == 31) {
      if (board[bally][ballx]) {
        numBricks--;
        speedy = 1;
      }
      speedx = -speedx;
    }
    // Als bal het board raakt, gaan de bricks weg
    if (ballx != 0 && ballx != 32 && bally != 0 && bally != 32) {
      if (board[bally][ballx]) {
        board[bally][ballx] = 0;
        numBricks--;
        speedy = 1;
      }
    }
    ballx += speedx;
    bally += speedy;
  }
  else win(); //anders (als alle bricks/steentjes op zijn) gewonnen
}

int buttonState = 0;
int buttonState2 = 0;

void loop() {
  matrix.fillScreen(matrix.Color333(0, 0, 0)); //vul het scherm naar zwart om het scherm te hertekenen
  paintBricks();
  paintBar();
  paintBall();

  buttonState = digitalRead(right);
  buttonState2 = digitalRead(left);

  if (buttonState == HIGH) {
    digitalWrite(right, HIGH);

    if (digitalRead(right)) {
      if (bar <= 28) {
        bar++;
      }

    }
  } else {

    if (buttonState2 == HIGH) {
      digitalWrite(right, LOW);
      digitalWrite(left, HIGH);

      if (digitalRead(left)) {
        if (bar >= 3) {
          bar--;
        }

      }
    }
  }
  //  if (!digitalRead(left)) {
  //    if (bar >= 3) {
  //      bar--;
  //    }
  //  }
  //  if (!digitalRead(right)) {
  //    if (bar <= 28) {
  //      bar++;
  //    }
  //
  //  }
  checkCollisions();
  delay(200);
}

//Als speler game over is, toon dit op scherm
void gameOver() {


 
  
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0)); //clear het scherm

  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(7, 7, 0));
  matrix.print('A');
  matrix.setTextColor(matrix.Color333(4, 7, 0));
  matrix.print('M');
  matrix.setTextColor(matrix.Color333(0, 7, 0));
  matrix.print('E');

  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.print('O');
  matrix.setTextColor(matrix.Color333(0, 4, 7));
  matrix.print('V');
  matrix.setTextColor(matrix.Color333(0, 0, 7));
  matrix.print('E');
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.print("R");
  matrix.setTextColor(matrix.Color333(7, 0, 4));
  matrix.print("*");
sing(1);
 delay(99999999);
  //TODO: Terug naar beginscherm, hij maakt nu alleen een delay zodat de game niet verder gaat
}
//Als speler gewonnen is, toon dit op scherm
void win() {
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.print('Y');
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.print('O');
  matrix.setTextColor(matrix.Color333(7, 7, 0));
  matrix.print('U');

  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.print('W');
  matrix.setTextColor(matrix.Color333(0, 4, 7));
  matrix.print('O');
  matrix.setTextColor(matrix.Color333(0, 0, 7));
  matrix.print('N');
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.print("!");

  delay(99999999); //TODO: Terug naar beginscherm, hij maakt nu alleen een delay zodat de game niet verder gaat
}
  int melody[] = {
 NOTE_G4, NOTE_G4,NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_DS5, NOTE_AS4, NOTE_FS4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_G5, NOTE_G4, NOTE_G4, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_E5, 0, NOTE_GS4, NOTE_CS5, NOTE_C5, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_AS4, 0, NOTE_DS4, NOTE_FS4, NOTE_DS4, NOTE_FS4, NOTE_AS4, NOTE_G4, NOTE_AS4, NOTE_D5, NOTE_G5, NOTE_G4, NOTE_G4, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_E5, 0, NOTE_GS4, NOTE_CS5, NOTE_C5, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_AS4, 0, NOTE_DS4, NOTE_FS4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4

};
int tempo[] = {
   4, 4, 4, 6, 16, 4, 6, 16, 2, 4, 4, 4, 6, 16, 4, 6, 16, 2, 4, 6, 16, 4, 6, 16, 16, 16, 8, 8, 8, 4, 6, 16, 16, 16, 8, 8, 8, 4, 6, 16, 4, 6, 16, 2, 4, 6, 16, 4, 6, 16, 16, 16, 8, 8, 8, 4, 6, 16, 16, 16, 8, 8, 8, 4, 6, 16, 4, 6, 16, 2,
};


int song = 0;
void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
 
 
   if (song==1) {
 
    Serial.println(" 'Imperial March'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
 
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 800 / tempo[thisNote];
 
      buzz(melodyPin, melody[thisNote], noteDuration);
 
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);
 
    }
  }
}
void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
}


