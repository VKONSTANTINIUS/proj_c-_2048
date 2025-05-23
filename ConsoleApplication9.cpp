#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

enum direction { DOWN = 80, UP = 72, LEFT = 75, RIGHT = 77, ENTER = 13 };

const int MENU_SIZE = 3;
const char* menuItems[MENU_SIZE] = {
    "New Game",
    "Save / Load",
    "Exit"
};

const int BOX_SIZE = 4;
const int WIN_COUNT = 2048;
int board[BOX_SIZE][BOX_SIZE]{};
int score = 0;

void setCursorPos(int x, int y);
void setColor(int color);
void drawFrame(int x, int y, int width, int height, int color);
void drawMenu(int selected);
void menu_init();
 
void set_board();
void add_number();
void draw_board();
void move_up();
void move_down();
void move_left();
void move_right();

bool is_full();
bool is_won();
bool no_move();

void new_game();

int main()
{     
    
    menu_init();
   

}

void setCursorPos(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawFrame(int x, int y, int width, int height, int color) {
    setColor(color);
    for (int i = 0; i < width; i++) {
        setCursorPos(x + i, y);
        cout << char(196);
        setCursorPos(x + i, y + height - 1);
        cout << char(196);
    }
    for (int i = 0; i < height; i++) {
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

void drawMenu(int selected) {
    system("cls");

    int startX = 30;
    int startY = 10;
    int width = 20;
    int height = 3;

    for (int i = 0; i < MENU_SIZE; i++) {
        int x = startX;
        int y = startY + i * height;

        if (i == selected) {
            drawFrame(x - 2, y - 1, width, height, 10); // зелена рамка
            setColor(10);
        }
        else {
            setColor(7);
        }

        setCursorPos(x, y);
        cout << menuItems[i];
    }

    setColor(7);
}

void menu_init() {
    int selected = 0;
    bool running = true;

    while (running) {
        drawMenu(selected);

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == UP) {
                selected = (selected - 1 + MENU_SIZE) % MENU_SIZE;
            }
            else if (key == DOWN) {
                selected = (selected + 1) % MENU_SIZE;
            }
        }
        else if (key == ENTER) {
            system("cls");
            setColor(14);
            cout << "Selected option: " << menuItems[selected] << endl;
            setColor(7);
            if (selected == 2) { // Exit
                running = false;
            }
            else if (selected == 0) {
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

    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            cout << setw(6) << board[i][j];
        }
        cout << "\n\n\n";
    }
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

void new_game() {
    srand(time(0));
    set_board();
    draw_board();

    while (true) {

        if (is_won()) {
            cout << "You won!\n";
            break;
        }

        if (is_full() || no_move()) {
            cout << "Game over!\n";
            break;
        }

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

    }


}


