class SnakeGame {
    Control contr;  // Instance of the Control class to handle input

    // Structure defining the properties of the Snake game
    struct Snake {
      // Constants for game logic
      #define MAX_LENGTH 25
      #define SNAKE_SIZE 7
      #define APPLE_SIZE 4

      int snake[MAX_LENGTH][2];  // Array to store snake segments
      int length;                // Length of the snake
      int vectorX;               // X-component of the snake's movement vector
      int vectorY;               // Y-component of the snake's movement vector
      int vector;                // Current direction of the snake (1: left, 2: right, 3: down, 4: up)
      int apple[2];              // Array to store the position of the apple
      int speed;                 // Speed of the snake
      int move;                  // Move increment for the snake
      bool gameOver;             // Flag to indicate if the game is over

      // Constructor to initialize game state
      Snake() : length(2), vectorX(0), vectorY(-8), gameOver(true), move(9), vector(3), speed(200) {
        // Initialize snake positions
        for(int i = 0; i < MAX_LENGTH; i++) {
          snake[i][0] = 0;
          snake[i][1] = 0;
        }

        for (int i = 0; i < length; i++) {
          snake[i][0] = OLED_WIDTH / 2;
          snake[i][1] = OLED_HEIGHT / 2 + i * SNAKE_SIZE;
        }

        // Generate a random apple position
        do {
          apple[0] = random(3, OLED_WIDTH - 3);
          apple[1] = random(3, OLED_HEIGHT - 3);
        } while(apple[0] % 10 != 0 || apple[1] % 10 != 0);
      }
    };

public:
    // Main function to run the Snake game
    void main() {
      Snake snake;  // Instance of the Snake structure

      while(snake.gameOver) {
        display.clearDisplay();
        delay(snake.speed);
        gameControl(snake);
        if(!snake.gameOver) break;
        gameDraw(snake); 
        display.display();
      }
    }

private:
    // Function to draw the game on the display
    void gameDraw(Snake& snake) {
      display.fillRect(snake.snake[0][0] + snake.vectorX, snake.snake[0][1] + snake.vectorY, SNAKE_SIZE, SNAKE_SIZE, WHITE);

      for (int i = 0; i < snake.length; i++) {
        display.fillRect(snake.snake[i][0], snake.snake[i][1], SNAKE_SIZE, SNAKE_SIZE, WHITE);
      }
      display.drawCircle(snake.apple[0], snake.apple[1], APPLE_SIZE, WHITE);
    }

    // Function to control the game logic
    void gameControl(Snake& snake) {
      // Move the head of the snake
      snake.snake[0][0] += snake.vectorX;
      snake.snake[0][1] += snake.vectorY;

      // Wrap around the screen if the snake goes beyond the edges
      snake.snake[0][0] = (snake.snake[0][0] > OLED_WIDTH - 8) 
        ? 0 : (snake.snake[0][0] < 0) ? OLED_WIDTH - SNAKE_SIZE : snake.snake[0][0];

      snake.snake[0][1] = (snake.snake[0][1] > OLED_HEIGHT - 8) 
        ? 0 : (snake.snake[0][1] < 0) ? OLED_HEIGHT - SNAKE_SIZE : snake.snake[0][1];

      // Read analog joystick input for controlling the snake's direction
      if (contr.Jostick(HOR) == LEFT && snake.vector != 2) { 
        snake.vectorX = snake.move; 
        snake.vectorY = 0; 
        snake.vector = 1;
        
      } else if (contr.Jostick(HOR) == RIGHT && snake.vector != 1) { 
        snake.vectorX = -snake.move; 
        snake.vectorY = 0; 
        snake.vector = 2;
      }

      if (contr.Jostick(VER) == DOWN && snake.vector != 4) { 
        snake.vectorY = snake.move; 
        snake.vectorX = 0; 
        snake.vector = 3;
      } else if (contr.Jostick(VER) == UP && snake.vector != 3) { 
        snake.vectorY = -snake.move; 
        snake.vectorX = 0; 
        snake.vector = 4;
      }

      // Check if the snake has eaten the apple
      if (snake.snake[0][0] + SNAKE_SIZE > snake.apple[0] &&
          snake.snake[0][0] < snake.apple[0] + APPLE_SIZE * 2 &&
          snake.snake[0][1] + SNAKE_SIZE > snake.apple[1] &&
          snake.snake[0][1] < snake.apple[1] + APPLE_SIZE * 2) {
        snake.length++;

        // Generate a new apple location
        do {
          snake.apple[0] = random(3, OLED_WIDTH - 3);
          snake.apple[1] = random(3, OLED_HEIGHT - 3);
        } while (snake.apple[0] % 10 != 0 || snake.apple[1] % 10 != 0);
      }

      // Check for collision with the snake's own body
      for (int i = 1; i < snake.length; i++) {
        if (snake.snake[i][0] == snake.snake[0][0] && snake.snake[i][1] == snake.snake[0][1])
          gameOver(snake);
      }

      // Update the positions of the snake's body segments
      for (int i = snake.length - 1; i > 0; i--) {
        snake.snake[i][0] = snake.snake[i - 1][0];
        snake.snake[i][1] = snake.snake[i - 1][1];
      }
    }

    // Function to reset the game state
    void reset(Snake& snake) {
      snake.length = 2;
      snake.vectorX = 0;
      snake.vectorY = -8;
      snake.gameOver = true;
      snake.move = 9;
      snake.speed = 200;
      snake.vector = 3;

      for (int i = 0; i < MAX_LENGTH; i++) {
        snake.snake[i][0] = 0;
        snake.snake[i][1] = 0;
      }

      for (int i = 0; i < snake.length; i++) {
        snake.snake[i][0] = OLED_WIDTH / 2;
        snake.snake[i][1] = OLED_HEIGHT / 2 + i * SNAKE_SIZE;
      }

      // Generate a new apple location
      do {
        snake.apple[0] = random(3, OLED_WIDTH - 3);
        snake.apple[1] = random(3, OLED_HEIGHT - 3);
      } while (snake.apple[0] % 10 != 0 || snake.apple[1] % 10 != 0);
    }

    // Function to handle game over state
    void gameOver(Snake& snake) {
      int cursor=2, size=50, id=1;
      bool exit=true;

      while(exit) {
        display.display();
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2,2);
        display.setCursor(22, 15);
        display.print("Score:");
        display.println(snake.length-2);

        display.setTextSize(1,2);
        display.setCursor(5, 45);
        display.print("Exit");
        display.print("       Try again");
          
        display.drawRect(cursor, 42, size, 20, WHITE);
        if(contr.Jostick(HOR) == RIGHT) {
          cursor=2;
          size=50;
          id=1;
        }

        if(contr.Jostick(HOR) == LEFT) {
          cursor=64;
          size=62;
          id=2;
        }
        
        if(contr.Switch(SJO)) {
          if(id==1) snake.gameOver=false, exit=false;
          delay(10);
          if(id==2) reset(snake), exit=false;
        }
      }
    }
};
