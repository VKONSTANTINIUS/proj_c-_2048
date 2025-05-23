#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;

HANDLE h;

enum direction { DOWN = 80, UP = 72, LEFT = 75, RIGHT = 77, ENTER = 13, ESC = 27 };

const int MENU_SIZE = 3;
const char* menuItems[MENU_SIZE] = { "New Game", "Continue", "Exit" };

const int BOX_SIZE = 4;
const int WIN_COUNT = 2048;
int board[BOX_SIZE][BOX_SIZE]{};
int score = 0;
  
void ConsoleSett(); // Функція налаштувань консолі
void setCursorPos(int x, int y); // Функція налаштування позиції курсора
void setColor(int color); //Функція вибору кольору тексту

//Функції для роботи з меню
void drawFrame(int x, int y, int width, int height, int color); //рамка навколо обраного пункту меню
void drawMenu(int selected); //вивід ігрового меню
void menu_init(); // Ініціалізація ігрового меню

//Функції гри
void set_board(); //Очищення поля (заповнення масиву нулями)
void add_number(); //Додавання випадкового числа (2 або 4) в комірку зі значенням 0 
void draw_board(); //Вивід ігрового поля 
void move_up(); //Рух вгору
void move_down(); //Рух вниз
void move_left(); //Рух вліво
void move_right(); //Рух вправо

//Логічні функції для визначення стану гри
bool is_full(); //функція для визначення чи є вільні комірки
bool is_won(); //функція для перевірки чи досягнуто максимально можливе значення
bool no_move(); //функція для перевірки чи можливі подальші дії

void new_game(); //функція процесу гри

void autosave(const char* filename); //функція збереження у файл при виході з гри
void loadsave(const char* filename); //функція завантаження даних з файлу для продовження гри

int main()
{     
    ConsoleSett();
    menu_init();  

}

void ConsoleSett() {                        //Функція налаштувань консолі
    system("mode con cols=50 lines=20");
    system("title GAME2048");
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = false;
    cci.dwSize = 100;
    SetConsoleCursorInfo(h, &cci);
    system("cls");
}

void setCursorPos(int x, int y) {       // Функція налаштування позиції курсора
    COORD pos = { (int)x, (int)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int value) {          //Функція вибору кольору тексту
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (value) {
        case 2: SetConsoleTextAttribute(h, 2); break;      // light gray
        case 4: SetConsoleTextAttribute(h, 11); break;     // light cyan
        case 8: SetConsoleTextAttribute(h, 14); break;     // yellow
        case 16: SetConsoleTextAttribute(h, 10); break;    // green
        case 32: SetConsoleTextAttribute(h, 13); break;    // magenta
        case 64: SetConsoleTextAttribute(h, 12); break;    // red
        case 128: SetConsoleTextAttribute(h, 9); break;    // blue
        case 256: SetConsoleTextAttribute(h, 6); break;    // brown/yellow
        case 512: SetConsoleTextAttribute(h, 3); break;    // aqua
        case 1024: SetConsoleTextAttribute(h, 5); break;   // purple
        case 2048: SetConsoleTextAttribute(h, 15); break;  // white
        default: SetConsoleTextAttribute(h, 8); break;     // dark gray
    }
}


void drawFrame(int x, int y, int width, int height, int color) {    //рамка навколо обраного пункту меню
    setColor(8);
    for (int i = 0; i < width; i++) { //горизонтальний кордон
        setCursorPos(x + i, y);
        cout << char(196);
        setCursorPos(x + i, y + height - 1);
        cout << char(196);
    }
    for (int i = 0; i < height; i++) {  //вертикальний кордон
        setCursorPos(x, y + i);
        cout << char(179);
        setCursorPos(x + width - 1, y + i);
        cout << char(179);
    }
    // Кути
    setCursorPos(x, y);                     cout << char(218);
    setCursorPos(x + width - 1, y);         cout << char(191);
    setCursorPos(x, y + height - 1);        cout << char(192);
    setCursorPos(x + width - 1, y + height - 1); cout << char(217);
    setColor(7);
}

void drawMenu(int selected) { //вивід ігрового меню
    system("cls");

    int startX = 10;
    int startY = 10;
    int width = 20;
    int height = 3;

    for (int i = 0; i < MENU_SIZE; i++) {
        int x = startX;
        int y = startY + i * height;

        if (i == selected) {
            drawFrame(x - 2, y - 1, width, height, 10); // зелена рамка
            setColor(8);
        }
        else {
            setColor(2048);
        }

        setCursorPos(x, y);
        cout << menuItems[i];
    }

    setColor(7);
}

void menu_init() { // Ініціалізація ігрового меню
    int selected = 0; 
    bool running = true;

    while (running) {
        drawMenu(selected);

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == UP) {    // Рух по меню вгору
                selected = (selected - 1 + MENU_SIZE) % MENU_SIZE;
            }
            else if (key == DOWN) {     // Рух по меню вниз
                selected = (selected + 1) % MENU_SIZE;
            }
        }
        else if (key == ENTER) {        //Вибір пункту меню
            system("cls");
            setColor(14);
            cout << "Selected option: " << menuItems[selected] << endl;
            setColor(7);
            if (selected == 2) { // Вибрано пункт Вихід
                system("cls");
                cout << "ESCAPE!\n";
                exit(0);
            }
            else if (selected == 0) { // Вибрано пункт Нова гра
                set_board();
                new_game();
            }
            else if (selected == 1) { // Вибрано пункт Продовжити гру
                set_board();
                loadsave("autosave.txt");
                new_game();
                
            }
            else {
                system("pause");
            }
        }
    }


}

void set_board() {      // Ініціалізація ігрового поля
    
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            board[i][j] = 0;
        }
    }
    
    add_number(); // Додаємо нове число (2 або 4) у випадкову порожню клітинку
    add_number();

}

void add_number() {     // Додаємо нове число
    int i = rand() % BOX_SIZE;
    int j = rand() % BOX_SIZE;
    int value = (rand() % 2 + 1) * 2; 
    if (board[i][j] == 0) {
        board[i][j] = value;
    }
    else add_number();  

}

void draw_board() {         // Вивід ігрового поля

    system("cls");

    cout << "Score: " << score << "\n";

    // Верхня межа
    cout << char(218);
    for (int i = 0; i < BOX_SIZE - 1; i++) {
        cout << string(6, char(196)) << char(194);
    }
    cout << string(6, char(196)) << char(191) << endl;

    for (int y = 0; y < BOX_SIZE; y++) {
        cout << char(179);
        for (int x = 0; x < BOX_SIZE; x++) {
            setColor(board[y][x]);
            cout << setw(6) << board[y][x];
            setColor(2048);
            cout << char(179);
        }
        cout << endl;

        if (y != BOX_SIZE - 1) {
            cout << char(195);
            for (int i = 0; i < BOX_SIZE - 1; i++) {
                cout << string(6, char(196)) << char(197);
            }
            cout << string(6, char(196)) << char(180) << endl;
        }
    }

    // Нижня межа
    cout << char(192);
    for (int i = 0; i < BOX_SIZE - 1; i++) {
        cout << string(6, char(196)) << char(193);
    }
    cout << string(6, char(196)) << char(217) << endl;
}

void move_up() {            // Рух вгору
    for (int j = 0; j < BOX_SIZE; j++) {
        for (int i = 1; i < BOX_SIZE; i++) {
            if (board[i][j] != 0) {
                int k = i;
                while (k > 0 && board[k - 1][j] == 0) {
                    board[k - 1][j] = board[k][j];
                    board[k][j] = 0;
                    k--;
                                   }
                if (k > 0 && board[k - 1][j] == board[k][j]) {
                    board[k - 1][j] *= 2;
                    score += board[k - 1][j];
                    board[k][j] = 0;                    
                }
            }
        }
    }
    add_number();
    draw_board();
}

void move_down() {          // Рух вниз
    for (int j = 0; j < BOX_SIZE; j++) {
        for (int i = BOX_SIZE - 2; i >= 0; i--) {
            if (board[i][j] != 0) {
                int k = i;
                while (k < BOX_SIZE - 1 && board[k + 1][j] == 0) {
                    board[k + 1][j] = board[k][j];
                    board[k][j] = 0;
                    k++;                    
                }
                if (k < BOX_SIZE - 1 && board[k + 1][j] == board[k][j]) {
                    board[k + 1][j] *= 2;
                    score += board[k + 1][j];
                    board[k][j] = 0;                    
                }
            }
        }
    }
    add_number();
    draw_board();
}

void move_left() {          // Рух вліво
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 1; j < BOX_SIZE; j++) {
            if (board[i][j] != 0) {
                int k = j;
                while (k > 0 && board[i][k - 1] == 0) {
                    board[i][k - 1] = board[i][k];
                    board[i][k] = 0;
                    k--;                    
                }
                if (k > 0 && board[i][k - 1] == board[i][k]) {
                    board[i][k - 1] *= 2;
                    score += board[i][k - 1];
                    board[i][k] = 0;                    
                }
            }
        }
    }
    add_number();
    draw_board();
}

void move_right() {         // Рух вправо
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = BOX_SIZE - 2; j >= 0; j--) {
            if (board[i][j] != 0) {
                int k = j;
                while (k < BOX_SIZE - 1 && board[i][k + 1] == 0) {
                    board[i][k + 1] = board[i][k];
                    board[i][k] = 0;
                    k++;                    
                }
                if (k < BOX_SIZE - 1 && board[i][k + 1] == board[i][k]) {
                    board[i][k + 1] *= 2;
                    score += board[i][k + 1];
                    board[i][k] = 0;                    
                }
            }
        }
    }
    add_number();
    draw_board();
}

bool is_full() {            // Перевірка чи є вільні клітинки
    
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == 0) {
                return false; 
            }
        }
    }
    return true; 
}

bool is_won() {         // Перевірка чи виконана умова виграшу
    
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == WIN_COUNT) {
                return true;  
            }
        }
    }
    return false;  
}

bool no_move() {            // Перевірка чи є можливі ходи
     
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            if (board[i][j] == 0) {
                return false;
            }
            if (j > 0 && board[i][j] == board[i][j - 1]) {
                return false;
            }
            if (j < BOX_SIZE - 1 && board[i][j] == board[i][j + 1]) {
                return false;
            }
            if (i > 0 && board[i][j] == board[i - 1][j]) {
                return false;
            }
            if (i < BOX_SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return false;  
            }
        }
    }
    return true;  
}

void new_game() {       //функція процесу гри
    srand(time(0));
   
    draw_board();

    while (true) {

        int direct = _getch();

        if (direct == 224)
            direct = _getch();

        if (direct == RIGHT) {
            move_right();
        }
        else if (direct == LEFT) {
            move_left();
        }
        else if (direct == UP) {
            move_up();
        }
        else if (direct == DOWN) {
            move_down();
        }
        else if (direct == ESC) {
            autosave("autosave.txt");
            menu_init();
        }

        if (is_won()) {
            system("cls");
            cout << "You won!\n";
            system("pause");
            menu_init();
        }

        if (is_full() || no_move()) {
            system("cls");
            cout << "Game over!\n";
            system("pause");
            menu_init();
        }
    }

}

void autosave(const char* filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "---" << endl;
        system("pause");
        return;
    }

    for (int y = 0; y < BOX_SIZE; y++) {
        for (int x = 0; x < BOX_SIZE; x++) {
            out << board[y][x] << ' ';
        }
        out << '\n';
    }

    out.close();
    
}

void loadsave(const char* filename) {
    ifstream in(filename);
    if (!in) {
        system("cls");
        cerr << "Can't open file" << endl;
        system("pause");
        menu_init();
        return;
    }

    for (int y = 0; y < BOX_SIZE; y++) {
        for (int x = 0; x < BOX_SIZE; x++) {
            in >> board[y][x];
        }
    }

    in.close();
    
}

