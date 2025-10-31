#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Point {
    int x;
    int y;
};


void gotoXY(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void clearScreen() {
    system("cls");
}


void setupConsole() {
    system("mode 80, 30");  
    system("color 0A");     

int main() {
    setupConsole();

    const int BOX_HEIGHT = 20;
    const int BOX_WIDTH = 50;

    bool playAgain = true;

    while (playAgain) {
        vector<Point> snake;
        snake.push_back({10, 10});
        int snakeLength = 3;
        int score = 0;

        srand(time(0));
        Point food = {rand() % (BOX_WIDTH - 2) + 1, rand() % (BOX_HEIGHT - 2) + 1};

        char dir = 'd';
        bool gameOver = false;

        while (!gameOver) {
            clearScreen();
            cout << "Score: " << score << "\n\n";

            
            for (int row = 0; row < BOX_HEIGHT; ++row) {
                for (int col = 0; col < BOX_WIDTH; ++col) {
                    if (row == 0 || row == BOX_HEIGHT - 1 || col == 0 || col == BOX_WIDTH - 1)
                        cout << '#';
                    else {
                        bool printed = false;
                        if (row == food.y && col == food.x) {
                            cout << 'F';
                            printed = true;
                        }
                        for (auto s : snake) {
                            if (row == s.y && col == s.x) {
                                cout << 'O';
                                printed = true;
                                break;
                            }
                        }
                        if (!printed) cout << ' ';
                    }
                }
                cout << '\n';
            }

            
            if (_kbhit()) {
                char key = _getch();
                if (key == 'w' && dir != 's') dir = 'w';
                else if (key == 's' && dir != 'w') dir = 's';
                else if (key == 'a' && dir != 'd') dir = 'a';
                else if (key == 'd' && dir != 'a') dir = 'd';
            }

            
            Point newHead = snake[0];
            if (dir == 'w') newHead.y--;
            else if (dir == 's') newHead.y++;
            else if (dir == 'a') newHead.x--;
            else if (dir == 'd') newHead.x++;

           
            if (newHead.x <= 0 || newHead.x >= BOX_WIDTH - 1 || newHead.y <= 0 || newHead.y >= BOX_HEIGHT - 1)
                gameOver = true;

            
            for (auto s : snake) {
                if (newHead.x == s.x && newHead.y == s.y)
                    gameOver = true;
            }

           
            snake.insert(snake.begin(), newHead);
            if (newHead.x == food.x && newHead.y == food.y) {
                score++;
                food.x = rand() % (BOX_WIDTH - 2) + 1;
                food.y = rand() % (BOX_HEIGHT - 2) + 1;
                snakeLength++;
            } else {
                while (snake.size() > snakeLength)
                    snake.pop_back();
            }

            Sleep(120);
        }

        
        clearScreen();
        cout << "\n\n\t===== GAME OVER =====\n";
        cout << "\tFinal Score: " << score << "\n\n";
        cout << "\tPress [R] to Restart or [Q] to Quit\n";

        char choice;
        while (true) {
            choice = _getch();
            if (choice == 'r' || choice == 'R') {
                playAgain = true;
                break;
            } else if (choice == 'q' || choice == 'Q') {
                playAgain = false;
                break;
            }
        }
    }

    clearScreen();
    cout << "\n\n\tThank you for playing Snake Game!\n";
    
    return 0;
}
