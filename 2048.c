
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

struct Game {
    int size;
    int target;
    int difficulty;
    int score;
    int highestScore;
    int moves;
    int moveLimit;
};

void display(int (*game)[8], struct Game *g);
void moveup(int (*game)[8], int size, int *score);
void movedown(int (*game)[8], int size, int *score);
void moveright(int (*game)[8], int size, int *score);
void moveleft(int (*game)[8], int size, int *score);
void spawn(int (*game)[8], int size);
int checkgameover(int (*game)[8], int size, int target);
void copyBoard(int (*game)[8], int oldGame[8][8], int size);
int boardChanged(int (*game)[8], int oldGame[8][8], int size);
void difficulty(struct Game *g);
void updateHighest(struct Game *g);

// File Handling Functions
void loadHighest(struct Game *g);
void saveHighest(struct Game *g);
void saveHistory(struct Game *g);

void gotoxy(int x, int y) {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = FALSE;

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Main Function
int main() {
    int game[8][8] = {0};
    int oldGame[8][8];
    int ch;
    int choice;
    int end;

    struct Game g;

    g.highestScore = 0;

    // Load Previous Highest Score
    loadHighest(&g);

    srand(time(NULL));

    while(1) {

        // Board Size
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

        if(g.size == 1){
            g.size = 4;
        }
        else if(g.size == 2){
            g.size = 5;
        }
        else if(g.size == 3){
            g.size = 6;
        }
        else if(g.size == 4){
            g.size = 7;
        }
        else{
            g.size = 8;
        }

        // Target
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

        // Difficulty
        difficulty(&g);

        // Restart Loop
        while(1) {
            end = 0;
            g.score = 0;
            g.moves = 0;

            // Reset Board
            for(int i=0; i<g.size; i++) {
                for(int j=0; j<g.size; j++) {
                    game[i][j] = 0;
                }
            }

            system("cls");
            spawn(game,g.size);

            // Main Loop Game
            while(end == 0) {
                display(game,&g);

                // Check Keyboard
                if(_kbhit()) {
                    ch = getch();

                    if(ch == 'q' || ch == 'Q') {
                        end = 3;
                        break;
                    }

                    if(ch == 0 || ch == 224) {
                        ch = getch();

                        // Save Old Game
                        copyBoard(game,oldGame,g.size);

                        // Movement
                        if(ch == 72){
                            moveup(game,g.size,&g.score);
                        }
                        else if(ch == 80){
                            movedown(game,g.size,&g.score);
                        }
                        else if(ch == 75){
                            moveleft(game,g.size,&g.score);
                        }
                        else if(ch == 77){
                            moveright(game,g.size,&g.score);
                        }

                        // Check Board Changed
                        if(boardChanged(game,oldGame,g.size)) {
                            updateHighest(&g);
                            spawn(game,g.size);

                            // Check Move Limit
                            if(g.moveLimit != 0 &&
                               g.moves >= g.moveLimit) {
                                end = 2;
                                break;
                            }
                        }
                    }
                }

                display(game,&g);

                // Check Win or Game Over
                if(end == 0) {
                    end = checkgameover(game,g.size,g.target);
                }

                Sleep(50);
            }

            // Final Result
            if(end == 1) {
                printf("\nYOU WON!");
            }
            else if(end == 2) {
                printf("\nGAME OVER! Move limit reached or no moves left.");
            }
            else if(end == 3) {
                printf("\nGAME PAUSED!");
            }

            // Save Game History and Highest Score
            saveHistory(&g);
            saveHighest(&g);

            // Game Menu
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

// Difficulty Function
void difficulty(struct Game *g) {
    do {
        printf("\nSelect Difficulty:\n");
        printf("1. Easy (Unlimited Moves)\n");
        printf("2. Medium (1000 Moves)\n");
        printf("3. Hard (900 Moves)\n");
        printf("Enter difficulty: ");
        scanf("%d",&g->difficulty);
    } while(g->difficulty < 1 || g->difficulty > 3);

    if(g->difficulty == 1){
        g->moveLimit = 0;
    }
    else if(g->difficulty == 2){
        g->moveLimit = 1000;
    }
    else{
        g->moveLimit = 900;
    }
}

// Load Highest Score
void loadHighest(struct Game *g) {
    FILE *fp;

    fp = fopen("highest.txt", "r");

    if(fp != NULL) {
        fscanf(fp, "%d", &g->highestScore);
        fclose(fp);
    }
}

// Save Highest Score
void saveHighest(struct Game *g) {
    FILE *fp;

    fp = fopen("highest.txt", "w");

    if(fp != NULL) {
        fprintf(fp, "%d", g->highestScore);
        fclose(fp);
    }
}

// Save Game History
void saveHistory(struct Game *g) {
    FILE *fp;

    fp = fopen("history.txt", "a");

    if(fp != NULL) {
        fprintf(fp, "Score: %d | Moves: %d\n",
                g->score, g->moves);

        fclose(fp);
    }
}

// Update Highest Function
void updateHighest(struct Game *g) {
    g->moves++;

    if(g->score > g->highestScore) {
        g->highestScore = g->score;
        saveHighest(g);
    }
}

// Display Function
void display(int (*game)[8], struct Game *g) {
    gotoxy(0,0);

    printf("Highest: %-10d       Score: %-10d       Moves: %d\n",
           g->highestScore,g->score,g->moves);

    if(g->moveLimit == 0) {
        printf("Difficulty: EASY       Moves: Unlimited          \n");
    }
    else {
        printf("Difficulty: %s       Moves Remaining: %-10d\n",
               g->difficulty == 2 ? "MEDIUM" : "HARD",
               g->moveLimit - g->moves);
    }

    printf("\n========== 2048 GAME ==========\n\n");

    // Top Border
    for(int j=0; j<g->size; j++) {
        printf("+-------");
    }
    printf("+\n");

    // Board
    for(int i=0; i<g->size; i++) {
        for(int j=0; j<g->size; j++) {
            if(game[i][j] == 0){
                printf("|       ");
            }
            else{
                printf("| %5d ",game[i][j]);
            }
        }

        printf("|\n");

        // Row Border
        for(int j=0; j<g->size; j++) {
            printf("+-------");
        }
        printf("+\n");
    }
}

// Spawn Function
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

// Move Up
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

// Move Down
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

// Move Right
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

// Move Left
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

// Copy Board
void copyBoard(int (*game)[8], int oldGame[8][8], int size) {
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            oldGame[i][j] = game[i][j];
        }
    }
}

// Check Board Changed
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

// Check Game Over
int checkgameover(int (*game)[8], int size, int target) {

    // Check Target
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            if(game[i][j] == target) {
                return 1;
            }
        }
    }

    // Check Empty Cell and Merging
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            if(game[i][j] == 0) {
                return 0;
            }

            // Right Side
            if(j<size-1) {
                if(game[i][j] == game[i][j+1]) {
                    return 0;
                }
            }

            // Down Side
            if(i<size-1) {
                if(game[i][j] == game[i+1][j]) {
                    return 0;
                }
            }
        }
    }

    return 2;
}
