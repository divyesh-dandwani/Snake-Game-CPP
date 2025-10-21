#include <iostream>
using namespace std;

int main() {
    const int BOX_HEIGHT = 20;
    const int BOX_WIDTH = 50;

    int snakePosX = 10;
    int snakePosY = 10;
    int snakeLength = 3;

    for (int row = 0; row < BOX_HEIGHT; ++row) {
        for (int col = 0; col < BOX_WIDTH; ++col) {
            if (row == 0 || row == BOX_HEIGHT - 1 || col == 0 || col == BOX_WIDTH - 1)
                cout << '#';
            else if (row == snakePosY && col >= snakePosX && col < snakePosX + snakeLength)
                cout << 'O';
            else
                cout << ' ';
        }
        cout << '\n';
    }

    return 0;
}







