#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    const int BOX_HEIGHT = 20;
    const int BOX_WIDTH = 50;

    vector<Point> snake;
    snake.push_back({10, 10});
    int snakeLength = 3;
    int score = 0;

    srand(time(0));
    Point food = {rand() % (BOX_WIDTH - 2) + 1, rand() % (BOX_HEIGHT - 2) + 1};

    char dir = 'd';
    bool gameOver = false;

    while (!gameOver) {
        system("cls");
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

        if (cin.peek() != '\n') cin >> dir;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Point newHead = snake[0];
        if (dir == 'w') newHead.y--;
        else if (dir == 's') newHead.y++;
        else if (dir == 'a') newHead.x--;
        else if (dir == 'd') newHead.x++;

        if (newHead.x <= 0 || newHead.x >= BOX_WIDTH - 1 || newHead.y <= 0 || newHead.y >= BOX_HEIGHT - 1) {
            gameOver = true;
        }
        for (auto s : snake) {
            if (newHead.x == s.x && newHead.y == s.y) {
                gameOver = true;
            }
        }

        snake.insert(snake.begin(), newHead);
        if (newHead.x == food.x && newHead.y == food.y) {
            score++;
            food.x = rand() % (BOX_WIDTH - 2) + 1;
            food.y = rand() % (BOX_HEIGHT - 2) + 1;
        } else {
            while (snake.size() > snakeLength) snake.pop_back();
        }
        snakeLength = snake.size();

        this_thread::sleep_for(chrono::milliseconds(200));
    }

    system("cls");
    cout << "\n\n\tGame Over!\n";
    cout << "\tFinal Score: " << score << "\n\n";

    return 0;
}
