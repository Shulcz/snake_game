#include <iostream>
#include <ncurses.h>
#include <unistd.h>
using namespace std;

bool gameover;
const int W = 30;
const int H = 15;
int x, y, point_x, point_y;
enum sdirections
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
sdirections dir;
int tail_x[200], tail_y[200];
int lentail;

void setup()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    gameover = false;
    dir = STOP;
    x = W / 2;
    y = H / 2;
    point_x = rand() % W;
    point_y = rand() % H;
}

void cleanup()
{
    endwin();
}

void draw()
{
    clear();

    for (int i = 0; i < W + 2; ++i)
        printw("#");
    printw("\n");

    for (int i = 0; i < H; ++i)
    {
        for (int j = 0; j < W; ++j)
        {
            if (j == 0)
                printw("#");
            if (i == y && j == x)
                printw("@");
            else if (i == point_y && j == point_x)
                printw("*");
            else
            {
                bool flag = false;
                for (int k = 0; k < lentail; ++k)
                {
                    if (tail_x[k] == j && tail_y[k] == i)
                    {
                        printw("o");
                        flag = true;
                    }
                }
                if (!flag)
                    printw(" ");
            }
            if (j == W - 1)
                printw("#");
        }
        printw("\n");
    }

    for (int i = 0; i < W + 2; ++i)
        printw("#");
    printw("\n");

    refresh();
}

void input()
{
    int key = getch();

    if (key != ERR)
    {
        switch (key)
        {
        case 'a':
            if (dir == RIGHT)
                break;
            dir = LEFT;
            break;
        case 'd':
            if (dir == LEFT)
                break;
            dir = RIGHT;
            break;
        case 'w':
            if (dir == DOWN)
                break;
            dir = UP;
            break;
        case 's':
            if (dir == UP)
                break;
            dir = DOWN;
            break;
        case 27:
            gameover = true;
            break;
        case 32:
            dir = STOP;
            break;
        }
    }
}

void logic()
{
    if (dir != STOP)
    {
        for (int i = lentail; i > 0; --i)
        {
            tail_x[i] = tail_x[i - 1];
            tail_y[i] = tail_y[i - 1];
        }
        tail_x[0] = x;
        tail_y[0] = y;
        switch (dir)
        {
        case LEFT:
            --x;
            break;
        case RIGHT:
            ++x;
            break;
        case UP:
            --y;
            break;
        case DOWN:
            ++y;
            break;
        default:
            break;
        }
        if (x >= W)
            x = 0;
        else if (x < 0)
            x = W - 1;
        if (y >= H)
            y = 0;
        else if (y < 0)
            y = H - 1;
        for (int i = 0; i < lentail; ++i)
        {
            if (tail_x[i] == x && tail_y[i] == y)
                gameover = true;
        }
        if (x == point_x && y == point_y)
        {
            point_x = rand() % W;
            point_y = rand() % H;
            ++lentail;
        }
    }
}

int main()
{
    setup();
    while (!gameover)
    {
        draw();
        input();
        logic();
        usleep(100000);
    }
    cleanup();
    cout << "GAME OVER" << endl;
    return 0;
}
