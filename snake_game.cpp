#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <unistd.h>
#include "ADXL345.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

int width;
int height;
std::vector<char> board;
const int START_X = 5;
const int START_Y = 3;
const int SENS = 100;
const char BODY_SYMBOL = '*';
const char FOOD_SYMBOL = 'O';
const char BORDER_SYMBOL = '#';
const int WAIT_TIME = 400; //milliseconds

volatile sig_atomic_t stop;

enum direction {up, down, left, right};

const direction START_DIRECTION = right;

const char HEADS[4] = {'^' , 'v' , '<' , '>'};

void catchSIGINT(int signum){
    stop = 1;
}

struct SnakePart {
    int x;
    int y;

    SnakePart(int a, int b);
};

class Snake {
    private:

        direction dir;
        std::vector<SnakePart> parts;

    public:

        Snake();

        int getLength() const;
        const SnakePart& getPart(int i) const;
	    direction getDir() const;
        void grow(SnakePart newPart);

        //returns a copy of the tail as it was before the snake moved.
        //in the event that the snake needs to grow, the new part will be
        //the snake's previous tail.
        const SnakePart move();

        void changeDirection(direction newdir);
};

SnakePart::SnakePart(int a, int b){
    x = a;
    y = b;
}

Snake::Snake(){
    dir = START_DIRECTION;
    parts.push_back(SnakePart(START_X,START_Y));
    parts.push_back(SnakePart(START_X-1,START_Y));
    parts.push_back(SnakePart(START_X-2,START_Y));
}

int Snake::getLength() const{
    return parts.size();
}

const SnakePart& Snake::getPart(int i) const{
    return parts.at(i);
}

direction Snake::getDir() const{
    return dir; 
}

void Snake::grow(SnakePart newPart){
    parts.push_back(newPart);
}

const SnakePart Snake::move(){

    SnakePart prevTail(parts.back().x,parts.back().y);

    for(int i = parts.size() - 1; i > 0; i--){
        parts.at(i).x = parts.at(i-1).x;
        parts.at(i).y = parts.at(i-1).y;
    }

    switch(dir){
        case up:
            parts.at(0).y--;
            break;
        case down:
            parts.at(0).y++;
            break;
        case left:
            parts.at(0).x--;
            break;
        case right:
            parts.at(0).x++;
            break;
        default:
            break;
    }

    board[prevTail.y * width + prevTail.x] = ' ';

    return prevTail;
}

void Snake::changeDirection(direction newdir){
    switch(newdir){
        case up:
            if(dir == down)
                break;
            dir = newdir;
            break;
        case down:
            if(dir == up)
                break;
            dir = newdir;
            break;
        case right:
            if(dir == left)
                break;
            dir = newdir;
            break;
        case left:
            if(dir == right)
                break;
            dir = newdir;
            break;
        default:
            break;
    }
}


//places the border of the board and fills the rest with whitespace
void initBoard(){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(x==0||y==0||x==width-1||y==height-1)
                board[y*width + x] = BORDER_SYMBOL;
            else
                board[y*width + x] = ' ';
        }
    }
}

void drawBoard(){
    system("clear");
    for(int y = 0; y < height; y++){
	printf("%.*s\n",width,&board[y * width]);
    }
}

void drawSnake(const Snake& s){
    for(int i = s.getLength()-1; i > 0; i--){
        SnakePart part = s.getPart(i);
        board[part.y * width + part.x] = BODY_SYMBOL;
    }
    SnakePart head = s.getPart(0);
    board[head.y * width + head.x] = HEADS[s.getDir()];
}

//places new food in a random empty space
void spawnFood(int prevX, int prevY){
    int x,y;

    do{
        x = rand() % (width - 1) + 1;
        y = rand() % (height - 1) + 1;
    }while(board[y*width + x] != ' ' || x == prevX && y == prevY);

    board[y*width + x] = FOOD_SYMBOL;
}

//determines if the snake head is currently on the food
bool isEatingFood(const Snake& s){
    SnakePart head = s.getPart(0);
    return board[head.y * width + head.x] == FOOD_SYMBOL;
}

bool isGameOver(const Snake& s){
    SnakePart head = s.getPart(0);
    return board[head.y * width + head.x] == BORDER_SYMBOL || board[head.y * width + head.x] == BODY_SYMBOL;
}

int main(int argc, char* argv[]){
    if(argc < 3){
        width = 60;
        height = 20;
    } 
    else{
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }

    // catch SIGINT from ctrl+c, instead of having it abruptly close this program
    signal(SIGINT, catchSIGINT);

    uint8_t devid;
    int16_t mg_per_lsb = 4;
    int16_t XYZ[3];

    // For the bubble level, we will do a 10x10 grid output to console
    int x,y,z,i;

    // Map physical addresses
    Map_Physical_Addrs();

    // Configure Pin Muxing
    Pinmux_Config();

    // Initialize I2C0 Controller
    I2C0_Init();

    // 0xE5 is read from DEVID(0x00) if I2C is functioning correctly
    ADXL345_REG_READ(0x00, &devid);

    srand(time(0));
    Snake snake;
    board.resize(width*height);
    initBoard();
    drawSnake(snake);
    spawnFood(START_X+1,START_Y);

    if(devid == 0xE5){

        printf("Device ID Verified\n");

        // Initialize accelerometer chip
        ADXL345_Init();

        printf("ADXL345 Initialized\n");

        while(!stop){
            if(ADXL345_IsDataReady()){
                ADXL345_XYZ_Read(XYZ);

                x = XYZ[0];
                y = XYZ[1];
                z = XYZ[2];

                if(abs(x) >= abs(y)){
                    if(x < -1 * SENS)
                        snake.changeDirection(left);
                    else if(x > SENS)
                        snake.changeDirection(right);
                }
                else {
                    if(y < -1 * SENS)
                        snake.changeDirection(down);
                    else if(y > SENS)
                        snake.changeDirection(up);
                }
            }
            const SnakePart prevTail = snake.move();
            if(isGameOver(snake)){
                drawSnake(snake);
		        drawBoard();
		        break;
            }
            if(isEatingFood(snake)){
                snake.grow(prevTail);
                spawnFood(snake.getPart(0).x,snake.getPart(0).y);
            }
            drawSnake(snake);
            drawBoard();
            usleep(WAIT_TIME * 1000); 
        }

        printf("GAME OVER\n");

    } else {
        printf("Incorrect device ID\n");
    }

    Close_Device();

    return 0;
}
