#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
#endif

using namespace std;

struct Point { int x, y; };

// Cross-platform utilities
class Terminal {
public:
    static void clear() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    static void hideCursor() {
        #ifdef _WIN32
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO info;
            GetConsoleCursorInfo(h, &info);
            info.bVisible = false;
            SetConsoleCursorInfo(h, &info);
        #else
            cout << "\033[?25l";
            cout.flush();
        #endif
    }
    
    static void showCursor() {
        #ifdef _WIN32
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO info;
            GetConsoleCursorInfo(h, &info);
            info.bVisible = true;
            SetConsoleCursorInfo(h, &info);
        #else
            cout << "\033[?25h";
            cout.flush();
        #endif
    }
    
    static void gotoXY(int x, int y) {
        #ifdef _WIN32
            COORD pos = {(SHORT)x, (SHORT)y};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        #else
            cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
            cout.flush();
        #endif
    }
    
    static void setColor(int fg, int bg = 0) {
        #ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg + (bg * 16));
        #else
            // ANSI colors: 30-37 foreground, 40-47 background
            int ansi_fg = 30 + (fg % 8);
            int bright = (fg > 7) ? 1 : 0;
            cout << "\033[" << bright << ";" << ansi_fg << "m";
            cout.flush();
        #endif
    }
    
    static void resetColor() {
        #ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        #else
            cout << "\033[0m";
            cout.flush();
        #endif
    }
    
    static bool kbhit() {
        #ifdef _WIN32
            return _kbhit();
        #else
            struct timeval tv = {0, 0};
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
        #endif
    }
    
    static char getChar() {
        #ifdef _WIN32
            return _getch();
        #else
            return getchar();
        #endif
    }
    
    static void setupRawMode() {
        #ifndef _WIN32
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        #endif
    }
    
    static void restoreMode() {
        #ifndef _WIN32
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag |= (ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        #endif
    }
    
    static void sleep_ms(int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
    }
};

struct Theme {
    string name;
    int wallColor, blockColor, headColor, bodyColor, foodColor, uiColor;
    vector<string> foodEmojis;
    
    Theme(string n, int w, int b, int h, int bo, int f, int ui, vector<string> fe)
        : name(n), wallColor(w), blockColor(b), headColor(h),
          bodyColor(bo), foodColor(f), uiColor(ui), foodEmojis(fe) {}
};

vector<Theme> themes = {
    Theme("🌟 Neon Tech", 11, 8, 14, 10, 13, 15, {"🍎", "🍊", "🍋", "🍇", "🍓"}),
    Theme("🔥 Inferno", 12, 8, 14, 6, 9, 14, {"🌶️", "🔥", "🍖", "🥓", "🍗"}),
    Theme("🌿 Jungle", 10, 8, 10, 2, 13, 15, {"🍌", "🥥", "🍍", "🥭", "🍉"}),
    Theme("❄️  Ice", 11, 8, 15, 9, 11, 15, {"🧊", "❄️", "🐟", "🦐", "🐙"}),
    Theme("🎮 Retro", 14, 8, 14, 10, 13, 15, {"🍒", "🍑", "🍐", "🥝", "🫐"})
};

vector<Point> generateBlocks(int w, int h, int clusterCount, int blockMin, int blockMax, int spacingMin) {
    vector<Point> blocks;
    for (int i = 0; i < clusterCount; i++) {
        bool valid = false;
        Point c;
        while (!valid) {
            valid = true;
            c.x = rand() % (w - 8) + 4;
            c.y = rand() % (h - 8) + 4;
            for (auto &b : blocks)
                if (abs(c.x - b.x) < spacingMin && abs(c.y - b.y) < spacingMin)
                    valid = false;
        }
        int pattern = rand() % 5;
        int count = rand() % (blockMax - blockMin + 1) + blockMin;
        for (int j = 0; j < count; j++) {
            Point p = c;
            switch (pattern) {
                case 0: p.x += (j < 2 ? j : 0); p.y += (j < 2 ? 0 : j - 1); break;
                case 1: p.x += (j < 3 ? j : 1); p.y += (j < 3 ? 0 : j - 2); break;
                case 2: p.x += (j % 2 == 0 ? j / 2 : -(j / 2)); p.y += (j < 2 ? 0 : 1); break;
                case 3: p.x += j; p.y += (j % 2); break;
                default: p.x += (j % 3); p.y += (j / 3); break;
            }
            if (p.x > 0 && p.x < w - 1 && p.y > 0 && p.y < h - 1)
                blocks.push_back(p);
        }
    }
    return blocks;
}

void drawBlocks(const vector<Point> &blocks, int ox, int oy, int color) {
    Terminal::setColor(color);
    for (auto &b : blocks) {
        Terminal::gotoXY(b.x + ox, b.y + oy);
        cout << "▓";
    }
    Terminal::resetColor();
}

bool hitBlock(const vector<Point> &blocks, const Point &h) {
    for (auto &b : blocks) if (b.x == h.x && b.y == h.y) return true;
    return false;
}

Point generateFood(int w, int h, const vector<Point> &snake, const vector<Point> &blocks) {
    Point f;
    bool ok = false;
    int attempts = 0;
    while (!ok && attempts < 1000) {
        ok = true;
        f.x = rand() % (w - 4) + 2; // More margin from walls
        f.y = rand() % (h - 4) + 2;
        
        // Check walls (with extra margin)
        if (f.x <= 1 || f.x >= w - 2 || f.y <= 1 || f.y >= h - 2) ok = false;
        
        // Check snake collision
        for (auto &s : snake) {
            if (s.x == f.x && s.y == f.y) {
                ok = false;
                break;
            }
        }
        
        // Check block collision
        if (ok) {
            for (auto &b : blocks) {
                if (b.x == f.x && b.y == f.y) {
                    ok = false;
                    break;
                }
            }
        }
        attempts++;
    }
    return f;
}

void drawBox(int w, int h, int ox, int oy, int color) {
    Terminal::setColor(color);
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            Terminal::gotoXY(c + ox, r + oy);
            if (r == 0 && c == 0) cout << "╔";
            else if (r == 0 && c == w - 1) cout << "╗";
            else if (r == h - 1 && c == 0) cout << "╚";
            else if (r == h - 1 && c == w - 1) cout << "╝";
            else if (r == 0 || r == h - 1) cout << "═";
            else if (c == 0 || c == w - 1) cout << "║";
        }
    }
    Terminal::resetColor();
}

int selectTheme() {
    int index = 0;
    bool changed = true;
    
    while (true) {
        if (changed) {
            Terminal::clear();
            Terminal::gotoXY(25, 3);
            Terminal::setColor(14);
            cout << "╔═══════════════════════════════╗";
            Terminal::gotoXY(25, 4);
            cout << "║    SELECT YOUR THEME    ║";
            Terminal::gotoXY(25, 5);
            cout << "╚═══════════════════════════════╝";
            
            // for (int i = 0; i < (int)themes.size(); i++) {
            //     Terminal::gotoXY(25, 8 + i * 2);
            //     if (i == index) {
            //         Terminal::setColor(15);
            //         cout << "► ";
            //     } else {
            //         Terminal::setColor(8);
            //         cout << "  ";
            //     }
            //     Terminal::setColor(themes[i].uiColor);
            //     cout << themes[i].name;
            // }
            
            for (int i = 0; i < (int)themes.size(); i++) {
    Terminal::gotoXY(25, 8 + i * 2);

    if (i == index) {
        Terminal::setColor(14);   // bright yellow for selected theme
        cout << "► " << themes[i].name;
    } else {
        Terminal::setColor(8);    // dim gray for unselected
        cout << "  " << themes[i].name;
    }
}

            Terminal::gotoXY(20, 20);
            Terminal::setColor(10);
            cout << "↑↓ Navigate  │  ENTER Select  │  Q Quit";
            Terminal::resetColor();
            changed = false;
        }
        
        if (Terminal::kbhit()) {
            char ch = Terminal::getChar();
            #ifdef _WIN32
            if (ch == -32 || ch == 224) {
                ch = Terminal::getChar();
                if (ch == 72) { index = (index - 1 + themes.size()) % themes.size(); changed = true; }
                else if (ch == 80) { index = (index + 1) % themes.size(); changed = true; }
            }
            #else
            if (ch == 27) {
                Terminal::getChar();
                ch = Terminal::getChar();
                if (ch == 'A') { index = (index - 1 + themes.size()) % themes.size(); changed = true; }
                else if (ch == 'B') { index = (index + 1) % themes.size(); changed = true; }
            }
            #endif
            else if (ch == 13 || ch == 10) return index;
            else if (ch == 'q' || ch == 'Q') exit(0);
        }
        Terminal::sleep_ms(50);
    }
}

void showTitle(const Theme &t) {
    Terminal::clear();
    vector<string> lines = {
        "  ███████╗███╗   ██╗ █████╗ ██╗  ██╗███████╗",
        "  ██╔════╝████╗  ██║██╔══██╗██║ ██╔╝██╔════╝",
        "  ███████╗██╔██╗ ██║███████║█████╔╝ █████╗  ",
        "  ╚════██║██║╚██╗██║██╔══██║██╔═██╗ ██╔══╝  ",
        "  ███████║██║ ╚████║██║  ██║██║  ██╗███████╗",
        "  ╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝"
    };
    
    for (int i = 0; i < (int)lines.size(); i++) {
        Terminal::gotoXY(15, 5 + i);
        Terminal::setColor(t.uiColor);
        cout << lines[i];
        Terminal::sleep_ms(80);
    }
    
    Terminal::gotoXY(20, 13);
    Terminal::setColor(t.foodColor);
    cout << "🎮 CROSS-PLATFORM EDITION 2025 🎮";
    
    Terminal::gotoXY(25, 16);
    Terminal::setColor(10);
    cout << "Press any key to start...";
    Terminal::resetColor();
    
    while (!Terminal::kbhit()) Terminal::sleep_ms(50);
    Terminal::getChar();
}

void showScoreboard(int score, int speed, int highScore, const Theme &t, bool rainbow) {
    Terminal::gotoXY(0, 0);
    Terminal::setColor(t.uiColor);
    cout << "╔════════════════════════════════════════════════════════════════════════════╗";
    Terminal::gotoXY(0, 1);
    cout << "║ ";
    Terminal::setColor(14);
    cout << "SCORE: ";
    Terminal::setColor(t.bodyColor);
    cout << score;
    Terminal::setColor(t.uiColor);
    cout << "  │  ";
    Terminal::setColor(14);
    cout << "HIGH: ";
    Terminal::setColor(13);
    cout << highScore;
    Terminal::setColor(t.uiColor);
    cout << "  │  ";
    Terminal::setColor(14);
    cout << "SPEED: ";
    Terminal::setColor(t.headColor);
    cout << speed << "/10";
    Terminal::setColor(t.uiColor);
    cout << "  │  " << t.name;
    if (rainbow) {
        Terminal::setColor(13);
        cout << " 🌈";
    }
    Terminal::gotoXY(0, 2);
    Terminal::setColor(t.uiColor);
    cout << "║ ";
    Terminal::setColor(10);
    cout << "WASD/Arrows";
    Terminal::setColor(8);
    cout << " │ ";
    Terminal::setColor(11);
    cout << "+/- Speed";
    Terminal::setColor(8);
    cout << " │ ";
    Terminal::setColor(13);
    cout << "R Rainbow";
    Terminal::setColor(8);
    cout << " │ ";
    Terminal::setColor(12);
    cout << "Q Quit";
    Terminal::gotoXY(0, 3);
    Terminal::setColor(t.uiColor);
    cout << "╚════════════════════════════════════════════════════════════════════════════╝";
    Terminal::resetColor();
}

int main() {
    srand((unsigned)time(0));
    Terminal::setupRawMode();
    Terminal::hideCursor();
    
    #ifdef _WIN32
    system("chcp 65001 >nul");
    #endif
    
    int currentTheme = selectTheme();
    Theme cur = themes[currentTheme];
    showTitle(cur);
    
    const int H = 25, W = 78, OX = 1, OY = 5;
    bool playAgain = true;
    int highScore = 0;
    bool rainbowMode = false;
    
    while (playAgain) {
        Terminal::clear();
        drawBox(W, H, OX, OY, cur.wallColor);
        
        vector<Point> blocks = generateBlocks(W, H, 6, 3, 6, 6);
        drawBlocks(blocks, OX, OY, cur.blockColor);
        
        // vector<Point> snake = {{15, 10}};
        vector<Point> snake = {
    {15, 10},   // head
    {14, 10}    // one more segment behind head
    // you can also add {13, 10} for a 3-block start
};
        int len = 3, score = 0;
        Point food = generateFood(W, H, snake, blocks);
        char dir = 'd';
        bool gameOver = false;
        int speed = 5, delay = 120 - (speed * 10);
        auto lastMove = chrono::steady_clock::now();
        int currentFoodIndex = rand() % cur.foodEmojis.size();
        
        while (!gameOver) {
            showScoreboard(score, speed, highScore, cur, rainbowMode);
            
            if (Terminal::kbhit()) {
                char k = Terminal::getChar();
                
                #ifdef _WIN32
                if (k == -32 || k == 224) {
                    k = Terminal::getChar();
                    if (k == 72 && dir != 's') dir = 'w';
                    else if (k == 80 && dir != 'w') dir = 's';
                    else if (k == 75 && dir != 'd') dir = 'a';
                    else if (k == 77 && dir != 'a') dir = 'd';
                }
                #else
                if (k == 27) {
                    Terminal::getChar();
                    k = Terminal::getChar();
                    if (k == 'A' && dir != 's') dir = 'w';
                    else if (k == 'B' && dir != 'w') dir = 's';
                    else if (k == 'D' && dir != 'd') dir = 'a';
                    else if (k == 'C' && dir != 'a') dir = 'd';
                }
                #endif
                else {
                    if ((k == 'w' || k == 'W') && dir != 's') dir = 'w';
                    else if ((k == 's' || k == 'S') && dir != 'w') dir = 's';
                    else if ((k == 'a' || k == 'A') && dir != 'd') dir = 'a';
                    else if ((k == 'd' || k == 'D') && dir != 'a') dir = 'd';
                    else if (k == '+' || k == '=') { if (speed < 10) speed++; }
                    else if (k == '-') { if (speed > 1) speed--; }
                    else if (k == 'r' || k == 'R') rainbowMode = !rainbowMode;
                    else if (k == 'q' || k == 'Q') { playAgain = false; gameOver = true; break; }
                }
            }
            
            delay = 120 - (speed * 10);
            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(now - lastMove).count() < delay) {
                Terminal::sleep_ms(1);
                continue;
            }
            lastMove = now;
            
            // Calculate new head position
            Point head = snake[0];
            if (dir == 'w') head.y--;
            else if (dir == 's') head.y++;
            else if (dir == 'a') head.x--;
            else head.x++;
            
            // Check collisions BEFORE modifying snake
            if (head.x <= 0 || head.x >= W - 1 || head.y <= 0 || head.y >= H - 1) {
                gameOver = true;
                break; // Exit immediately without changing snake
            }
            if (hitBlock(blocks, head)) {
                gameOver = true;
                break; // Exit immediately without changing snake
            }
            for (auto &s : snake) {
                if (s.x == head.x && s.y == head.y) {
                    gameOver = true;
                    break;
                }
            }
            if (gameOver) break; // Exit immediately without changing snake
            
            // Check food collision BEFORE adding head
            bool ateFood = false;
            if (head.x == food.x && head.y == food.y) {
                ateFood = true;
                score++;
                if (score > highScore) highScore = score;
                len++;
            }
            
            // Now safe to modify snake - add new head
            snake.insert(snake.begin(), head);
            
            // Remove tail if didn't eat food
            if (!ateFood) {
                Point tail = snake.back();
                snake.pop_back();
                // Clear tail position
                Terminal::gotoXY(tail.x + OX, tail.y + OY);
                cout << " ";
            }
            
            // Generate new food AFTER snake is updated (if food was eaten)
            if (ateFood) {
                // Clear old food position
                Terminal::gotoXY(food.x + OX, food.y + OY);
                cout << " ";
                // Generate new food away from current snake position
                Point newFood;
                bool validFood = false;
                int attempts = 0;
                while (!validFood && attempts < 1000) {
                    newFood = generateFood(W, H, snake, blocks);
                    // Make sure new food is not at same position as old food
                    if (newFood.x != food.x || newFood.y != food.y) {
                        validFood = true;
                    }
                    attempts++;
                }
                food = newFood;
                currentFoodIndex = rand() % cur.foodEmojis.size();
            }
            
            // Draw food
            Terminal::gotoXY(food.x + OX, food.y + OY);
            Terminal::setColor(cur.foodColor);
            cout << cur.foodEmojis[currentFoodIndex];
            
            // Draw snake
            for (int i = 0; i < (int)snake.size(); i++) {
                Terminal::gotoXY(snake[i].x + OX, snake[i].y + OY);
                if (i == 0) {
                    Terminal::setColor(cur.headColor);
                    cout << "◉";
                } else {
                    if (rainbowMode) {
                        int col = (i % 6) + 9;
                        Terminal::setColor(col);
                    } else {
                        Terminal::setColor(cur.bodyColor);
                    }
                    cout << "●";
                }
            }
            
            // Redraw blocks to prevent them from disappearing
            drawBlocks(blocks, OX, OY, cur.blockColor);
            Terminal::resetColor();
        }
        
        Terminal::gotoXY(OX + W / 2 - 10, H / 2 + OY);
        Terminal::setColor(12);
        cout << "╔══════════════════╗";
        Terminal::gotoXY(OX + W / 2 - 10, H / 2 + OY + 1);
        cout << "║   GAME OVER!   ║";
        Terminal::gotoXY(OX + W / 2 - 10, H / 2 + OY + 2);
        cout << "╚══════════════════╝";
        Terminal::gotoXY(OX + W / 2 - 10, H / 2 + OY + 4);
        Terminal::setColor(14);
        cout << "Score: ";
        Terminal::setColor(10);
        cout << score;
        Terminal::gotoXY(OX + W / 2 - 10, H / 2 + OY + 6);
        Terminal::setColor(11);
        cout << "R = Restart  Q = Quit";
        Terminal::resetColor();
        
        char ch;
        while (true) {
            if (Terminal::kbhit()) {
                ch = Terminal::getChar();
                if (ch == 'r' || ch == 'R') { playAgain = true; break; }
                else if (ch == 'q' || ch == 'Q') { playAgain = false; break; }
            }
            Terminal::sleep_ms(50);
        }
    }
    
    Terminal::clear();
    Terminal::showCursor();
    Terminal::restoreMode();
    Terminal::setColor(10);
    cout << "\n\n\tThanks for playing Snake Game 2025! 🐍\n\n";
    Terminal::resetColor();
    return 0;
}
