class ArkanoidGame {
    Control contr;  // Instance of the Control class for input handling

    // Structure defining properties of the Arkanoid game
    struct Arkanoid {
      // Constants for game logic
      #define BALL_SIZE 4
      #define BLOCK_NUMBER 16
      #define PAD_WEIGHT 30
      #define PAD_HEIGHT 5

      int ball[2];  // positionX, positionY
      int vector[2];  // vectorX, vectorY
      int block[BLOCK_NUMBER][3];  // positionX, positionY, Size
      int pad;  // Paddle position
      int points;  // Player's score
      bool gameOver;  // Flag indicating game over state

      // Constructor for initializing the game state
      Arkanoid() : pad(49), points(0), gameOver(true) {
        ball[0] = 60;
        ball[1] = 40;
        vector[0] = 1;
        vector[1] = 1;

        // Initialize block positions and sizes
        for (int i = 0; i < BLOCK_NUMBER; i++) {
          block[i][0] = i * 10;
          block[i][1] = 0;
          block[i][2] = 8;
        }
      }
    };

public:
    // Main function to run the Arkanoid game
    void main() {
      Arkanoid arkanoid;
      while (arkanoid.gameOver) {
        display.clearDisplay();
        gameControl(arkanoid);
        display.display();
      }
    }

private:
    // Function to control the game logic
    void gameControl(Arkanoid& arkanoid) {
      // Draw the ball, paddle, and blocks on the display
      display.drawCircle(arkanoid.ball[0], arkanoid.ball[1], BALL_SIZE, WHITE);
      display.fillRect(arkanoid.pad, 55, 30, 5, WHITE);

      for (int i = 0; i < BLOCK_NUMBER; i++) {
        display.fillRect(arkanoid.block[i][0], arkanoid.block[i][1], arkanoid.block[i][2], arkanoid.block[i][2], WHITE);
      }

      // Check for collisions with blocks
      for (int i = 0; i < BLOCK_NUMBER; i++) {
        if (arkanoid.ball[0] + BALL_SIZE > arkanoid.block[i][0] &&
            arkanoid.ball[0] < arkanoid.block[i][0] + arkanoid.block[i][2] * 2 &&
            arkanoid.ball[1] + BALL_SIZE > arkanoid.block[0][1] &&
            arkanoid.ball[1] < arkanoid.block[i][1] + arkanoid.block[i][2] * 2) {
          arkanoid.vector[1] = -arkanoid.vector[1];
          arkanoid.block[i][2] = 0;  // Set block size to 0 to indicate it's destroyed
        }
      }

      // Move the ball
      arkanoid.ball[0] = arkanoid.ball[0] + arkanoid.vector[0];

      // Bounce off the walls
      if (arkanoid.ball[0] > 123 || arkanoid.ball[0] < 5) {
        arkanoid.vector[0] = -arkanoid.vector[0];
      }

      arkanoid.ball[1] = arkanoid.ball[1] + arkanoid.vector[1];

      // Bounce off the ceiling
      if (arkanoid.ball[1] < 5) {
        arkanoid.vector[1] = -arkanoid.vector[1];
      }

      // Check if the ball hits the paddle
      if (arkanoid.ball[1] == 50 && arkanoid.ball[0] > arkanoid.pad && arkanoid.ball[0] < arkanoid.pad + PAD_WEIGHT) {
        arkanoid.vector[1] = -arkanoid.vector[1];
        arkanoid.points++;
      }

      // Check if the ball goes below the paddle (game over condition)
      if (arkanoid.ball[1] > 64) {
        gameOver(arkanoid);
      }

      // Move the paddle based on joystick input
      if (contr.Jostick(HOR) == LEFT) {
        arkanoid.pad += 3;
        if (arkanoid.pad > 97) {
          arkanoid.pad = 97;
        }
      }

      if (contr.Jostick(HOR) == RIGHT) {
        arkanoid.pad -= 3;
        if (arkanoid.pad < 1) {
          arkanoid.pad = 1;
        }
      }
    }

    // Function to reset the game state
    void reset(Arkanoid& arkanoid) {
      arkanoid.pad = 49;
      arkanoid.points = 0;
      arkanoid.gameOver = true;
      arkanoid.ball[0] = 60;
      arkanoid.ball[1] = 40;
      arkanoid.vector[0] = 1;
      arkanoid.vector[1] = 1;

      // Reset block positions and sizes
      for (int i = 0; i < BLOCK_NUMBER; i++) {
        arkanoid.block[i][0] = i * 10;
        arkanoid.block[i][1] = 0;
        arkanoid.block[i][2] = 8;
      }
    }

    // Function to handle the game over state
    void gameOver(Arkanoid& arkanoid) {
      int cursor = 2, size = 50, id = 1;
      bool exit = true;

      while (exit) {
        display.display();
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2, 2);
        display.setCursor(22, 15);
        display.print("Score:");
        display.println(arkanoid.points);

        display.setTextSize(1, 2);
        display.setCursor(5, 45);
        display.print("Exit");
        display.print("       Try again");
          
        display.drawRect(cursor, 42, size, 20, WHITE);

        // Handle joystick input for menu navigation
        if (contr.Jostick(HOR) == RIGHT) {
          cursor = 2;
          size = 50;
          id = 1;
        }

        if (contr.Jostick(HOR) == LEFT) {
          cursor = 64;
          size = 62;
          id = 2;
        }
        
        // Handle button press to select menu option
        if (contr.Switch(SJO)) {
          if (id == 1) {
            arkanoid.gameOver = false;
            exit = false;
          }
          delay(10);
          if (id == 2) {
            reset(arkanoid);
            exit = false;
          }
        }
      }
    } 
};
