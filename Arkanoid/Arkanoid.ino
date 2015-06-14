#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

const uint8_t left = 5, right = 4; // Buttons
uint8_t ballx = 16, bally = 14; // Current location of the ball
int speedx = -1, speedy = -1; // Ball's speed & direction
uint8_t vaus = 15; // (The thing that we move) Vause's left pixel location
uint8_t board [][31] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // Board 1:brick, 0: no brick
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                      };
uint8_t numBricks = 186; // count of bricks

void setup() {
matrix.begin();
 pinMode(right, INPUT);
 digitalWrite(right, HIGH);
 pinMode(left, INPUT);
 digitalWrite(left, HIGH);

}

/* Paints the bricks in the display*/
void paintBricks() {
  for(uint8_t i = 0; i<32; i++) {
    for(uint8_t j = 0; j<32; j++){
      if(board[i][j] == 1){
        if(i % 2 == 0)
          matrix.drawPixel(j,i,matrix.Color333(1,1,0)); // Consider: hits to break block depending on the colour
        else
          matrix.drawPixel(j,i, matrix.Color333(5, 5, 1));
      }
    }
  }
}

/* Paints Vaus in the display*/
void paintVaus() {
  matrix.drawPixel(vaus, 15, matrix.Color333(153,50,204));
  matrix.drawPixel(vaus + 1,15, matrix.Color333(153,50,204));
  matrix.drawPixel(vaus + 2,15, matrix.Color333(153,50,204));
  matrix.drawPixel(vaus - 1,15, matrix.Color333(153,50,204));
  matrix.drawPixel(vaus - 2,15, matrix.Color333(153,50,204));
}

/* Paints the ball in the display*/
void paintBall() {
 matrix.drawPixel(ballx, bally, matrix.Color333(5, 0, 0)); 
}

/* Checks the collisions*/
void checkCollisions() {
  if(numBricks > 0) {
    // Bottom 
    if(bally == 14) {
      // Check if Vaus is here
      if(ballx == vaus || ballx == vaus + 1) {
        speedy = -1;
	if(ballx == vaus)
	  speedx = -1;
	else speedx = 1;
      } else gameOver();
    }
    // Top
    if(bally == 0){
      if(board[bally][ballx]) {
	board[bally][ballx] = 0;
	numBricks--;
      }
      speedy = 1;
    }
    // Side walls
    if( ballx == 0 || ballx == 31){
      if(board[bally][ballx]) {
        board[bally][ballx] = 0;
	numBricks--;
	speedy = 1;
      }
      speedx = -speedx;
    }
    // Everything else
    if(ballx != 0 && ballx != 31 && bally != 0 && bally != 31){
      if(board[bally][ballx]) {
        board[bally][ballx] = 0;
        numBricks--;
        speedy = 1;
      }
    }
    ballx += speedx;
    bally += speedy;
  } else win();
}

void gameOver() {
    matrix.setTextColor(matrix.Color333(7,0,0));
    matrix.print('G');
    matrix.setTextColor(matrix.Color333(7,4,0)); 
    matrix.print('A');
    matrix.setTextColor(matrix.Color333(7,7,0));
    matrix.print('M');
    matrix.setTextColor(matrix.Color333(4,7,0)); 
    matrix.print('E');
    
    matrix.setCursor(1, 9);   // next line
    matrix.setTextColor(matrix.Color333(0,7,7)); 
    matrix.print('O');
    matrix.setTextColor(matrix.Color333(0,4,7)); 
    matrix.print('V');
    matrix.setTextColor(matrix.Color333(0,0,7));
    matrix.print('E');
    matrix.setTextColor(matrix.Color333(4,0,7)); 
    matrix.print("R");
}

void win() {
    matrix.setTextColor(matrix.Color333(7,0,0));
    matrix.print('Y');
    matrix.setTextColor(matrix.Color333(7,4,0)); 
    matrix.print('O');
    matrix.setTextColor(matrix.Color333(7,7,0));
    matrix.print('U');
    
    matrix.setCursor(1, 9);   // next line
    matrix.setTextColor(matrix.Color333(0,7,7)); 
    matrix.print('W');
    matrix.setTextColor(matrix.Color333(0,4,7)); 
    matrix.print('O');
    matrix.setTextColor(matrix.Color333(0,0,7));
    matrix.print('N');
    matrix.setTextColor(matrix.Color333(4,0,7)); 
    matrix.print("!");
}

void loop() {
   //matrix.clear();
   paintBricks();
   paintVaus();
   paintBall();
   
  // matrix.writeDisplay();
   if(!digitalRead(left)){
     if(vaus > 0)
       vaus--; 
   }
   if(!digitalRead(right)){
     if(vaus + 1 < 11)
       vaus++;
   }
   checkCollisions();
   delay(150); 

}
