#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Grid dimensions
const int gridX = 40, gridY = 40;
const int squareSize = 20;

// Snake variables
int snakeLength = 1;
int snakeX[100] = {20}, snakeY[100] = {20}; // Snake body (positions)
int dirX = 1, dirY = 0;                     // Snake direction
int foodX, foodY;                            // Food position

bool gameOver = false;

// Function to initialize game variables
void initGame() {
    snakeLength = 1;
    snakeX[0] = gridX / 2;
    snakeY[0] = gridY / 2;
    dirX = 1;
    dirY = 0;
    gameOver = false;

    srand(time(0));
    foodX = rand() % gridX;
    foodY = rand() % gridY;
}

// Function to draw a square on the grid
void drawSquare(int x, int y) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 1, y);
    glVertex2f(x + 1, y + 1);
    glVertex2f(x, y + 1);
    glEnd();
}

// Function to draw the grid (snake, food, etc.)
void drawGrid() {
    // Draw snake
    for (int i = 0; i < snakeLength; i++) {
        if (i == 0) {
            glColor3f(0.0, 1.0, 0.0); // Snake head color
        } else {
            glColor3f(0.0, 0.5, 0.0); // Snake body color
        }
        drawSquare(snakeX[i], snakeY[i]);
    }

    // Draw food
    glColor3f(1.0, 0.0, 0.0); // Food color
    drawSquare(foodX, foodY);
}

// Function to check collision with boundaries or self
void checkCollision() {
    // Check boundary collision
    if (snakeX[0] < 0 || snakeX[0] >= gridX || snakeY[0] < 0 || snakeY[0] >= gridY) {
        gameOver = true;
    }

    // Check self collision
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = true;
        }
    }
}

// Function to move the snake
void moveSnake() {
    // Shift the snake body
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    // Move the head
    snakeX[0] += dirX;
    snakeY[0] += dirY;

    checkCollision();

    // Check if the snake eats the food
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        snakeLength++;
        foodX = rand() % gridX;
        foodY = rand() % gridY;
    }
}

// GLUT display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        glColor3f(1.0, 0.0, 0.0); // Game over text color
        glRasterPos2f(gridX / 2 - 3, gridY / 2);
        const char *gameOverText = "GAME OVER";
        for (int i = 0; gameOverText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, gameOverText[i]);
        }
    } else {
        drawGrid();
    }

    glutSwapBuffers();
}

// Function to handle keyboard input for controlling the snake
void keyboard(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP:
            if (dirY != 1) { dirX = 0; dirY = -1; }
            break;
        case GLUT_KEY_DOWN:
            if (dirY != -1) { dirX = 0; dirY = 1; }
            break;
        case GLUT_KEY_LEFT:
            if (dirX != 1) { dirX = -1; dirY = 0; }
            break;
        case GLUT_KEY_RIGHT:
            if (dirX != -1) { dirX = 1; dirY = 0; }
            break;
    }
}

// Function to update game state and handle animations
void update(int) {
    if (!gameOver) {
        moveSnake();
    }

    glutPostRedisplay();
    glutTimerFunc(100, update, 0); // Update every 100ms (controls speed)
}

// Function to set up the grid and viewport
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, gridX, 0, gridY); // Set up the 2D orthographic view
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(gridX * squareSize, gridY * squareSize);
    glutCreateWindow("Snake Game");

    initOpenGL();
    initGame();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(100, update, 0); // Start the game loop

    glutMainLoop();
    return 0;
}
