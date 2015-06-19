#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

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
        bar = bar + 2;
      }

    }
  } else {

    if (buttonState2 == HIGH) {
      digitalWrite(right, LOW);
      digitalWrite(left, HIGH);

      if (digitalRead(left)) {
        if (bar >= 3) {
          bar = bar - 2;
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

  delay(99999999); //TODO: Terug naar beginscherm, hij maakt nu alleen een delay zodat de game niet verder gaat
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




