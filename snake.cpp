#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX_LENGTH 300

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

// Initialize console screen dimensions
void initScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

// Set console text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Move cursor to (x, y)
void gotoxy(int x, int y) {
    COORD coord; coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Point structure for positions
struct Point {
    int xCoord, yCoord;
    Point() {}
    Point(int x, int y) { xCoord = x; yCoord = y; }
};

// Snake class
class Snake {
    int length;
    char direction;
public:
    Point body[MAX_LENGTH];
    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength() { return length; }
    char getDirection() { return direction; }

    void changeDirection(char newDirection) {
        if (newDirection == DIR_UP && direction != DIR_DOWN) direction = newDirection;
        else if (newDirection == DIR_DOWN && direction != DIR_UP) direction = newDirection;
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT) direction = newDirection;
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT) direction = newDirection;
    }

    // Move snake, return false if it collides with itself
    bool move(Point food, Point specialFood, bool specialFoodActive) {
        Point prevTail = body[length - 1];
        for (int i = length - 1; i > 0; i--) body[i] = body[i - 1];

        switch (direction) {
        case DIR_UP: body[0].yCoord--; break;
        case DIR_DOWN: body[0].yCoord++; break;
        case DIR_RIGHT: body[0].xCoord++; break;
        case DIR_LEFT: body[0].xCoord--; break;
        }

        // Wrap-around
        if (body[0].xCoord <= 0) body[0].xCoord = consoleWidth - 2;
        if (body[0].xCoord >= consoleWidth - 1) body[0].xCoord = 1;
        if (body[0].yCoord <= 0) body[0].yCoord = consoleHeight - 2;
        if (body[0].yCoord >= consoleHeight - 1) body[0].yCoord = 1;

        // Self-collision
        for (int i = 1; i < length; i++)
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
                return false;

        // Eats normal food
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) {
            Beep(1000, 150); // sound for food
            body[length++] = Point(prevTail.xCoord, prevTail.yCoord);
        }

        // Eats special food
        if (specialFoodActive && specialFood.xCoord == body[0].xCoord && specialFood.yCoord == body[0].yCoord) {
            Beep(1500, 200); // sound for special food
            body[length++] = Point(prevTail.xCoord, prevTail.yCoord);
        }

        else {
            // Erase previous tail
            gotoxy(prevTail.xCoord, prevTail.yCoord);
            cout << " ";
        }

        return true;
    }
};

// Board class
class Board {
    Snake* snake;
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = 'o';
    Point specialFood;
    const char SPECIAL_FOOD = '#';
    bool specialFoodActive = false;
    int specialFoodFrameCounter = 0;
    int specialFoodDuration = 50;
    int specialFoodColorIndex = 0;
    vector<int> specialColors = {12, 13, 10, 11, 14};
    bool blinkState = true;
    int score;
    int highScore;
    bool paused = false;

public:
    Board() {
        snake = new Snake(10, 10);
        score = 0;
        loadHighScore();
        spawnFood();
    }
    ~Board() { delete snake; }

    int getScore() { return score; }

    void loadHighScore() {
        ifstream file("highscore.txt");
        if (file.is_open()) { file >> highScore; file.close(); }
        else highScore = 0;
    }

    void saveHighScore() {
        if (score > highScore) {
            ofstream file("highscore.txt");
            if (file.is_open()) { file << score; file.close(); }
        }
    }

    void spawnFood() {
        int x = 1 + rand() % (consoleWidth - 2);
        int y = 1 + rand() % (consoleHeight - 2);
        food = Point(x, y);
    }

    void spawnSpecialFood() {
        int x = 1 + rand() % (consoleWidth - 2);
        int y = 1 + rand() % (consoleHeight - 2);
        specialFood = Point(x, y);
        specialFoodActive = true;
        specialFoodColorIndex = 0;
    }

    char getHeadSymbol(char direction) {
        switch(direction) {
            case DIR_UP: return '^';
            case DIR_DOWN: return 'v';
            case DIR_LEFT: return '<';
            case DIR_RIGHT: return '>';
            default: return 'O';
        }
    }

    void drawBorder() {
        setColor(15);
        for (int x = 0; x < consoleWidth; x++) {
            gotoxy(x, 0); cout << (char)219;
            gotoxy(x, consoleHeight - 1); cout << (char)219;
        }
        for (int y = 0; y < consoleHeight; y++) {
            gotoxy(0, y); cout << (char)219;
            gotoxy(consoleWidth - 1, y); cout << (char)219;
        }
    }

    void displayScore() {
        gotoxy(consoleWidth / 2 - 12, 0);
        setColor(14); cout << "Score: " << score << "  High: " << highScore;
        setColor(15);
    }

    void drawSnakeAndFood() {
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            if (i == 0) setColor(10), cout << getHeadSymbol(snake->getDirection());
            else setColor(2), cout << SNAKE_BODY;
        }

        setColor(12);
        gotoxy(food.xCoord, food.yCoord); cout << FOOD;

        if (specialFoodActive) {
            if (blinkState) {
                setColor(specialColors[specialFoodColorIndex]);
                gotoxy(specialFood.xCoord, specialFood.yCoord); cout << SPECIAL_FOOD;
                specialFoodColorIndex = (specialFoodColorIndex + 1) % specialColors.size();
            } else {
                gotoxy(specialFood.xCoord, specialFood.yCoord); cout << " ";
            }
            blinkState = !blinkState;
        }

        setColor(15);
    }

    bool update() {
        if (paused) return true;
        bool alive = snake->move(food, specialFood, specialFoodActive);
        if (!alive) return false;

        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord) {
            score++;
            spawnFood();
        }

        if (specialFoodActive && snake->body[0].xCoord == specialFood.xCoord &&
            snake->body[0].yCoord == specialFood.yCoord) {
            score += 10;
            specialFoodActive = false;
            specialFoodFrameCounter = 0;
        }

        specialFoodFrameCounter++;
        if (!specialFoodActive && specialFoodFrameCounter >= 100) {
            spawnSpecialFood();
            specialFoodFrameCounter = 0;
        }

        if (specialFoodActive) {
            specialFoodDuration--;
            if (specialFoodDuration <= 0) {
                gotoxy(specialFood.xCoord, specialFood.yCoord);
                cout << " ";
                specialFoodActive = false;
                specialFoodDuration = 50;
            }
        }

        return true;
    }

    void togglePause() {
        paused = !paused;
        if (paused) {
            Beep(600, 100);
            showPauseMenu();
        } else {
            Beep(800, 100);
        }
    }

    void showPauseMenu() {
        system("cls");
        string title = "=== GAME PAUSED ===";
        string opt1 = "[1] Resume";
        string opt2 = "[2] Restart";
        string opt3 = "[3] Exit";
        gotoxy((consoleWidth - title.length()) / 2, consoleHeight / 2 - 2);
        setColor(14); cout << title;
        gotoxy((consoleWidth - opt1.length()) / 2, consoleHeight / 2); setColor(15); cout << opt1;
        gotoxy((consoleWidth - opt2.length()) / 2, consoleHeight / 2 + 1); cout << opt2;
        gotoxy((consoleWidth - opt3.length()) / 2, consoleHeight / 2 + 2); cout << opt3;

        char ch = getch();
        if (ch == '1') paused = false;
        else if (ch == '2') {
            Beep(700, 200);
            restartGame();
        }
        else if (ch == '3') {
            system("cls");
            Beep(300, 400);
            exit(0);
        }
        system("cls");
        drawBorder();
    }

    void restartGame() {
        delete snake;
        snake = new Snake(10, 10);
        score = 0;
        specialFoodActive = false;
        specialFoodFrameCounter = 0;
        spawnFood();
        paused = false;
    }

    void getInput() {
        if (kbhit()) {
            int key = getch();
            if (key == 32) { // Space bar for pause/resume
                togglePause();
                return;
            }
            if (key == 224) {
                key = getch();
                if (key == 72) snake->changeDirection(DIR_UP);
                else if (key == 80) snake->changeDirection(DIR_DOWN);
                else if (key == 75) snake->changeDirection(DIR_LEFT);
                else if (key == 77) snake->changeDirection(DIR_RIGHT);
            } else {
                if (key == 'w' || key == 'W') snake->changeDirection(DIR_UP);
                else if (key == 'a' || key == 'A') snake->changeDirection(DIR_LEFT);
                else if (key == 's' || key == 'S') snake->changeDirection(DIR_DOWN);
                else if (key == 'd' || key == 'D') snake->changeDirection(DIR_RIGHT);
            }
        }
    }

    void startScreen() {
        system("cls");
        string title = "=== SNAKE GAME ===";
        string instr1 = "Use Arrow Keys or WASD to move.";
        string instr2 = "Eat food 'o' and special food '#' (+10 points).";
        string instr3 = "Press SPACE to Pause/Resume.";
        string instr4 = "Press any key to start...";
        gotoxy((consoleWidth - title.length()) / 2, consoleHeight / 2 - 4);
        setColor(14); cout << title;
        gotoxy((consoleWidth - instr1.length()) / 2, consoleHeight / 2 - 2);
        setColor(15); cout << instr1;
        gotoxy((consoleWidth - instr2.length()) / 2, consoleHeight / 2 - 1);
        cout << instr2;
        gotoxy((consoleWidth - instr3.length()) / 2, consoleHeight / 2);
        cout << instr3;
        gotoxy((consoleWidth - instr4.length()) / 2, consoleHeight / 2 + 2);
        cout << instr4;
        getch();
        system("cls");
    }

    void endGame() { saveHighScore(); }
};

// Main
int main() {
    srand(time(0));
    initScreen();
    Board* board = new Board();

    board->startScreen();
    board->drawBorder();
    board->displayScore();
    board->drawSnakeAndFood();

    const int SLEEP_TIME = 100;

    while (board->update()) {
        board->getInput();
        board->drawSnakeAndFood();
        board->displayScore();
        Sleep(SLEEP_TIME);
    }

    Beep(300, 400);
    board->endGame();
    gotoxy(0, consoleHeight);
    cout << "\n\nGame Over!\nFinal Score: " << board->getScore() << "\n";
    return 0;
}
