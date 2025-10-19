#include <iostream>
using namespace std;    
int main() {
    const int BOX_HEIGHT = 12;
    const int BOX_WIDTH = 30;

    int snakePosX = 5;
    int snakePosY = 5;
    int snakeLength = 6;

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






