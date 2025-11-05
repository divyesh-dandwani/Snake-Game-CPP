#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Point
{
    int x;
    int y;
};

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoXY(int x, int y)
{
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(console, coord);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);
}

void setupConsole()
{
    system("mode 120, 40"); // ✅ Wider console
    system("color 0A");
    hideCursor();
}

// Draw static box (border only)
void drawBox(int BOX_WIDTH, int BOX_HEIGHT, int OFFSET_X, int OFFSET_Y)
{
    for (int row = 0; row < BOX_HEIGHT; ++row)
    {
        for (int col = 0; col < BOX_WIDTH; ++col)
        {
            gotoXY(col + OFFSET_X, row + OFFSET_Y);
            if (row == 0 || row == BOX_HEIGHT - 1 || col == 0 || col == BOX_WIDTH - 1)
                cout << '#';
            else
                cout << ' ';
        }
    }
}

// Generate random blocks (obstacles)
// vector<Point> generateBlocks(int BOX_WIDTH, int BOX_HEIGHT, int count)
// {
//     vector<Point> blocks;
//     for (int i = 0; i < count; i++)
//     {
//         Point b;
//         b.x = rand() % (BOX_WIDTH - 2) + 1;
//         b.y = rand() % (BOX_HEIGHT - 2) + 1;
//         blocks.push_back(b);
//     }
//     return blocks;
// }

vector<Point> generateBlocks(int BOX_WIDTH, int BOX_HEIGHT, int clusterCount,
                             int blockMin, int blockMax, int spacingMin)
{
    vector<Point> blocks;

    for (int i = 0; i < clusterCount; i++)
    {
        bool valid = false;
        Point center;

        // choose cluster center with spacing
        while (!valid)
        {
            valid = true;
            center.x = rand() % (BOX_WIDTH - 8) + 4;
            center.y = rand() % (BOX_HEIGHT - 8) + 4;
            for (auto b : blocks)
                if (abs(center.x - b.x) < spacingMin && abs(center.y - b.y) < spacingMin)
                    valid = false;
        }

        int pattern = rand() % 4; // 0=L, 1=U, 2=T, 3=line
        int blockCount = rand() % (blockMax - blockMin + 1) + blockMin;

        for (int j = 0; j < blockCount; j++)
        {
            Point p = center;
            switch (pattern)
            {
            case 0:
                p.x += (j < 2 ? j : 0);
                p.y += (j < 2 ? 0 : j - 1);
                break; // L
            case 1:
                p.x += (j < 3 ? j : 1);
                p.y += (j < 3 ? 0 : j - 2);
                break; // U
            case 2:
                p.x += (j == 0 ? -1 : (j == 1 ? 0 : 1));
                p.y += (j < 3 ? 0 : 1);
                break; // T
            case 3:
                p.x += j;
                break; // line
            }

            if (p.x > 0 && p.x < BOX_WIDTH - 1 && p.y > 0 && p.y < BOX_HEIGHT - 1)
                blocks.push_back(p);
        }
    }
    return blocks;
}

// Draw blocks
void drawBlocks(const vector<Point> &blocks, int OFFSET_X, int OFFSET_Y)
{
    for (auto b : blocks)
    {
        gotoXY(b.x + OFFSET_X, b.y + OFFSET_Y);
        cout << '#';
    }
}

// Check collision with blocks
bool hitBlock(const vector<Point> &blocks, const Point &head)
{
    for (auto b : blocks)
        if (b.x == head.x && b.y == head.y)
            return true;
    return false;
}

// Generate food not inside snake or block
Point generateFood(int BOX_WIDTH, int BOX_HEIGHT, const vector<Point> &snake, const vector<Point> &blocks)
{
    Point f;
    bool valid = false;
    while (!valid)
    {
        f.x = rand() % (BOX_WIDTH - 2) + 1;
        f.y = rand() % (BOX_HEIGHT - 2) + 1;
        valid = true;
        for (auto s : snake)
            if (s.x == f.x && s.y == f.y)
                valid = false;
        for (auto b : blocks)
            if (b.x == f.x && b.y == f.y)
                valid = false;
    }
    return f;
}

int main()
{
    setupConsole();

    const int BOX_HEIGHT = 25;
    const int BOX_WIDTH = 80;
    const int OFFSET_X = 10;
    const int OFFSET_Y = 3;

    int difficultyLevel = 1;
    while (true)
    {
        system("cls");
        cout << "\n\n\tSelect Difficulty:\n";
        cout << "\t[1] Easy\n";
        cout << "\t[2] Medium\n";
        cout << "\t[3] Hard\n\n";
        cout << "\tChoice: ";
        char ch = _getch();
        if (ch == '1' || ch == '2' || ch == '3')
        {
            difficultyLevel = ch - '0';
            break;
        }
    }

    system("cls");
    cout << "\n\n\tYou chose: "
         << (difficultyLevel == 1 ? "EASY" : difficultyLevel == 2 ? "MEDIUM"
                                                                  : "HARD")
         << "\n\tPress any key to begin...";
    _getch();

    bool playAgain = true;

    while (playAgain)
    {
        system("cls");
        drawBox(BOX_WIDTH, BOX_HEIGHT, OFFSET_X, OFFSET_Y);

        // ✅ Random obstacles
        srand((unsigned)time(0));
        // int blockCount = rand() % 15 + 10; // 10–25 random blocks
        int clusterMin, clusterMax, blockMin, blockMax, spacingMin;

        if (difficultyLevel == 1)
        { // Easy
            clusterMin = 3;
            clusterMax = 4;
            blockMin = 2;
            blockMax = 4;
            spacingMin = 8;
        }
        else if (difficultyLevel == 2)
        { // Medium
            clusterMin = 5;
            clusterMax = 6;
            blockMin = 3;
            blockMax = 5;
            spacingMin = 6;
        }
        else
        { // Hard
            clusterMin = 7;
            clusterMax = 9;
            blockMin = 4;
            blockMax = 6;
            spacingMin = 4;
        }

        int clusterCount = rand() % (clusterMax - clusterMin + 1) + clusterMin;

        vector<Point> blocks = generateBlocks(BOX_WIDTH, BOX_HEIGHT,
                                              clusterCount, blockMin, blockMax, spacingMin);

        drawBlocks(blocks, OFFSET_X, OFFSET_Y);

        vector<Point> snake;
        snake.push_back({15, 10});
        int snakeLength = 3;
        int score = 0;

        Point food = generateFood(BOX_WIDTH, BOX_HEIGHT, snake, blocks);

        char dir = 'd';
        bool gameOver = false;

        int speedLevel = 5; // Default medium speed
        int delay = 120 - (speedLevel * 10);

        auto lastMoveTime = steady_clock::now();

        while (!gameOver)
        {
            gotoXY(0, 0);
            cout << "Score: " << score;
            cout << "   Speed: " << speedLevel << "/10";
            cout << "   Controls: [WASD / Arrows] Move | [+/-] Speed | [Q] Quit";

            // Handle input
            if (_kbhit())
            {
                char key = _getch();
                if (key == -32 || key == 224)
                {
                    key = _getch();
                    if (key == 72 && dir != 's')
                        dir = 'w';
                    else if (key == 80 && dir != 'w')
                        dir = 's';
                    else if (key == 75 && dir != 'd')
                        dir = 'a';
                    else if (key == 77 && dir != 'a')
                        dir = 'd';
                }
                else
                {
                    if ((key == 'w' || key == 'W') && dir != 's')
                        dir = 'w';
                    else if ((key == 's' || key == 'S') && dir != 'w')
                        dir = 's';
                    else if ((key == 'a' || key == 'A') && dir != 'd')
                        dir = 'a';
                    else if ((key == 'd' || key == 'D') && dir != 'a')
                        dir = 'd';
                    else if (key == '+' || key == '=')
                    {
                        if (speedLevel < 10)
                            speedLevel++;
                    }
                    else if (key == '-')
                    {
                        if (speedLevel > 1)
                            speedLevel--;
                    }
                    else if (key == 'q' || key == 'Q')
                    {
                        playAgain = false;
                        gameOver = true;
                        break;
                    }
                }
            }

            delay = 120 - (speedLevel * 10);

            auto currentTime = steady_clock::now();
            auto elapsed = duration_cast<milliseconds>(currentTime - lastMoveTime).count();
            if (elapsed < delay)
            {
                Sleep(1);
                continue;
            }
            lastMoveTime = currentTime;

            // Erase tail
            Point tail = snake.back();
            gotoXY(tail.x + OFFSET_X, tail.y + OFFSET_Y);
            cout << ' ';

            // Move head
            Point newHead = snake[0];
            if (dir == 'w')
                newHead.y--;
            else if (dir == 's')
                newHead.y++;
            else if (dir == 'a')
                newHead.x--;
            else if (dir == 'd')
                newHead.x++;

            // Collisions
            if (newHead.x <= 0 || newHead.x >= BOX_WIDTH - 1 || newHead.y <= 0 || newHead.y >= BOX_HEIGHT - 1)
                gameOver = true;
            if (hitBlock(blocks, newHead))
                gameOver = true;

            for (auto s : snake)
                if (newHead.x == s.x && newHead.y == s.y)
                    gameOver = true;

            snake.insert(snake.begin(), newHead);

            // Eat food
            if (newHead.x == food.x && newHead.y == food.y)
            {
                score++;
                food = generateFood(BOX_WIDTH, BOX_HEIGHT, snake, blocks);
                snakeLength++;
            }
            else
            {
                while (snake.size() > snakeLength)
                    snake.pop_back();
            }

            // Draw updates
            gotoXY(food.x + OFFSET_X, food.y + OFFSET_Y);
            cout << '@';
            gotoXY(newHead.x + OFFSET_X, newHead.y + OFFSET_Y);
            cout << 'O';
        }

        if (!playAgain)
            break;

        gotoXY(OFFSET_X, BOX_HEIGHT + OFFSET_Y + 2);
        cout << "\n\n\t===== GAME OVER =====\n";
        cout << "\tFinal Score: " << score << "\n\n";
        cout << "\tPress [R] to Restart or [Q] to Quit\n";

        char choice;
        while (true)
        {
            choice = _getch();
            if (choice == 'r' || choice == 'R')
            {
                playAgain = true;
                break;
            }
            else if (choice == 'q' || choice == 'Q')
            {
                playAgain = false;
                break;
            }
        }
    }

    system("cls");
    cout << "Thank you for playing Snake Game!\n";
    cout << "Returning to your command prompt...\n\n";
    return 0;
}
