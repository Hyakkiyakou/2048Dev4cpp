#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <curses.h>
using namespace std;

#define N 4 // number of lattices
#define WIDTH 5 // length of each lattice

#define TARGET 2048  //game target

// game status
#define S_FAIL 0
#define S_WIN 1
#define S_NORMAL 2
#define S_QUIT 3

class Game2048 {
public:
    Game2048() : status(S_NORMAL) {
        setTestData();
    }
    int getStatus() { return status; }

    void processInput() {
        char ch = getch();
        if (ch >= 'a' && ch <= 'z') {
            ch -= 32;
        }
        if (status == S_NORMAL) {
            bool updated = false;
            if (ch == 'A') {
                updated = moveLeft();
            } else if (ch == 'S') { //down should rotate counterclockwise three times, then same as "left"
                rotate();
                rotate();
                rotate();
                updated = moveLeft();
                rotate();
            } else if (ch == 'D') {
                rotate();
                rotate();
                updated = moveLeft();
                rotate();
                rotate();
            } else if (ch == 'W') {
                rotate();
                updated = moveLeft();
                rotate();
                rotate();
                rotate();
            }
            if (updated) {
                randNew();
                if(isOver()){
                	status = S_FAIL;
				} 
            }
        }
        if (ch == 'Q') {  //if user press "Q", quit the game
            status = S_QUIT;
        } else if (ch == 'R') {  //if user press "R", restart the game
            restart();
        }
    }

    void draw() { //draw the game screen
        clear();
        const int offset = 12;
        for (int i = 0; i <= N; ++i) {
            for (int j = 0; j <= N; ++j) {
                drawItem(i * 2, 1 + j * WIDTH + offset, '+');

                if (i != N) {
                    drawItem(i * 2 + 1, 1 + j * WIDTH + offset, '|');
                }
                for (int k = 1; j != N && k < WIDTH; ++k) {
                    drawItem(i * 2, 1 + j * WIDTH + k + offset, '-');
                }
                if (i != N && j != N) {
                    drawNum(i * 2 + 1, (j + 1) * WIDTH + offset, data[i][j]);
                }
            }
        }
 
        mvprintw(2 * N + 2, (5 * (N - 4) - 1) / 2, "W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");

        if (status == S_WIN) {
            mvprintw( N, 5 * N / 2 - 1, " YOU WIN,PRESS R TO CONTINUE ");
        } else if (status == S_FAIL) {
            mvprintw( N, 5 * N / 2 - 1, " YOU LOSE,PRESS R TO CONTINUE ");
        }
    }

    void setTestData() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i][j] = 16 << (i + j);
                data[i][j] = 2 << (1 + rand() % 7);

            }
        }
    }
private:
	//judge whether the game is over
	bool isOver(){
		for(int i = 0;i < N;i++){
			for(int j = 0;j < N;j++){
				//if there is a empty lattice or same adjacent numbers
				if ((j + 1 < N) && (data[i][j] * data[i][j+1] == 0 || data[i][j] == data[i][j+1])) return false;
                if ((i + 1 < N) && (data[i][j] * data[i+1][j] == 0 || data[i][j] == data[i+1][j])) return false;
			}
		}
		return true;
	}
	
    //move left, return whether the array has changed
    bool moveLeft() {
        int tmp[N][N];
        for (int i = 0; i < N; ++i) {
            int currentWritePos = 0;  //current position to write
            int lastValue = 0;  //last haddled number
            for (int j = 0; j < N; ++j) {
                tmp[i][j] = data[i][j];//tmp save the original array 
                if (data[i][j] == 0) {
                    continue;
                }
                if (lastValue == 0) {
                    lastValue = data[i][j];
                } else { //should check whether same
                    if (lastValue == data[i][j]) {
                        data[i][currentWritePos] = lastValue * 2;//merge
                        lastValue = 0;
                        if(data[i][currentWritePos] == TARGET){ //everytime there a merge, judge target2048 
                        	status = S_WIN;
						}
                    } else { //the last haddled number doesn't need to merge
                        data[i][currentWritePos] = lastValue;
                        lastValue = data[i][j];
                    }
                    ++currentWritePos;
                }
                data[i][j] = 0;//current number has haddled
            }
            if (lastValue != 0) {
                data[i][currentWritePos] = lastValue;
            }
        }
        //tmp save the original array, data is rotated array, if changed shows that rotate success
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (data[i][j] != tmp[i][j]) return true;
            }
        }
        return false; //
    }
    // rotates 90 degrees counterclockwise
    void rotate() {
        int tmp[N][N] = {0};
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                tmp[i][j] = data[j][N - 1 - i];
            }
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i][j] = tmp[i][j];
            }
        }
    }

    //restart the game
    void restart() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i][j] = 0;
            }
        }
        randNew();
        randNew();
        status = S_NORMAL;
    }
    
	//random generate a number
    bool randNew() {
        vector<int> emptyPos; //to generate a number, should check whether there is a empty lattice
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (data[i][j] == 0) {
                    emptyPos.push_back(i * N + j);
                }
            }
        }
        if (emptyPos.size() == 0) {
            return false;
        }
        int value = emptyPos[rand() % emptyPos.size()];
        data[value / N][value % N] = rand() % 10 == 1 ? 4 : 2; //10% probability to generate 4
        return true;
    }

    //draw a item at(row, col)
    void drawItem(int row, int col, char c) {
        move(row, col);
        addch(c);
    }

    //draw a number at(row, col)
    void drawNum(int row, int col, int num) {
        while (num > 0) {
            drawItem(row, col, num % 10 + '0');
            num /= 10;
            --col;
        }
    }
private:
    int data[N][N];
    int status;
};
void initialize() {
    initscr();  //initialize screen
    cbreak();   //continue enter
    noecho();   //no echo enter
    curs_set(0);
    srand(time(NULL));
}
void shutdown() {
    endwin();
}
int main() {
    initialize();
    Game2048 game;
    do {
        game.draw();
        game.processInput();
    } while (S_QUIT != game.getStatus());  //if quit the game, shutdown
    shutdown();
    return 0;
}
