#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    const int BOX_HEIGHT = 20;
    const int BOX_WIDTH = 50;

    int snakePosX = 10;
    int snakePosY = 10;
    int snakeLength = 3;
    int score = 0;

    srand(time(0));
    int foodX = rand() % (BOX_WIDTH - 2) + 1;
    int foodY = rand() % (BOX_HEIGHT - 2) + 1;

    char dir = 'd';
    bool gameOver = false;

    while (!gameOver) {
        system("cls");
        cout << "Score: " << score << "\n\n";

        for (int row = 0; row < BOX_HEIGHT; ++row) {
            for (int col = 0; col < BOX_WIDTH; ++col) {
                if (row == 0 || row == BOX_HEIGHT - 1 || col == 0 || col == BOX_WIDTH - 1)
                    cout << '#';
                else if (row == snakePosY && col >= snakePosX && col < snakePosX + snakeLength)
                    cout << 'O';
                else if (row == foodY && col == foodX)
                    cout << 'F';
                else
                    cout << ' ';
            }
            cout << '\n';
        }

        if (cin.peek() != '\n') cin >> dir;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (dir == 'w') snakePosY--;
        else if (dir == 's') snakePosY++;
        else if (dir == 'a') snakePosX--;
        else if (dir == 'd') snakePosX++;

        if (snakePosY == foodY && snakePosX <= foodX && foodX < snakePosX + snakeLength) {
            snakeLength++;
            score++;
            foodX = rand() % (BOX_WIDTH - 2) + 1;
            foodY = rand() % (BOX_HEIGHT - 2) + 1;
        }

        if (snakePosX <= 0 || snakePosX + snakeLength >= BOX_WIDTH - 1 || snakePosY <= 0 || snakePosY >= BOX_HEIGHT - 1) {
            gameOver = true;
        }

        this_thread::sleep_for(chrono::milliseconds(200));
    }

    system("cls");
    cout << "\n\n\tGame Over!\n";
    cout << "\tFinal Score: " << score << "\n\n";

    return 0;
}


