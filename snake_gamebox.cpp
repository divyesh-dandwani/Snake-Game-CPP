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

    srand(time(0));
    int foodX = rand() % (BOX_WIDTH - 2) + 1;
    int foodY = rand() % (BOX_HEIGHT - 2) + 1;

    char dir = 'd';

    while (true) {
        system("cls");

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

        if (snakePosX <= 0) snakePosX = 1;
        if (snakePosX + snakeLength >= BOX_WIDTH - 1) snakePosX = BOX_WIDTH - snakeLength - 1;
        if (snakePosY <= 0) snakePosY = 1;
        if (snakePosY >= BOX_HEIGHT - 1) snakePosY = BOX_HEIGHT - 2;

        if (snakePosY == foodY && snakePosX <= foodX && foodX < snakePosX + snakeLength) {
            snakeLength++;
            foodX = rand() % (BOX_WIDTH - 2) + 1;
            foodY = rand() % (BOX_HEIGHT - 2) + 1;
        }

        this_thread::sleep_for(chrono::milliseconds(200));
    }

    return 0;
}







