#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>
using namespace std;

struct Point { int x; int y; };

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoXY(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(console, coord);
}
void setColor(int color) { SetConsoleTextAttribute(console, color); }
void hideCursor() { CONSOLE_CURSOR_INFO c; GetConsoleCursorInfo(console, &c); c.bVisible = false; SetConsoleCursorInfo(console, &c); }

void setupConsole() {
    system("chcp 65001 >nul");
    SetConsoleOutputCP(65001);
    system("mode 120, 40");
    hideCursor();
}

// ---------- Theme structure ----------
struct Theme {
    string name;
    int wallColor;
    int blockColor;
    int headColor;
    int bodyColor;
    int foodColor;
    int uiColor;
    string foodChar;

    Theme(string n,int w,int b,int h,int bo,int f,int ui,string fc)
        : name(n), wallColor(w), blockColor(b), headColor(h),
          bodyColor(bo), foodColor(f), uiColor(ui), foodChar(fc) {}
};

vector<Theme> themes = {
    Theme("Neon Tech", 11, 8, 14, 10, 12, 15, "@"),
    Theme("Inferno",   12, 8, 12, 6, 12, 14, "@"),
    Theme("Jungle",    10, 8, 10, 2, 12, 15, "@"),
    Theme("Ice",        9, 8,  9, 11,  3, 15, "@"),
    Theme("Retro",      6, 8, 14, 10, 13, 15, "@")
};

int currentThemeIndex = 0;
bool padmaMode = false;

// ---------- Utility ----------
void msleep(int ms) { Sleep(ms); }

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
        int pattern = rand() % 4;
        int count = rand() % (blockMax - blockMin + 1) + blockMin;
        for (int j = 0; j < count; j++) {
            Point p = c;
            switch (pattern) {
            case 0: p.x += (j < 2 ? j : 0); p.y += (j < 2 ? 0 : j - 1); break;
            case 1: p.x += (j < 3 ? j : 1); p.y += (j < 3 ? 0 : j - 2); break;
            case 2: p.x += (j == 0 ? -1 : (j == 1 ? 0 : 1)); p.y += (j < 3 ? 0 : 1); break;
            default: p.x += j; break;
            }
            if (p.x > 0 && p.x < w - 1 && p.y > 0 && p.y < h - 1)
                blocks.push_back(p);
        }
    }
    return blocks;
}

void drawBlocks(const vector<Point> &blocks, int ox, int oy, int color) {
    setColor(color);
    for (auto &b : blocks) { gotoXY(b.x + ox, b.y + oy); cout << "▒"; }
    setColor(15);
}

bool hitBlock(const vector<Point> &blocks, const Point &h) {
    for (auto &b : blocks) if (b.x == h.x && b.y == h.y) return true;
    return false;
}

Point generateFood(int w, int h, const vector<Point> &snake, const vector<Point> &blocks) {
    Point f; bool ok = false;
    while (!ok) {
        ok = true;
        f.x = rand() % (w - 2) + 1;
        f.y = rand() % (h - 2) + 1;
        for (auto &s : snake) if (s.x == f.x && s.y == f.y) ok = false;
        for (auto &b : blocks) if (b.x == f.x && b.y == f.y) ok = false;
    }
    return f;
}

void drawBox(int w, int h, int ox, int oy, int color) {
    setColor(color);
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            gotoXY(c + ox, r + oy);
            if (r == 0 || r == h - 1 || c == 0 || c == w - 1)
                cout << "█";
            else
                cout << " ";
        }
    }
    setColor(15);
}

// ---------- Simple Theme Selection ----------
int selectTheme() {
    int index = 0;
    bool changed = true;

    while (true) {
        if (changed) {
            system("cls");
            gotoXY(0, 3);
            setColor(15);
            cout << "\n\n\t\tSelect a Theme:\n\n";

            for (int i = 0; i < (int)themes.size(); i++) {
                gotoXY(25, 7 + i);
                if (i == index) {
                    setColor(15);
                    cout << "> ";
                } else {
                    cout << "  ";
                }

                // theme name in its unique color
                setColor(themes[i].uiColor);
                cout << themes[i].name;
            }

            setColor(15);
            gotoXY(0, 14);
            cout << "\n\n\tUse ↑ ↓ to navigate  |  Press ENTER to select";
            changed = false;
        }

        if (_kbhit()) {
            char ch = _getch();
            if (ch == -32 || ch == 224) {
                ch = _getch();
                if (ch == 72) { // up
                    index = (index - 1 + themes.size()) % themes.size();
                    changed = true;
                } else if (ch == 80) { // down
                    index = (index + 1) % themes.size();
                    changed = true;
                }
            } else if (ch == 13) {
                return index; // Enter
            }
        }
        Sleep(60);
    }
}


// ---------- UI + Title ----------
void showAnimatedTitle(const Theme &t) {
    system("cls");
    vector<string> lines = {
        " ███████╗ ███╗   ██╗ █████╗ ██╗  ██╗███████╗ ",
        " ██╔════╝ ████╗  ██║██╔══██╗██║ ██╔╝██╔════╝ ",
        " ███████╗ ██╔██╗ ██║███████║█████╔╝ █████╗   ",
        " ╚════██║ ██║╚██╗██║██╔══██║██╔═██╗ ██╔══╝   ",
        " ███████║ ██║ ╚████║██║  ██║██║  ██╗███████╗ ",
        " ╚══════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝ "
    };
    int x = 8, y = 3;
    for (int i = 0; i < (int)lines.size(); i++) {
        setColor(t.uiColor);
        gotoXY(x, y + i);
        cout << lines[i];
        msleep(50);
    }
    setColor(t.foodColor);
    gotoXY(x + 10, y + (int)lines.size() + 1);
    cout << "   S N A K E   G A M E   2 0 2 5   @  Classic Deluxe Edition";
    setColor(15);
    gotoXY(x, y + (int)lines.size() + 4);
    cout << "\tPress any key to continue...";
    _getch();
}

void showScoreboard(int score, int speed, const Theme &t) {
    setColor(t.uiColor);
    gotoXY(0, 0);
    cout << "╔════════════════════════════════════════════════════════════════════════╗";
    gotoXY(0, 1);
    cout << "║ SCORE: "; setColor(t.bodyColor); cout << score;
    setColor(t.uiColor);
    cout << "   SPEED: "; setColor(t.headColor); cout << speed << "/10   ";
    setColor(t.uiColor);
    cout << " THEME: " << t.name;
    gotoXY(0, 2);
    cout << "║ CONTROLS: W/A/S/D | [+/-] Speed | [P] Padma | [Q] Quit ║";
    gotoXY(0, 3);
    cout << "╚════════════════════════════════════════════════════════════════════════╝";
    setColor(15);
}

void playGameOverTune() {
    vector<pair<int,int>> n = {
        {659,120},{523,180},{440,300},{0,80},
        {440,120},{0,60},{330,400},{0,80},
        {220,300},{0,80},{196,250},{0,50},{176,200}
    };
    for (auto &x : n) {
        if (x.first>0) Beep(x.first,x.second);
        else Sleep(x.second);
    }
}

int main() {
    srand((unsigned)time(0));
    setupConsole();

    currentThemeIndex = selectTheme();
    Theme cur = themes[currentThemeIndex];
    showAnimatedTitle(cur);

    const int H=25,W=80,OX=10,OY=5;
    bool again=true;
    while(again){
        system("cls");
        drawBox(W,H,OX,OY,cur.wallColor);
        vector<Point> blocks = generateBlocks(W,H,5,3,5,6);
        drawBlocks(blocks,OX,OY,cur.blockColor);

        vector<Point> snake={{15,10}};
        int len=3,score=0;
        Point food=generateFood(W,H,snake,blocks);
        char dir='d';
        bool over=false;
        int spd=5,delay=120-(spd*10);
        auto last=chrono::steady_clock::now();

        while(!over){
            showScoreboard(score,spd,cur);
            if(_kbhit()){
                char k=_getch();
                if(k==-32||k==224){
                    k=_getch();
                    if(k==72&&dir!='s')dir='w';
                    else if(k==80&&dir!='w')dir='s';
                    else if(k==75&&dir!='d')dir='a';
                    else if(k==77&&dir!='a')dir='d';
                }else{
                    if((k=='w'||k=='W')&&dir!='s')dir='w';
                    else if((k=='s'||k=='S')&&dir!='w')dir='s';
                    else if((k=='a'||k=='A')&&dir!='d')dir='a';
                    else if((k=='d'||k=='D')&&dir!='a')dir='d';
                    else if(k=='+'||k=='='){if(spd<10)spd++;}
                    else if(k=='-'){if(spd>1)spd--;}
                    else if(k=='q'||k=='Q'){again=false;over=true;break;}
                    else if(k=='p'||k=='P'){padmaMode=!padmaMode;}
                }
            }

            delay=120-(spd*10);
            auto now=chrono::steady_clock::now();
            if(chrono::duration_cast<chrono::milliseconds>(now-last).count()<delay){Sleep(1);continue;}
            last=now;

            Point tail=snake.back();
            gotoXY(tail.x+OX,tail.y+OY); cout<<' ';

            Point head=snake[0];
            if(dir=='w')head.y--; else if(dir=='s')head.y++; else if(dir=='a')head.x--; else head.x++;
            if(head.x<=0||head.x>=W-1||head.y<=0||head.y>=H-1)over=true;
            if(hitBlock(blocks,head))over=true;
            for(auto &s:snake) if(s.x==head.x&&s.y==head.y) over=true;
            snake.insert(snake.begin(),head);

            if(head.x==food.x&&head.y==food.y){
                score++;
                Beep(850,70);
                gotoXY(food.x+OX,food.y+OY); cout<<' ';
                food=generateFood(W,H,snake,blocks);
                len++;
            }else{
                while(snake.size()>len) snake.pop_back();
            }

            setColor(cur.foodColor);
            gotoXY(food.x+OX,food.y+OY);
            cout<<cur.foodChar;

            for(int i=0;i<(int)snake.size();i++){
                gotoXY(snake[i].x+OX,snake[i].y+OY);
                if(i==0){setColor(14);cout<<"■";}
                else{
                    if(padmaMode){int col=(9+(i%7))%15+1;setColor(col);cout<<"●";}
                    else {setColor(10);cout<<"●";}
                }
            }
        }

        for(int i=0;i<3;i++){system("color 4F");Sleep(80);system("color 0A");Sleep(80);}
        playGameOverTune();

        gotoXY(OX,H+OY+2);
        setColor(12);
        cout<<"\n\n\t===== GAME OVER =====\n";
        setColor(15);
        cout<<"\tFinal Score: "; setColor(10); cout<<score<<"\n\n";
        setColor(14);
        cout<<"\tPress [R] Restart or [Q] Quit\n";
        char ch;
        while(true){
            ch=_getch();
            if(ch=='r'||ch=='R'){again=true;break;}
            else if(ch=='q'||ch=='Q'){again=false;break;}
        }
    }
    system("cls");
    setColor(10);
    cout<<"Thank you for playing Snake Game 2025 - Padma Shree Classic Edition @!\n";
    setColor(15);
    return 0;
}
