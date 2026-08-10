#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>

#ifdef _WIN32                       // Windows-specific includes
    #include <conio.h>
    #include <windows.h>
#else                               // Unix-like system includes
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
#endif

using namespace std;

struct Point { int x, y; };

class Terminal {                    // Cross-platform terminal handling
public:
    static void clear() {           // Clear the console
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    static void hideCursor() {      // Hide the console cursor
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
    static void showCursor() {          // Show the console cursor
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
    static void gotoXY(int x, int y) {          // Move cursor to (x, y)
        #ifdef _WIN32
            COORD pos = {(SHORT)x, (SHORT)y};       
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        #else
            cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
            cout.flush();
        #endif
    }
    static void setColor(int fg, int bg = 0) {   // Set text color (fg: foreground, bg: background)
        #ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg + (bg * 16));
        #else
            int ansi_fg = 30 + (fg % 8);
            int bright = (fg > 7) ? 1 : 0;
            cout << "\033[" << bright << ";" << ansi_fg << "m";
            cout.flush();
        #endif
    }
    static void resetColor() {                      // Reset text color to default
        #ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        #else
            cout << "\033[0m";
            cout.flush();
        #endif
    }
    static bool kbhit() {                       // Check if a key has been pressed
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
    static char getChar() {                 // Get a single character input
        #ifdef _WIN32
            return _getch();
        #else
            return getchar();
        #endif
    }
    static void setupRawMode() {            // Set terminal to raw mode (Unix-like systems)
        #ifndef _WIN32
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        #endif
    }
    static void restoreMode() {             // Restore terminal to original mode (Unix-like systems)
        #ifndef _WIN32
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag |= (ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
        #endif
    }
    static void sleep_ms(int ms) {          // Sleep for specified milliseconds
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}
};

struct Theme {                              // Game theme structure
    string name;
    int wallColor, blockColor, headColor, bodyColor, foodColor, uiColor;
    vector<string> foodEmojis;
    Theme(string n, int w, int b, int h, int bo, int f, int ui, vector<string> fe)
        : name(n), wallColor(w), blockColor(b), headColor(h),
          bodyColor(bo), foodColor(f), uiColor(ui), foodEmojis(fe) {}
};

vector<Theme> themes = {                    //Predefined game themes
    Theme("🌟 Neon Tech", 11, 8, 14, 10, 13, 15, {"🍏", "🍒", "🍇", "★", "✦"}), 
    Theme("🔥 Inferno", 12, 8, 14, 6, 9, 14, {"🍊", "🍋", "❤", "♦", "⬢"}),
    Theme("🌿 Jungle", 10, 8, 10, 2, 13, 15, {"🍌", "🍉", "☘", "♣", "❀"}),
    Theme("❄️  Ice", 11, 8, 15, 9, 11, 15, {"🍋", "☃", "❄", "✶", "✴"}),
    Theme("🎮 Retro", 14, 8, 14, 10, 13, 15, {"🍒", "■", "●", "⬤", "◉"})
};

// Generate block positions with clustering
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

// Draw blocks on the screen
void drawBlocks(const vector<Point> &blocks, int ox, int oy, int color) {
    Terminal::setColor(color);
    for (auto &b : blocks) {
        Terminal::gotoXY(b.x + ox, b.y + oy);
        cout << "▓";
    }
    Terminal::resetColor();
}

// Check if head hits any block
bool hitBlock(const vector<Point> &blocks, const Point &h) {
    for (auto &b : blocks) if (b.x == h.x && b.y == h.y) return true;
    return false;
}

// Generate food position ensuring accessibility
Point generateFood(int w, int h, const vector<Point> &snake1, const vector<Point> &snake2, const vector<Point> &blocks) {
    auto inside = [&](const Point& p) -> bool {
        return p.x > 0 && p.x < w - 1 && p.y > 0 && p.y < h - 1;
    };
    auto occupied = [&](const Point& p) -> bool {
        for (const auto& s : snake1) if (s.x == p.x && s.y == p.y) return true;
        for (const auto& s : snake2) if (s.x == p.x && s.y == p.y) return true;
        for (const auto& b : blocks) if (b.x == p.x && b.y == p.y) return true;
        return false;
    };
    auto freeNeighborCount = [&](const Point& p) -> int {
        static const int dx[4] = {1, -1, 0, 0};
        static const int dy[4] = {0, 0, 1, -1};
        int cnt = 0;
        for (int k = 0; k < 4; ++k) {
            Point q{p.x + dx[k], p.y + dy[k]};
            if (inside(q) && !occupied(q)) cnt++;
        }
        return cnt;
    };

    // Try random positions first
    Point f;
    bool ok = false;
    int attempts = 0;

    while (!ok && attempts < 1000) {
        f.x = rand() % (w - 4) + 2;
        f.y = rand() % (h - 4) + 2;
        if (!inside(f)) { attempts++; continue; }
        if (occupied(f)) { attempts++; continue; }
        if (freeNeighborCount(f) < 2) { attempts++; continue; }
        ok = true;
    }

    // If random attempts fail, do a systematic search
    if (ok) return f;
    for (int y = 1; y < h - 1; ++y)
        for (int x = 1; x < w - 1; ++x) {
            Point c{x, y};
            if (!inside(c)) continue;
            if (occupied(c)) continue;
            if (freeNeighborCount(c) >= 2) return c;
        }
    return snake1.front();
}

// Draw game boundary box
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

// Theme selection menu
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
            cout << "║    SELECT YOUR THEME          ║";
            Terminal::gotoXY(25, 5);
            cout << "╚═══════════════════════════════╝";
            for (int i = 0; i < (int)themes.size(); i++) {
                Terminal::gotoXY(25, 8 + i * 2);
                if (i == index) {
                    Terminal::setColor(14);
                    cout << "► " << themes[i].name;
                } else {
                    Terminal::setColor(8);
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

// Show game title screen
void showTitle(const Theme &t) {
    Terminal::clear();
    // Display animated title
    vector<string> lines = {
        "  ███████╗███╗   ██╗ █████╗ ██╗  ██╗███████╗",
        "  ██╔════╝████╗  ██║██╔══██╗██║ ██╔╝██╔════╝",
        "  ███████╗██╔██╗ ██║███████║█████╔╝ █████╗  ",
        "  ╚════██║██║╚██╗██║██╔══██║██╔═██╗ ██╔══╝  ",
        "  ███████║██║ ╚████║██║  ██║██║  ██╗███████╗",
        "  ╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝"
    };

    // Animate title display
    for (int i = 0; i < (int)lines.size(); i++) {
        Terminal::gotoXY(15, 5 + i);
        Terminal::setColor(t.uiColor);
        cout << lines[i];
        Terminal::sleep_ms(80);
    }
    // Display subtitle and prompt
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

// Show scoreboard at the top
void showScoreboard(int score1, int score2, int speed, int highScore, const Theme &t, bool rainbow) {
    Terminal::gotoXY(0, 0);
    Terminal::setColor(t.uiColor);
    cout << "╔════════════════════════════════════════════════════════════════════════════╗";
    Terminal::gotoXY(0, 1);
    cout << "║ ";
    Terminal::setColor(14);
    cout << "P1: ";
    Terminal::setColor(t.bodyColor);
    cout << score1;
    Terminal::setColor(t.uiColor);
    cout << "  │  ";
    Terminal::setColor(14);
    cout << "P2: ";
    Terminal::setColor(t.bodyColor);
    cout << score2;
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
    cout << "P1:Arrows │ P2:WASD";
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
    cout << "P Pause │ C Continue │ Q Quit";
    Terminal::gotoXY(0, 3);
    Terminal::setColor(t.uiColor);
    cout << "╚════════════════════════════════════════════════════════════════════════════╝";
    Terminal::resetColor();
}

// Main game loop
int main() {
    // Initialize random seed and terminal
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

    // Game restart loop
    while (playAgain) {
        Terminal::clear();
        drawBox(W, H, OX, OY, cur.wallColor);
        vector<Point> blocks = generateBlocks(W, H, 6, 3, 6, 6);
        drawBlocks(blocks, OX, OY, cur.blockColor);
        vector<Point> snake1 = {{15, 8}, {14, 8}};
        vector<Point> snake2 = {{15, 16}, {14, 16}};
        int score1 = 0;
        int score2 = 0;
        Point food = generateFood(W, H, snake1, snake2, blocks);
        char dir1 = 'd';
        char dir2 = 'd';
        bool gameOver = false;
        bool paused = false;
        int speed = 5, delay = 120 - (speed * 10);
        auto lastMove = chrono::steady_clock::now();
        int currentFoodIndex = rand() % cur.foodEmojis.size();
        string loserMsg = "";

        // Main game loop
        while (!gameOver) {
            showScoreboard(score1, score2, speed, highScore, cur, rainbowMode);

            if (Terminal::kbhit()) {
                char k = Terminal::getChar();
                #ifdef _WIN32
                if (k == -32 || k == 224) {
                    k = Terminal::getChar();
                    if (k == 72 && dir1 != 's') dir1 = 'w';
                    else if (k == 80 && dir1 != 'w') dir1 = 's';
                    else if (k == 75 && dir1 != 'd') dir1 = 'a';
                    else if (k == 77 && dir1 != 'a') dir1 = 'd';
                }
                #else
                if (k == 27) {
                    Terminal::getChar();
                    k = Terminal::getChar();
                    if (k == 'A' && dir1 != 's') dir1 = 'w';
                    else if (k == 'B' && dir1 != 'w') dir1 = 's';
                    else if (k == 'D' && dir1 != 'd') dir1 = 'a';
                    else if (k == 'C' && dir1 != 'a') dir1 = 'd';
                }
                #endif
                else {
                    if ((k == 'w' || k == 'W') && dir2 != 's') dir2 = 'w';
                    else if ((k == 's' || k == 'S') && dir2 != 'w') dir2 = 's';
                    else if ((k == 'a' || k == 'A') && dir2 != 'd') dir2 = 'a';
                    else if ((k == 'd' || k == 'D') && dir2 != 'a') dir2 = 'd';
                    else if (k == '+' || k == '=') { if (speed < 10) speed++; }
                    else if (k == '-') { if (speed > 1) speed--; }
                    else if (k == 'r' || k == 'R') rainbowMode = !rainbowMode;
                    else if (k == 'p' || k == 'P') paused = true;
                    else if (k == 'c' || k == 'C') paused = false;
                    else if (k == 'q' || k == 'Q') { playAgain = false; gameOver = true; break; }
                }
            }

            delay = 120 - (speed * 10);

            // Pause screen logic
            if (paused) {
                Terminal::gotoXY(OX + W / 2 - 8, H / 2 + OY);
                Terminal::setColor(14);
                cout << "⏸️  GAME PAUSED  ⏸️";
                Terminal::resetColor();

                while (paused) {
                    if (Terminal::kbhit()) {
                        char key = Terminal::getChar();
                        if (key == 'c' || key == 'C') {
                            paused = false;
                            Terminal::gotoXY(OX + W / 2 - 8, H / 2 + OY);
                            cout << "                   ";
                            break;
                        }
                        else if (key == 'q' || key == 'Q') {
                            gameOver = true;
                            playAgain = false;
                            break;
                        }
                    }
                    Terminal::sleep_ms(100);
                }
                if (gameOver) break;
                continue;
            }

            // Movement timing control
            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(now - lastMove).count() < delay) {
                Terminal::sleep_ms(1);
                continue;
            }
            lastMove = now;

            Point head1 = snake1[0];
            if (dir1 == 'w') head1.y--;
            else if (dir1 == 's') head1.y++;
            else if (dir1 == 'a') head1.x--;
            else head1.x++;

            Point head2 = snake2[0];
            if (dir2 == 'w') head2.y--;
            else if (dir2 == 's') head2.y++;
            else if (dir2 == 'a') head2.x--;
            else head2.x++;

            bool ateFood1 = (head1.y == food.y && std::abs(head1.x - food.x) <= 1);
            bool ateFood2 = (!ateFood1 && head2.y == food.y && std::abs(head2.x - food.x) <= 1);

            bool p1Lost = false;
            bool p2Lost = false;

            if (head1.x == head2.x && head1.y == head2.y) {
                p1Lost = true;
                p2Lost = true;
            }

            if (head1.x <= 0 || head1.x >= W - 1 || head1.y <= 0 || head1.y >= H - 1) p1Lost = true;
            if (hitBlock(blocks, head1)) p1Lost = true;

            if (head2.x <= 0 || head2.x >= W - 1 || head2.y <= 0 || head2.y >= H - 1) p2Lost = true;
            if (hitBlock(blocks, head2)) p2Lost = true;

            int checkLen1 = ateFood1 ? (int)snake1.size() : (int)snake1.size() - 1;
            int checkLen2 = ateFood2 ? (int)snake2.size() : (int)snake2.size() - 1;

            for (int i = 0; i < checkLen1; i++) {
                if (snake1[i].x == head1.x && snake1[i].y == head1.y) p1Lost = true;
                if (snake1[i].x == head2.x && snake1[i].y == head2.y) p2Lost = true;
            }

            for (int i = 0; i < checkLen2; i++) {
                if (snake2[i].x == head1.x && snake2[i].y == head1.y) p1Lost = true;
                if (snake2[i].x == head2.x && snake2[i].y == head2.y) p2Lost = true;
            }

            if (p1Lost || p2Lost) {
                if (p1Lost && p2Lost) loserMsg = "BOTH PLAYERS LOST!";
                else if (p1Lost) loserMsg = "PLAYER 1 LOST!";
                else loserMsg = "PLAYER 2 LOST!";
                gameOver = true;
                break;
            }

            if (ateFood1) {
                score1++;
                if (score1 > highScore) highScore = score1;
            }

            if (ateFood2) {
                score2++;
                if (score2 > highScore) highScore = score2;
            }

            snake1.insert(snake1.begin(), head1);
            if (!ateFood1) {
                Point tail = snake1.back();
                snake1.pop_back();
                Terminal::gotoXY(tail.x + OX, tail.y + OY);
                cout << " ";
            }

            snake2.insert(snake2.begin(), head2);
            if (!ateFood2) {
                Point tail = snake2.back();
                snake2.pop_back();
                Terminal::gotoXY(tail.x + OX, tail.y + OY);
                cout << " ";
            }

            if (ateFood1 || ateFood2) {
                Terminal::gotoXY(food.x + OX, food.y + OY);
                cout << " ";
                Point newFood;
                bool validFood = false;
                int attempts2 = 0;
                while (!validFood && attempts2 < 1000) {
                    newFood = generateFood(W, H, snake1, snake2, blocks);
                    if (newFood.x != food.x || newFood.y != food.y) validFood = true;
                    attempts2++;
                }
                food = newFood;
                currentFoodIndex = rand() % cur.foodEmojis.size();
            }

            Terminal::gotoXY(food.x + OX, food.y + OY);
            Terminal::setColor(cur.foodColor);
            cout << cur.foodEmojis[currentFoodIndex];

            for (int i = 0; i < (int)snake1.size(); i++) {
                Terminal::gotoXY(snake1[i].x + OX, snake1[i].y + OY);
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

            for (int i = 0; i < (int)snake2.size(); i++) {
                Terminal::gotoXY(snake2[i].x + OX, snake2[i].y + OY);
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

            drawBlocks(blocks, OX, OY, cur.blockColor);
            Terminal::resetColor();
        }

        // Game over screen
        Terminal::gotoXY(OX + W / 2 - 12, H / 2 + OY);
        Terminal::setColor(12);
        cout << "╔════════════════════════╗";
        Terminal::gotoXY(OX + W / 2 - 12, H / 2 + OY + 1);
        cout << "║   GAME OVER!           ║";
        Terminal::gotoXY(OX + W / 2 - 12, H / 2 + OY + 2);
        cout << "╚════════════════════════╝";
        Terminal::gotoXY(OX + W / 2 - 12, H / 2 + OY + 3);
        Terminal::setColor(14);
        cout << " " << loserMsg;
        Terminal::gotoXY(OX + W / 2 - 12, H / 2 + OY + 5);
        Terminal::setColor(14);
        cout << "P1: ";
        Terminal::setColor(10);
        cout << score1;
        Terminal::setColor(14);
        cout << " │ P2: ";
        Terminal::setColor(10);
        cout << score2;
        Terminal::gotoXY(OX + W / 2 - 12, H / 2 + OY + 7);
        Terminal::setColor(11);
        cout << "R = Restart  Q = Quit";
        Terminal::resetColor();

        // Wait for restart or quit input
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
