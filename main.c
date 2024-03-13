#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <term.h>
#include "game.h"

int main(int argc, char *argv[]){
    if (argc > 1) {
        fprintf(stderr,"./shootingのみ入力してください\n");
        return 1;
    }
    
    play();
    
    return 0;
}