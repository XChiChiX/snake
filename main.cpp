#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

#define Map_Width 50
#define Map_Height 30
#define Left_Edge 30

class Snake{
private:
    int x;
    int y;
    int player;
    bool eat = false;
    class Body{
    private:
        int x;
        int y;
    public:
        int xpos(){
            return x;
        }
        int ypos(){
            return y;
        }
        void setxy(int a, int b){
            x = a;
            y = b;
        }
    };
    deque<Body> body;
public:
    Body GetBody(int i){
        return body[i];
    }
    void gotoxy(int xpos, int ypos)
    {
        COORD scrn;
        HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
        scrn.X = xpos; scrn.Y = ypos;
        SetConsoleCursorPosition(hOuput,scrn);
    }
    void Init(){
        player = 4;
        Body head;
        head.setxy(1, 1);
        body.clear();
        body.push_front(head);
    }
    int speed(){
        return (50 + length()*10);
    }
    void move(char key){
        Body head = body.front();
        if(!eat){
            Body tail = body.back();
            gotoxy(tail.xpos() + Left_Edge, tail.ypos());
            cout << " ";
            body.pop_back();
        }
        if(key == 'a'){
            head.setxy(head.xpos()-1, head.ypos());
        }
        else if(key == 'd'){
            head.setxy(head.xpos()+1, head.ypos());
        }
        else if(key == 'w'){
            head.setxy(head.xpos(), head.ypos()-1);
        }
        else if(key == 's'){
            head.setxy(head.xpos(), head.ypos()+1);
        }
        gotoxy(head.xpos() + Left_Edge, head.ypos());
        cout << "S";
        body.push_front(head);
        eat = false;
    }
    int length(){
        return body.size();
    }
    int HP(){
        return player;
    }
    void hurt(){
        --player;
    }
    void grow(){
        eat = true;
    }
    bool die(){
        Body head = body.front();
        if(player <= 0) return true;
        if(head.xpos() < 1 || head.xpos() > Map_Width-1 || head.ypos() < 1 || head.ypos() > Map_Height-1) return true;
        for(int i = 1; i < body.size(); ++i){
            if(head.xpos() == body[i].xpos() && head.ypos() == body[i].ypos()){
                return true;
            }
        }
        return false;
    }
}snake;

class Food{
private:
    bool flag;
    int x;
    int y;
public:
    int xpos(){
        return x;
    }
    int ypos(){
        return y;
    }
    void gotoxy(int xpos, int ypos)
    {
        COORD scrn;
        HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
        scrn.X = xpos; scrn.Y = ypos;
        SetConsoleCursorPosition(hOuput,scrn);
    }
    void InitFood(){
        flag = true;
    }
    void CreateFood(){
        if(flag){
            x = rand() % (Map_Width-2) + 1;
            y = rand() % (Map_Height-2) + 1;
            for(int i = 0; i < snake.length(); ++i){
                if(x == snake.GetBody(i).xpos() && y == snake.GetBody(i).ypos()){
                    x = rand() % (Map_Width-2) + 1;
                    y = rand() % (Map_Height-2) + 1;
                }
            }
            bool CreateError = false;
            for(int i = 0; i < snake.length(); ++i){
                if(x == snake.GetBody(i).xpos() && y == snake.GetBody(i).ypos()){
                    CreateError = true;
                    break;
                }
            }
            if(CreateError) flag = true;
            else {
                gotoxy(x + Left_Edge, y);
                cout << "f";
                flag = false;
            }
        }
    }
    void EatFood(){
        if(x == snake.GetBody(0).xpos() && y == snake.GetBody(0).ypos()){
            snake.grow();
            flag = true;
        }
    }
    bool exist(){
        if(flag) return false;
        else return true;
    }
}food;

class Bomb{
private:
    bool flag;
    int x;
    int y;
public:
    int xpos(){
        return x;
    }
    int ypos(){
        return y;
    }
    void gotoxy(int xpos, int ypos)
    {
        COORD scrn;
        HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
        scrn.X = xpos; scrn.Y = ypos;
        SetConsoleCursorPosition(hOuput,scrn);
    }
    void InitBomb(){
        flag = true;
    }
    void CreateBomb(){
        if(flag){
            x = rand() % (Map_Width-2) + 1;
            y = rand() % (Map_Height-2) + 1;
            for(int i = 0; i < snake.length(); ++i){
                if((x == snake.GetBody(i).xpos() && y == snake.GetBody(i).ypos()) || (x == food.xpos() && y == food.ypos())){
                    x = rand() % (Map_Width-2) + 1;
                    y = rand() % (Map_Height-2) + 1;
                }
            }
            bool CreateError = false;
            for(int i = 0; i < snake.length(); ++i){
                if((x == snake.GetBody(i).xpos() && y == snake.GetBody(i).ypos()) || (x == food.xpos() && y == food.ypos())){
                    CreateError = true;
                    break;
                }
            }
            if(CreateError) flag =true;
            else {
                gotoxy(x + Left_Edge, y);
                cout << "B";
                flag = false;
            }
        }
    }
    void Eatbomb(){
        if(x == snake.GetBody(0).xpos() && y == snake.GetBody(0).ypos()){
            snake.hurt();
            flag = true;
        }
    }
    bool exist(){
        if(flag) return false;
        else return true;
    }
}bomb[3];

void gotoxy(int xpos, int ypos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos; scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput,scrn);
}

void SetColor(int color = 7)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}

void clearScreen(){    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };    // home for the cursor
    SetConsoleCursorPosition( hConsole, coordScreen );
}

void full_screen(){
	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);

	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE ) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}

void HideCursor(){
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


void InitScene();
void update();
void Instructions();
void start_UI();
void die_UI();
void music_UI();
void state1();
void state2();

int score, gametime;
clock_t sec, startTime;
char Key;
bool EndGame, stage1Clear;

int main(){
	// system("mode con cols=200 lines=150");
	full_screen();
    HideCursor();
    system("title 在C++做遊戲是否搞錯了什麼");
    system("color 0F"); //背景顏色 字體顏色
    srand((unsigned int)time(NULL));
    while(true){
        PlaySound(TEXT("./musics/touhou0.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
        start_UI();
        if(EndGame) break;
        PlaySound(TEXT("./musics/touhou1.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
        Key = 'd';
        score = 0;
        stage1Clear = false;
        InitScene();
        snake.Init();
        gametime = clock();
        state1();
        if(EndGame) break;
        PlaySound(TEXT("./musics/touhou2.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
        if(stage1Clear) state2();
        if(EndGame) break;

    }
    // system("pause");
    return 0;
}

void state1(){
    food.InitFood();
    while(true){
        food.CreateFood();
        if(_kbhit()){
            Key = _getch();
        }
        food.EatFood();
        snake.move(Key);
        update();
        if(snake.die()){
            system("color 0F");
            system("CLS");
            PlaySound(TEXT("./musics/bgm2.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
            die_UI();
            break;
        }
        sec = clock();
        if(Key == 'w' || Key == 's') Sleep(snake.speed());
        else Sleep(snake.speed()/2);
        if(sec >= gametime){
            gametime += 1000;
            score += (rand() % 100 + 10)*snake.length();
        }
        if(sec-startTime >= 90000){
            stage1Clear = true;
            system("color 0f");
            system("CLS");
            InitScene();
            gotoxy(Left_Edge + 5, 5);
            cout << "Stage Clear";
            Sleep(3000);
            system("CLS");
            break;
        }
        // system("CLS");
    }
}

void state2(){
    InitScene();
    food.InitFood();
    for(int i = 0; i < 3; ++i){
        bomb[i].InitBomb();
    }
    while(true){
        food.CreateFood();
        for(int i = 0; i < 3; ++i){
            bomb[i].CreateBomb();
        }
        if(_kbhit()){
            Key = _getch();
        }
        food.EatFood();
        for(int i = 0; i < 3; ++i) bomb[i].Eatbomb();
        snake.move(Key);
        update();
        if(snake.die()){
            system("color 0F");
            system("CLS");
            PlaySound(TEXT("./musics/bgm2.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
            die_UI();
            break;
        }
        sec = clock();
        if(Key == 'w' || Key == 's') Sleep(snake.speed());
        else Sleep(snake.speed()/2);
        if(sec >= gametime){
            gametime += 1000;
            score += (rand() % 100 + 10)*snake.length();
        }
        if(sec-startTime >= 180000){
            system("color 0f");
            system("CLS");
            InitScene();
            gotoxy(Left_Edge + 5, 5);
            cout << "Stage Clear";
            gotoxy(Left_Edge + 5, 7);
            cout << "沒有下一關";
            Sleep(5000);
            system("CLS");
            break;
        }
        // system("CLS");
    }
}

void Instructions(){
    gotoxy(Left_Edge + Map_Width/2, 10);
    cout << "w: 向上移動";
    gotoxy(Left_Edge + Map_Width/2, 12);
    cout << "s: 向下移動";
    gotoxy(Left_Edge + Map_Width/2, 14);
    cout << "a: 向左移動";
    gotoxy(Left_Edge + Map_Width/2, 16);
    cout << "d: 向右移動";
    gotoxy(Left_Edge + Map_Width/2, 18);
    cout << "撞到自己身體或邊界即遊戲結束";

    gotoxy(Left_Edge + Map_Width/2 + 5, 20);
    cout << "按 x 返回";
    cout << "\n";

    while(true){
        if(_kbhit()){
            Key = _getch();
            if(Key == 'x'){
                system("CLS");
                start_UI();
                break;
            }
        }
    }
}

void start_UI(){
    gotoxy(Left_Edge + Map_Width/2, 10);
    cout << "      |              \\          ";
    gotoxy(Left_Edge + Map_Width/2, 11);
    cout << "- - - - - - -     ---------      ";
    gotoxy(Left_Edge + Map_Width/2, 12);
    cout << "   ___|___            |          ";
    gotoxy(Left_Edge + Map_Width/2, 13);
    cout << "   |  |  |            | _____    ";
    gotoxy(Left_Edge + Map_Width/2, 14);
    cout << "   -------            |     |";
    gotoxy(Left_Edge + Map_Width/2, 15);
    cout << "   |  |  |           /      |    ";
    gotoxy(Left_Edge + Map_Width/2, 16);
    cout << "   -------          /       |    ";
    gotoxy(Left_Edge + Map_Width/2, 17);
    cout << "     /| \\         /        /    ";
    gotoxy(Left_Edge + Map_Width/2, 18);
    cout << "    / |  \\               \\/    ";
    gotoxy(Left_Edge + Map_Width + 5, 19);
    cout << "貪食蛇";

    gotoxy(Left_Edge + Map_Width + 15, 10);
    cout << "按 a 遊戲說明";

    gotoxy(Left_Edge + Map_Width + 15, 12);
    cout << "按 z 開始遊戲";
    gotoxy(Left_Edge + Map_Width + 15, 14);
    cout << "按 x 結束遊戲";
    gotoxy(Left_Edge + Map_Width + 15, 16);
    cout << "按 m 聽歌";

    while(true){
        if(_kbhit()){
            Key = _getch();
            if(Key == 'z'){
                EndGame = false;
                startTime = clock();
                system("CLS");
                return;
            }
            else if(Key == 'x'){
                EndGame = true;
                return;
            }
            else if(Key == 'a'){
                system("CLS");
                Instructions();
                return;
            }
            else if(Key == 'm'){
                system("CLS");
                music_UI();
                return;
            }
        }
    }
}

void die_UI(){
    gotoxy(Left_Edge + Map_Width/2, 5);
    cout << "你已經死了!!";
    gotoxy(Left_Edge + Map_Width/2, 7);
    cout << "存活時間: " << (sec-startTime)/1000 << "秒";
    gotoxy(Left_Edge + Map_Width/2, 9);
    cout << "分數: " << score;
    gotoxy(Left_Edge + Map_Width + 15, 10);
    cout << "按 z 返回主選單";
    gotoxy(Left_Edge + Map_Width + 15, 12);
    cout << "按 x 結束遊戲";
    cout << "\n";

    while(true){
        if(_kbhit()){
            Key = _getch();
            if(Key == 'z'){
                EndGame = false;
                system("CLS");
                break;
            }
            else if(Key == 'x'){
                EndGame = true;
                break;
            }
        }
    }
}

void music_UI(){
    gotoxy(Left_Edge + Map_Width/2, 10);
    cout << "1: Only my railgun";
    gotoxy(Left_Edge + Map_Width/2, 12);
    cout << "2: 戀上換裝娃娃ED";
    gotoxy(Left_Edge + Map_Width/2, 14);
    cout << "3: 表裏一体";
    gotoxy(Left_Edge + Map_Width/2, 16);
    cout << "4: 沒";

    gotoxy(Left_Edge + Map_Width/2 + 5, 20);
    cout << "按 x 返回";
    cout << "\n";

    while(true){
        if(_kbhit()){
            Key = _getch();
            if(Key == 'x'){
                system("CLS");
                start_UI();
                break;
            }
            else if(Key == 't'){
                Key = _getch();
                if(Key == 'r'){
                    Key = _getch();
                    if(Key == 'r'){
                        PlaySound(TEXT("./musics/bgm4.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
                    }
                }
            }
            else{
                if(Key == '1'){
                    PlaySound(TEXT("./musics/bgm1.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
                }
                else if(Key == '2'){
                    PlaySound(TEXT("./musics/bgm2.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
                }
                else if(Key == '3'){
                    PlaySound(TEXT("./musics/bgm3.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
                }
                else {
                    PlaySound(TEXT("./musics/bgm0.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP);
                }
            }
        }
    }
}

void InitScene(){
    for(int i = 0; i < Map_Width + 1; ++i){
        gotoxy(Left_Edge + i, 0);
        cout << "-";
        gotoxy(Left_Edge + i, Map_Height);
        cout << "-";
    }
    for(int i = 1; i < Map_Height; ++i){
        gotoxy(Left_Edge - 1, i);
        cout << "|";
        gotoxy(Left_Edge + Map_Width, i);
        cout << "|";
    }
    SetColor(17);
    for(int i = 1; i < 30; ++i){
        for(int j = 0; j < Map_Height+5; ++j){
            gotoxy(Left_Edge + Map_Width + j + 1, i);
            cout << " ";
        }
    }

    gotoxy(Left_Edge + Map_Width + 1, 1);
    SetColor(16);
    cout << "得點: " << score;
    gotoxy(Left_Edge + Map_Width + 1, 3);
    cout << "Player: 4";
    gotoxy(Left_Edge + Map_Width + 1, 5);
    cout << "Bomb: 3";
    gotoxy(Left_Edge + Map_Width + 1, 8);
    SetColor(28);
    cout << "Power: ";
    SetColor(16);
    cout << snake.length() << " ";
    gotoxy(Left_Edge + Map_Width + 1, 10);
    SetColor(26);
    cout << "Graze: ";
    SetColor(16);
    cout << snake.length()*10 << " ";
    gotoxy(Left_Edge + Map_Width + 1, 12);
    SetColor(27);
    cout << "點: ";
    SetColor(16);
    cout << snake.length()*100 << " ";

    //右下角東方貪食蛇
    gotoxy(Left_Edge + Map_Width + 1, 20);
    cout << "      |              \\          ";
    gotoxy(Left_Edge + Map_Width + 1, 21);
    cout << "- - - - - - -     ---------      ";
    gotoxy(Left_Edge + Map_Width + 1, 22);
    cout << "   ___|___            |          ";
    gotoxy(Left_Edge + Map_Width + 1, 23);
    cout << "   |  |  |            | _____    ";
    gotoxy(Left_Edge + Map_Width + 1, 24);
    cout << "   -------            |     |";
    gotoxy(Left_Edge + Map_Width + 1, 25);
    cout << "   |  |  |           /      |    ";
    gotoxy(Left_Edge + Map_Width + 1, 26);
    cout << "   -------          /       |    ";
    gotoxy(Left_Edge + Map_Width + 1, 27);
    cout << "     /| \\         /        /    ";
    gotoxy(Left_Edge + Map_Width + 1, 28);
    cout << "    / |  \\               \\/    ";
    gotoxy(Left_Edge + Map_Width + 20, 29);
    cout << "貪食蛇";

    SetColor();
}

void update(){
    gotoxy(Left_Edge + Map_Width + 1, 1);
    SetColor(16);
    cout << "得點: " << score;
    gotoxy(Left_Edge + Map_Width + 1, 3);
    cout << "Player: " << snake.HP();
    gotoxy(Left_Edge + Map_Width + 1, 5);
    cout << "Bomb: 3";
    gotoxy(Left_Edge + Map_Width + 1, 8);
    SetColor(28);
    cout << "Power: ";
    SetColor(16);
    cout << snake.length() << " ";
    gotoxy(Left_Edge + Map_Width + 1, 10);
    SetColor(26);
    cout << "Graze: ";
    SetColor(16);
    cout << snake.length()*10 << " ";
    gotoxy(Left_Edge + Map_Width + 1, 12);
    SetColor(27);
    cout << "點: ";
    SetColor(16);
    cout << snake.length()*100 << " ";
    
    SetColor();
}