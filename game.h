#ifndef GAME_H
#define GAME_H
#include <ncursesw/ncurses.h>


#define NAME_LENGTH 4
#define NUM_ENEMIES 30

typedef struct {
    int rank;
    char name[NAME_LENGTH + 1];
    int score;
} Rank;

void play();
void title(int x, int y);
void gameover(int x, int y);
void gameclear(int x, int y);
void help(int x);
int screenCheck(int x, int y);
void updateRanking(Rank ranking[]);
void init_File(Rank ranking[]);
void main_init();
void drawBorder(WINDOW* win);

#endif
