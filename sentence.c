#include "sentence.h"
#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <term.h>
#include <time.h>

/*タイトル表示*/
void title(x,y){
    attron(A_BOLD);
    mvprintw(y/2-7, x/2-40, "   ********  **                                               ********** **  ** ");
    mvprintw(y/2-6, x/2-40, "  **//////  /**                                              /////**/// /** /** ");
    mvprintw(y/2-5, x/2-40, " /**        /**        ******    ******    ******    ******      /**    /** /** ");
    mvprintw(y/2-4, x/2-40, " /********* /******   **////**  **////**  **////**  **////**     /**    /** /** ");
    mvprintw(y/2-3, x/2-40, " ////////** /**///** /**   /** /**   /** /**   /** /**   /**     /**    /** /** ");
    mvprintw(y/2-2, x/2-40, "        /** /**  /** /**   /** /**   /** /**   /** /**   /**     /**    //  //  ");
    mvprintw(y/2-1, x/2-40, "  ********  /**  /** //******  //******  //******  //******      /**     **  ** ");
    mvprintw(y/2  , x/2-40, " ////////   //   //   //////    //////    //////    //////       //     //  //  ");
    attroff(A_BOLD);
    
    mvprintw(y/2+3,x/2-36,"     ｑ、ｑ                    　1.Start 　　　　　　　 ＿＿ｐ、ｐ      ");
    mvprintw(y/2+4,x/2-36,"    ┼ｰ` （[ ＠ｌﾆｌ二二二二]  　2.Rank  　[二二二二ｌﾆｌ＠ ]）`ｰ┼     ");
    mvprintw(y/2+5,x/2-36," ノ　＜┬ｒー`　　　　　　     　3.Help  　　　　　　　　`ーｒ┬＜　ヽ  ");
    mvprintw(y/2+6,x/2-36," L,_｣ ￣￣　　　　　　　　     　0.End   　　　　　　　　　　￣￣ Ｌ_,｣ ");
}

/*ゲームオーバー表示*/
void gameover(x,y){
    attron(COLOR_PAIR(1));
    attron(A_BOLD);
    mvprintw(y/2-4, x/2-36, "   ****     **     **   **  *******   *****   **   **  *******  ****** ");
    mvprintw(y/2-3, x/2-36, "  **  **   ****    *** ***   **   *  **   **  **   **   **   *   **  **");
    mvprintw(y/2-2, x/2-36, " **       **  **   *******   ** *    **   **   ** **    ** *     **  **");
    mvprintw(y/2-1, x/2-36, " **       **  **   *******   ****    **   **   ** **    ****     ***** ");
    mvprintw(y/2,   x/2-36, " **  ***  ******   ** * **   ** *    **   **    ***     ** *     ** ** ");
    mvprintw(y/2+1, x/2-36, "  **  **  **  **   **   **   **   *  **   **    ***     **   *   **  **");
    mvprintw(y/2+2, x/2-36, "   *****  **  **   **   **  *******   *****      *     *******  **** **");
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);
}

/*ゲームクリア表示*/
void gameclear(x,y){
    attron(COLOR_PAIR(2));
    attron(A_BOLD);
    mvprintw(y/2-9, x/2-20, "   ****     **     **   **  *******");
    mvprintw(y/2-8, x/2-20, "  **  **   ****    *** ***   **   *");
    mvprintw(y/2-7, x/2-20, " **       **  **   *******   ** *  ");
    mvprintw(y/2-6, x/2-20, " **       **  **   *******   ****  ");
    mvprintw(y/2-5, x/2-20, " **  ***  ******   ** * **   ** *  ");
    mvprintw(y/2-4, x/2-20, "  **  **  **  **   **   **   **   *");
    mvprintw(y/2-3, x/2-20, "   *****  **  **   **   **  *******");
    /*単語が長いため2列に分割*/               
    mvprintw(y/2-1, x/2-30, "    ****   ****     *******    **     ******     **  **  **");
    mvprintw(y/2  , x/2-30, "   **  **   **       **   *   ****     **  **    **  **  **");
    mvprintw(y/2+1, x/2-30, "  **        **       ** *    **  **    **  **    **  **  **");
    mvprintw(y/2+2, x/2-30, "  **        **       ****    **  **    *****     **  **  **");
    mvprintw(y/2+3, x/2-30, "  **        **   *   ** *    ******    ** **     **  **  **");
    mvprintw(y/2+4, x/2-30, "   **  **   **  **   **   *  **  **    **  **              ");
    mvprintw(y/2+5, x/2-30, "    ****   *******  *******  **  **   **** **    **  **  **");
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);
}

void help(x){
    mvprintw(3,x/2-11,"    ↑ : 上移動 ");
    mvprintw(4,x/2-11,"    ↓ : 下移動 ");
    mvprintw(5,x/2-11," SPACE : 発射！！！ ");
    mvprintw(6,x/2-11,"     Q : 強制終了 ");

    mvprintw(8,x/2-15,"敵と接触すると -1HP ");
    mvprintw(9,x/2-11,"     0 HP = GameOver ");

    mvprintw(12,x/2-15,"敵と同じ高さでスペースを押すと ");
    mvprintw(13,x/2-15,"弾が発射され敵を倒すことができます ");

    mvprintw(15,x/2-10," 1 Enemy = 250 Point");

    mvprintw(18,x/2-3,"0:Back");
}

