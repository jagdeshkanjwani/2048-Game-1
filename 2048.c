#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

struct Game {
    int size;
    int target;
    int difficulty;
    int score;
    int highestScore;
    int moves;
    int timeLimit;
};

void display(int (*game)[8], struct Game *g, int remaining);
void moveup(int (*game)[8], int size, int *score);
void movedown(int (*game)[8], int size, int *score);
void moveright(int (*game)[8], int size, int *score);
void moveleft(int (*game)[8], int size, int *score);
void spawn(int (*game)[8], int size);
int checkgameover(int (*game)[8], int size, int target);

void copyBoard(int (*game)[8], int oldGame[8][8], int size);
int boardChanged(int (*game)[8], int oldGame[8][8], int size);

void gotoxy(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


// MAIN FUNCTION

int main() {

    int game[8][8] = {0};
    int oldGame[8][8];

    int ch;
    int choice;
    int end;

    int elapsed;
    int remaining;
    int lastRemaining;
    int timeUp;

    time_t startTime;

    struct Game g;

    g.highestScore = 0;

    srand(time(NULL));

    while(1) {

        // BOARD SIZE

        do {
            printf("\nBoard Size:\n");
            printf("1. 4x4\n");
            printf("2. 5x5\n");
            printf("3. 6x6\n");
            printf("4. 7x7\n");
            printf("5. 8x8\n");

            printf("Enter board size: ");
            scanf("%d",&g.size);

        } while(g.size < 1 || g.size > 5);

        if(g.size == 1)
            g.size = 4;

        else if(g.size == 2)
            g.size = 5;

        else if(g.size == 3)
            g.size = 6;

        else if(g.size == 4)
            g.size = 7;

        else
            g.size = 8;


        // TARGET

        do {
            printf("\nCustom Target Choose:\n");
            printf("1. Custom Target\n");
            printf("2. Default Target (2048)\n");

            printf("Enter your choice: ");
            scanf("%d",&choice);

        } while(choice != 1 && choice != 2);

        if(choice == 1) {

            do {
                printf("Enter your target (4, 8, 16, 32...): ");
                scanf("%d",&g.target);

            } while(g.target < 4 ||
                    (g.target & (g.target-1)) != 0);
        }
        else {
            g.target = 2048;
        }


        // DIFFICULTY

        do {
            printf("\nSelect Difficulty:\n");
            printf("1. Easy (No Time Limit)\n");
            printf("2. Medium (15 Minutes)\n");
            printf("3. Hard (10 Minutes)\n");

            printf("Enter difficulty: ");
            scanf("%d",&g.difficulty);

        } while(g.difficulty < 1 || g.difficulty > 3);

        if(g.difficulty == 1)
            g.timeLimit = 0;

        else if(g.difficulty == 2)
            g.timeLimit = 900;

        else
            g.timeLimit = 600;


        // RESTART LOOP

        while(1) {

            end = 0;
            timeUp = 0;

            g.score = 0;
            g.moves = 0;

            // RESET BOARD

            for(int i=0; i<g.size; i++) {
                for(int j=0; j<g.size; j++) {
                    game[i][j] = 0;
                }
            }

            system("cls");            
            spawn(game,g.size);

            startTime = time(NULL);
            lastRemaining = -999;


            // MAIN GAME LOOP

            while(end == 0) {

                elapsed = (int)difftime(time(NULL),startTime);

                // CHECK TIME LIMIT

                if(g.timeLimit > 0 && elapsed >= g.timeLimit) {
                    timeUp = 1;
                    end = 2;
                    break;
                }

                if(g.timeLimit == 0)
                    remaining = -1;

                else
                    remaining = g.timeLimit - elapsed;


                // UPDATE DISPLAY EVERY SECOND

                if(remaining != lastRemaining) {

                    display(game,&g,remaining);

                    lastRemaining = remaining;
                }


                // CHECK KEYBOARD

                if(_kbhit()) {

                    ch = getch();

                    if(ch == 0 || ch == 224) {

                        ch = getch();

                        // SAVE OLD BOARD

                        copyBoard(game,oldGame,g.size);


                        // MOVEMENT

                        if(ch == 72)
                            moveup(game,g.size,&g.score);

                        else if(ch == 80)
                            movedown(game,g.size,&g.score);

                        else if(ch == 75)
                            moveleft(game,g.size,&g.score);

                        else if(ch == 77)
                            moveright(game,g.size,&g.score);


                        // CHECK BOARD CHANGES

                        if(boardChanged(game,oldGame,g.size)) {

                            g.moves++;

                            if(g.score > g.highestScore)
                                g.highestScore = g.score;

                            spawn(game,g.size);

                            elapsed = (int)difftime(time(NULL),startTime);

                            if(g.timeLimit == 0)
                                remaining = -1;

                            else
                                remaining = g.timeLimit - elapsed;

                            display(game,&g,remaining);

                            lastRemaining = remaining;
                        }
                    }
                }


                // CHECK WIN OR GAME OVER

                end = checkgameover(game,g.size,g.target);

                Sleep(50);
            }


            // FINAL RESULT

            if(timeUp == 1) {
                printf("\nTIME UP! You could not achieve the target.");
            }

            else if(end == 1) {
                printf("\nYOU WON!");
            }

            else if(end == 2) {
                printf("\nGAME OVER!");
            }


            // GAME MENU

            printf("\n\n1. New Game\n");
            printf("2. Restart Game\n");
            printf("3. Exit\n");

            do {
                printf("Enter your choice: ");
                scanf("%d",&choice);

            } while(choice < 1 || choice > 3);


            if(choice == 1) {
                break;
            }

            else if(choice == 2) {
                continue;
            }

            else {
                return 0;
            }
        }
    }

    return 0;
}


// DISPLAY FUNCTION

void display(int (*game)[8], struct Game *g, int remaining) {


    gotoxy(0,0);

    printf("Highest: %-10d       Score: %-10d       Moves: %d\n",
           g->highestScore,g->score,g->moves);

    if(remaining == -1) {

        printf("Difficulty: EASY       Time: No Limit\n");
    }

    else {

        printf("Difficulty: %s       Time Remaining: %02d:%02d\n",
               g->difficulty == 2 ? "MEDIUM" : "HARD",
               remaining/60,remaining%60);
    }

    printf("\n========== 2048 GAME ==========\n\n");


    // TOP BORDER

    for(int j=0; j<g->size; j++) {
        printf("+-------");
    }

    printf("+\n");


    // BOARD

    for(int i=0; i<g->size; i++) {

        for(int j=0; j<g->size; j++) {

            if(game[i][j] == 0)
                printf("|       ");

            else
                printf("| %5d ",game[i][j]);
        }

        printf("|\n");


        // ROW BORDER

        for(int j=0; j<g->size; j++) {
            printf("+-------");
        }

        printf("+\n");
    }
}


// SPAWN FUNCTION

void spawn(int (*game)[8], int size) {

    int length;
    int width;
    int n = 1;

    for(int i=0; i<size; i++) {

        for(int j=0; j<size; j++) {

            if(game[i][j] == 0) {
                n = 0;
            }
        }
    }

    if(n == 1) {
        return;
    }

    while(1) {

        length = rand()%size;
        width = rand()%size;

        if(game[length][width] == 0) {

            int num = (rand()%2)*2+2;

            game[length][width] = num;

            break;
        }
    }
}


// MOVE UP

void moveup(int (*game)[8], int size, int *score) {

    int n = 1;
    int m = 0;
    int f;

    for(int i=1; i<=size-1; i++) {

        for(int j=0; j<size; j++) {

            m = 0;
            n = 1;
            f = 0;

            while(n<=i) {

                if(game[i-n][j] == 0) {

                    game[i-n][j] = game[i-m][j];
                    game[i-m][j] = 0;
                }

                else if(game[i-n][j] != 0) {

                    if(f == 0) {

                        if(game[i-n][j] == game[i-m][j]) {

                            game[i-n][j] =
                            game[i-n][j] + game[i-m][j];

                            *score += game[i-n][j];

                            game[i-m][j] = 0;

                            f = 1;
                        }
                    }
                }

                if(i-n == 0) {
                    break;
                }

                n++;
                m++;
            }

            n = 1;
        }
    }
}


// MOVE DOWN

void movedown(int (*game)[8], int size, int *score) {

    int n = 1;
    int m = 0;
    int f;

    for(int i=size-2; i>=0; i--) {

        for(int j=0; j<size; j++) {

            n = 1;
            m = 0;
            f = 0;

            while(i+n<=size-1) {

                if(game[i+n][j] == 0) {

                    game[i+n][j] = game[i+m][j];
                    game[i+m][j] = 0;
                }

                else if(game[i+n][j] != 0) {

                    if(f == 0) {

                        if(game[i+n][j] == game[i+m][j]) {

                            game[i+n][j] =
                            game[i+n][j] + game[i+m][j];

                            *score += game[i+n][j];

                            game[i+m][j] = 0;

                            f = 1;
                        }
                    }
                }

                if(i+n == size-1) {
                    break;
                }

                n++;
                m++;
            }
        }
    }
}


// MOVE RIGHT

void moveright(int (*game)[8], int size, int *score) {

    int n = 1;
    int m = 0;
    int f;

    for(int i=0; i<size; i++) {

        f = 0;

        for(int j=size-2; j>=0; j--) {

            n = 1;
            m = 0;

            while(j+n<=size-1) {

                if(game[i][j+n] == 0) {

                    game[i][j+n] = game[i][j+m];
                    game[i][j+m] = 0;
                }

                else if(game[i][j+n] != 0) {

                    if(f == 0) {

                        if(game[i][j+n] == game[i][j+m]) {

                            game[i][j+n] =
                            game[i][j+n] + game[i][j+m];

                            *score += game[i][j+n];

                            game[i][j+m] = 0;

                            f = 1;
                        }
                    }
                }

                if(j+n == size-1) {
                    break;
                }

                n++;
                m++;
            }
        }
    }
}


// MOVE LEFT

void moveleft(int (*game)[8], int size, int *score) {

    int n = 1;
    int m = 0;
    int f;

    for(int i=0; i<size; i++) {

        f = 0;

        for(int j=1; j<=size-1; j++) {

            n = 1;
            m = 0;

            while(n<=j) {

                if(game[i][j-n] == 0) {

                    game[i][j-n] = game[i][j-m];
                    game[i][j-m] = 0;
                }

                else if(game[i][j-n] != 0) {

                    if(f == 0) {

                        if(game[i][j-n] == game[i][j-m]) {

                            game[i][j-n] =
                            game[i][j-n] + game[i][j-m];

                            *score += game[i][j-n];

                            game[i][j-m] = 0;

                            f = 1;
                        }
                    }
                }

                if(j-n == 0) {
                    break;
                }

                n++;
                m++;
            }
        }
    }
}


// COPY BOARD

void copyBoard(int (*game)[8], int oldGame[8][8], int size) {

    for(int i=0; i<size; i++) {

        for(int j=0; j<size; j++) {

            oldGame[i][j] = game[i][j];
        }
    }
}


// CHECK BOARD CHANGED

int boardChanged(int (*game)[8], int oldGame[8][8], int size) {

    for(int i=0; i<size; i++) {

        for(int j=0; j<size; j++) {

            if(game[i][j] != oldGame[i][j]) {
                return 1;
            }
        }
    }

    return 0;
}


// CHECK GAME OVER

int checkgameover(int (*game)[8], int size, int target) {

    // CHECK TARGET

    for(int i=0; i<size; i++) {

        for(int j=0; j<size; j++) {

            if(game[i][j] == target) {
                return 1;
            }
        }
    }


    // CHECK EMPTY CELLS AND MERGING

    for(int i=0; i<size; i++) {

        for(int j=0; j<size; j++) {

            if(game[i][j] == 0) {
                return 0;
            }

            // RIGHT SIDE

            if(j<size-1) {

                if(game[i][j] == game[i][j+1]) {
                    return 0;
                }
            }

            // DOWN SIDE

            if(i<size-1) {

                if(game[i][j] == game[i+1][j]) {
                    return 0;
                }
            }
        }
    }

    return 2;
}