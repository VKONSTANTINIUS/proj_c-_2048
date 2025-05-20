#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

enum direction { DOWN = 80, UP = 72, LEFT = 75, RIGHT = 77, ENTER = 13 };

const int BOX_SIZE = 4;
const int WIN_COUNT = 2048;
int board[BOX_SIZE][BOX_SIZE]{};
int score = 0;
 
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

int main()
{     
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

    return 0;

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


