#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <term.h>

#define ENEMY_SPEED 4 /*敵の移動速度*/
#define GAME_CLEAR 60 /*ゲームクリア条件(敵の合計数)*/
#define NAME_LENGTH 4 /*名前の長さ*/
#define NUM_ENEMIES 30 /*一度出現する敵の最大数*/

/*ランキングの構造体*/
typedef struct{
    int rank;
    char name[NAME_LENGTH+1];
    int score;
}Rank;

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
    
    mvprintw(y/2+3,x/2-4,"1.Start");
    mvprintw(y/2+4,x/2-4,"2.Rank");
    mvprintw(y/2+5,x/2-4,"3.Help");
    mvprintw(y/2+6,x/2-4,"0.End");
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
    mvprintw(13,x/2-15,"弾が発射され敵を倒すことができる ");

    mvprintw(15,x/2-10," 1 Enemy = 250 Point");

    mvprintw(18,x/2-3,"0:Back");
}

/*画面の大きさチェック*/
int screenCheck(int x,int y){
    if (x < 85 || y < 20) {
        return 1;
    }
    return 0;
}

void updateRanking(Rank ranking[]){
    int i;
    FILE* file;
    
    file = fopen("ranking.txt","w");
    if(file == NULL){
        fprintf(stderr,"ファイルを開くことができませんでした。\n");
        return;
    }
    
    for(i = 0;i < 10; i++){
        fprintf(file, "%d\t%s\t%d\n", ranking[i].rank, ranking[i].name, ranking[i].score);
    }
    fclose(file);
}

void init_File(Rank ranking[]){
    int i;
    FILE* file;
    
    file = fopen("ranking.txt", "r");
    if (file == NULL) {
        fprintf(stderr,"ファイルを開くことができませんでした。\n");
        return;
    }else{
        for (i = 0; i < 10; i++) {
            if (fscanf(file, "%d %s %d", &(ranking[i].rank), ranking[i].name, &(ranking[i].score)) != 3) {
                ranking[i].rank = i + 1;
                strncpy(ranking[i].name, "", 10);
                ranking[i].score = 0;
                return;
            }
        }
    }
    fclose(file);
}

void main_init(){
    srand(time(NULL));
    setlocale(LC_ALL, "");
    
    initscr();
    start_color();
    assume_default_colors(COLOR_CYAN,COLOR_BLACK); /*デフォルトの色設定(文字"緑",背景"黒")*/
    init_pair(1,COLOR_RED, COLOR_BLACK); /*敵の色とゲームオーバー設定(文字"赤",背景"黒")*/
    init_pair(2,COLOR_YELLOW, COLOR_BLACK); /*ダメージを食らった時とゲームクリア*/
    
    keypad(stdscr,TRUE);
    crmode();
    curs_set(0);
}

void drawBorder(WINDOW* win){
    wborder(win, '|', '|', '-','-', '+', '+', '+', '+');
    refresh();
    wrefresh(win);
}

int main(int argc, char *argv[]){
    int x,y,ch,i,l;
    int mvY; /*プレイヤー位置情報*/
    char name[NAME_LENGTH+1]; /*プレイヤーの名前配列*/
    char Player = '>'; /*自機*/
    int health=3; /*体力*/
    bool shot = false; /*発射*/
    bool damege = false; /*ダメージ*/
    bool end = false; /*終了フラグ*/
    bool readSuccess = true;
    int numE=0;/*敵の出現数*/
    int score=0; /*スコア*/
    int rankLine = 6; /*ランキングの初期位置*/
    int newRank; /*ランキング更新用*/
    WINDOW *win; /*windowの設定*/
    FILE *file; /*ファイル*/
    FILE* backupFile; /*バックアップ*/
    Rank ranking[10]; /*ランキングの構造体配列*/
    int enemyX[NUM_ENEMIES];
    int enemyY[NUM_ENEMIES];
    
    
    if (argc > 1) {
        fprintf(stderr,"./shootingのみ入力してください\n");
        return 1;
    }
    
    main_init();
    getmaxyx(stdscr,y,x); /*画面サイズの取得*/
    
    if(screenCheck(x,y)){
        mvprintw(y/2,x/2-20,"画面の大きさを85×20以上にしてください");
        refresh();
        sleep(2);
        endwin();
        exit(1);
    }
    
    win = newwin(y,x,0,0);
    
    init_File(ranking);/*rankingの初期化*/
        
    while(true){
        
        drawBorder(win);
        
        /*メニュー画面*/
        title(x,y);
        
        /*メニュー番号選択*/
        ch = getch();
        noecho();
        
        if(ch != ERR){
            switch(ch){
            case '1':
                clear();
                
                drawBorder(win);
                
                /*名前入力*/
                mvprintw(y/2+7, x/2-40, "※F1~F12までのキーやbackspaceなどを押さないで下さい ");
                mvprintw(y/2+6, x/2-40, "※空白文字が入力された場合_と出力されます ");
                mvprintw(y/2+5, x/2-40, "※入力しない場合nmlsとなります ");
                mvprintw(y/2-2, x/2-40, "名前をアルファベット四文字以内で入力: ");
                
                timeout(2147483647);/*名前が入力される限界まで待機*/
                echo();
                
                i=0;
                while ((ch = getch()) != '\n' && i < NAME_LENGTH) {
                    if(ch == ' '){
                        ch= '_';
                    }
                    name[i] = ch;
                    i++;
                    if (i == NAME_LENGTH) {
                        name[i] = '\0';
                        mvprintw(y/2 , x/2-10, "---Press Any Key---");
                        noecho();
                    }
                }
                
                if(name[0]=='\0'){
                    name[0]='n';
                    name[1]='m';
                    name[2]='l';
                    name[3]='s';
                    name[4]='\0'; 
                }
                noecho();
                
                clear();
                
                drawBorder(win);
                
                mvY = y/2; /*プレイヤーの初期位置*/
                
                enemyY[0] = rand() % (y - 4) + 3; /*敵の出現位置*/
                enemyX[0] = x - 3;
                numE++;
                
                for(i=1; i<NUM_ENEMIES;i++){
                    enemyY[i] = rand() % (y - 4) + 3; /*敵の出現位置*/
                    enemyX[i] = 9*i +x;
                    numE++;
                }
        
                mvprintw(1,1,"Health: %d |Score: %5d |Name: %4s |",health,score,name); /*体力の表示*/
                for (l = 1; l < x-1; l++) {
                    mvaddch(2, l, '-');
                }
                
                attron(A_BOLD);
                mvaddch(mvY,2,Player);
                attroff(A_BOLD);
                
                noecho();
                
                while(health>0&&end!=true){
                    ch = ERR;
                    timeout(180);
                    ch =getch();
                    
                    drawBorder(win);
                    
                    for(i=0;i<NUM_ENEMIES;i++){
                        if(mvY == enemyY[i] && 2 ==enemyX[i]){ /*プレイヤーと敵が同じ位置*/
                            health--;
                            
                            damege = true;
                            
                            enemyY[i] = rand() % (y - 4) + 3;
                            enemyX[i] = x-3;
                            numE++;
                        }else if(enemyX[i]<=2){
                            mvprintw(enemyY[i],enemyX[i], " ");
                            enemyY[i] = rand() % (y - 4) + 3;
                            enemyX[i] = x-3;
                            numE++;
                        }
                        
                
                        if(mvY == enemyY[i] && shot){ /*弾を発射したy座標と敵のy座標が一緒*/
                            mvprintw(enemyY[i],enemyX[i], " ");
                            enemyX[i] = x - 3;
                            enemyY[i] = rand() % (y - 4) + 3;
                            shot = false;
                            numE++;
                            score += 250;
                            
                        }
                        
                        mvprintw(enemyY[i], enemyX[i], " ");
                    
            
                        enemyX[i] -= ENEMY_SPEED;
                        if(enemyX[i] <0){
                            enemyX[i] = x-3;
                        }
                    
                    
                        attron(COLOR_PAIR(1));
                        attron(A_BOLD);
                        mvprintw(enemyY[i], enemyX[i], "@"); /*敵は"＠"で表現*/
                        attroff(COLOR_PAIR(1));
                        attroff(A_BOLD);
                    }
                    
                    mvprintw(1,1,"Health: %d |Score: %5d |Name: %4s |",health,score,name); /*体力の表示*/
                    for (l = 1; l < x-1; l++) { /*体力・スコアの画面間に一線*/
                        mvaddch(2, l, '-');
                    }
                    
                    mvaddch(mvY, 3, ' ');
                    mvaddch(mvY, 4, ' ');
                    
                    if(ch != ERR){
                        switch(ch){
                        case KEY_UP: /*上に移動*/
                                if(mvY>3){
                                    attron(A_BOLD);
                                    mvaddch(mvY,2,' ');
                                    mvY--;
                                    attroff(A_BOLD);
                                }
                                shot = false;
                                break;
                        case KEY_DOWN: /*下へ移動*/
                                if(mvY < y-2){
                                    attron(A_BOLD);
                                    mvaddch(mvY,2,' ');
                                    mvY++;
                                    attroff(A_BOLD);
                                }
                                shot = false;
                                break;
                        case ' ': /*弾の発射*/
                                shot = true;
                                attron(A_BOLD);
                                mvaddch(mvY, 3, '*');
                                mvaddch(mvY, 4, '-');
                                attroff(A_BOLD);
                                break;
                        case 'q':  /* 'q' キーが押された場合、ゲームを終了する*/
                                health = 0;  /* health を 0 にして強制終了*/
                                break;
                        }
                    }
                    
                    if (damege) {
                        attron(COLOR_PAIR(2)); /* 色ペア2を有効化 */
                        attron(A_BOLD);
                        mvaddch(mvY, 2, Player);
                        attroff(COLOR_PAIR(2)); /* 色ペア2を無効化 */
                        attroff(A_BOLD);
                        damege = false;
                    }else{
                        attron(A_BOLD);
                        mvaddch(mvY, 2, Player);
                        attroff(A_BOLD);
                    }
                    
                    if(numE >= GAME_CLEAR){
                        end = true;
                    }
                }
                if(health==0){
                    clear();
                    
                    drawBorder(win);
                    
                    gameover(x,y);
                    
                    score  = 0;
                    health = 3;
                    numE   = 0;
                    shot = false;
                    end = false;
                    
                }else if(end==true){
                    clear();
                    
                    drawBorder(win);
                    
                    gameclear(x,y);
                    mvprintw(y/2+8, x/2-10, "Score: %5d", score);
                    
                    newRank = -1;
                    for (i = 0; i < 10; i++) {
                        if (score > ranking[i].score) {
                            newRank = i;
                            break;
                        }
                    }

                    if (newRank != -1) {
                        for (i = 9; i > newRank; i--) {
                            ranking[i] = ranking[i - 1];
                            ranking[i].rank = i + 1;
                        }
                        ranking[newRank].rank = newRank + 1;
                        strncpy(ranking[newRank].name, name, NAME_LENGTH);
                        ranking[newRank].score = score;
                        updateRanking(ranking);
                        
                        mvprintw(y/2+9, x/2-10, "%d位にランクインしました ", ranking[newRank].rank);
                    }
                    
                    score  = 0;
                    health = 3;
                    numE   = 0;
                    shot = false;
                    end = false;
                }
                
                refresh();
                sleep(2);
                
                clear();
                break;
            case '2':
                clear();
                
                drawBorder(win);
                
                mvprintw(2, x/2-3, "RANKING");
                mvprintw(4, x/2-10, "Rank     Name   Score");
                
                /*ranking.txtの読み込み*/
                file = fopen("ranking.txt", "r");
                if (file == NULL) {
                    fprintf(stderr,"ファイルを開くことができませんでした。\n");
                    return 1;
                }else{
                    for (i = 0; i < 10; i++) {
                        if (fscanf(file, "%d %s %d", &(ranking[i].rank), ranking[i].name, &(ranking[i].score)) != 3) {
                            ranking[i].rank = i + 1;
                            strncpy(ranking[i].name, "", 10);
                            ranking[i].score = 0;
                            mvprintw(18, x/2-10, "データ読み込みが失敗しています");
                            readSuccess = false;
                            break;
                        }
                    }
                    fclose(file);
                    
                    if (readSuccess) {
                        /*ranking.txtをrankBackup.txtに上書きする*/
                        file = fopen("ranking.txt", "r");
                        backupFile = fopen("rankBackup.txt", "w");
                        if (file != NULL && backupFile != NULL) {
                            char buffer[100];
                            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                                fprintf(backupFile, "%s", buffer);
                            }
                            fclose(file);
                            fclose(backupFile);
                        } else {
                            fprintf(stderr, "バックアップファイルに書き込むことができませんでした。\n");
                            return 1;
                        }
                    }
                }
                
                for (i = 0; i < 10; i++) {
                    mvprintw(rankLine++, x/2-10, "%2d %10s %7d", ranking[i].rank, ranking[i].name, ranking[i].score);
                }
                
                mvprintw(rankLine+2,x/2-3,"0:Back");
                
                while(ch!= '0'){
                    ch=getch();
                }
                refresh();
                clear();
                rankLine = 6;
                break;
                    
            case '3':
                clear();
                drawBorder(win);
                
                help(x);
                
                while(ch!= '0'){
                    ch=getch();
                }
                refresh();
                clear();
                break;
            case '0':
                endwin();
                return 0;
            default:
                /*0,1,2以外のキー入力の場合*/
                break;
            }
        }
    }
    return 0;
}