#ifdef _WIN32
    #define CLEAR "cls"
#elif __unix__
    #define CLEAR "clear"
#else
    #define CLEAR none
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_ROW 10
#define BOARD_COL 10

char shipPos[2][BOARD_ROW][BOARD_COL];
char shootPos[2][BOARD_ROW][BOARD_COL];

/*Maybe it's unreasonable but I don't want to do same 
thing over an over for each ship names while placing.*/

//Ship Shapes

struct Ship {                           
    char shape[BOARD_ROW][BOARD_COL];
    char name[15];
    char length;
    char width;
};

char playerName[2][50];

struct Ship ships[] = {
    {
        .shape = {{'*', 'P', 'P', 'P'},
                   {'P', 'P', 'P', '*'}},
        .name = "Plane Ship",
        .length = 2,
        .width = 4
    },
    {
        .shape = {{'D', 'D', 'D', 'D'}},
        .name = "Destroyer Ship",
        .length = 1,
        .width = 4
    },
    {
        .shape = {{'*', 'S', '*'},
                   {'S', 'S', 'S'}},
        .name = "Submarine Ship",
        .length = 2,
        .width = 3
    },
    {
        .shape = {{'B', 'B'}},
        .name = "Boat Ship",
        .length = 1,
        .width = 2
    }
};

const char missSays[][42] = {
    "MISS, Maybe next time!",
    "MISS, Blind people aim better!",
    "MISS, To the mountains and the stones!!!",
    "MISS, This is not fish hunting game!",
    "MISS, What bad luck!",
    "MISS, Try wiping your glasses!",
    "MISS, This was bad"
};

void boardDraw(bool player, char board[2][BOARD_ROW][BOARD_COL]) 
{
    printf("\n");
    printf("   ");
    for (char col = 'A'; col < 'A' + BOARD_COL; col++) {
        printf("%c ", col);
    }
    printf("\n");

    for (int row = 0; row < BOARD_ROW; row++) {
        printf("%2d ", row + 1);
        for (int col = 0; col < BOARD_COL; col++) {
            printf("%c ", board[player][row][col]);
        }
        printf("\n");
    }
}

void boardEraser() 
{
    char i;
    for (i = 0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {
            shipPos[0][i][j] = '*';
        }
    }

    for (i = 0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {
            shipPos[1][i][j] = '*';
        }
    }

    for (i = 0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {
            shootPos[0][i][j] = '*';
        }
    }

    for (i = 0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {
            shootPos[1][i][j] = '*';
        }
    }

}

bool placeShip(struct Ship ship, int row, int col, char orientation, bool plr) 
{
    if (orientation == 'H') {
         // Overflow check
        if (row <= 0 || row + ship.length > BOARD_ROW || col <= 0 || col + ship.width > BOARD_COL) {
            printf("\nShip can't be placed because of overflow.");
            return 1;  
        }

        // Overlap cheCk
        int i;
        for (i = 0; i < ship.length; i++) {
            for (int j = 0; j < ship.width; j++) {
                if (shipPos[plr][row + i][col + j] != '*' && ship.shape[i][j] != '*') {
                    printf("\nShip can't be placed because of overlapping.");
                    return 1; 
                }
            }
        }

        for (i = 0; i < ship.length; i++) {
            for (int j = 0; j < ship.width; j++) {
                if (ship.shape[i][j] != '*') {
                    shipPos[plr][row + i][col + j] = ship.shape[i][j];
                }
            }
        }
    } else if (orientation == 'V') {
        int i;

        // Overflow check
        if (row <= 0 || row + ship.width > BOARD_ROW || col <= 0 || col + ship.length > BOARD_COL) {
            printf("\nShip can't be placed because of overflow.");
            return 1;  
        }

        //Overlap check
        for (i = 0; i < ship.length; i++) {
            for (int j = 0; j < ship.width; j++) {
                if (shipPos[plr][row + j][col + i] != '*' && ship.shape[i][j] != '*') {
                    printf("\nShip can't be placed because of overlapping.");
                    return 1; 
                }
            }
        }


        for (i = 0; i < ship.length; i++) {
            for (int j = 0; j < ship.width; j++) {
                if (ship.shape[i][j] != '*') { 
                    shipPos[plr][row + j][col + i] = ship.shape[i][j];
                }
            }
        }
    } else {
        printf("Sth wrong I can feel it.\n");
        return 1;
    }

    return 0; 
}

bool openingScreen()
{
    system(CLEAR);
    printf("Welcome to\n"
        "    ____ ___  _  _ _ ____ ____ _       ____ _  _ _  _ _  _\n"
        "    |__| |  \\ |\\/| | |__/ |__| |       [__  |  | |\\ | |_/ \n"
        "    |  | |__/ |  | | |  \\ |  | |___    ___] |__| | \\| | \\_\n"
        "\n"
        " by Efe Arda Yavaş\n"
        " Press enter to continue or 'e' to exit the game.\n");


    if(getchar() == 'e')
        return 1;

    return 0;
}


void playerRegister() 
{
    system(CLEAR);
    printf("Name of first player: ");
    scanf(" %49[^\n]", playerName[0]);
    printf("Name of second player: ");
    scanf(" %49[^\n]", playerName[1]);
}

void boardSetup()
{   
    char colTemp, direction;
    int row, col;
    char i;
    for(i=0; i<8; i++) {
        system(CLEAR);
        printf("\n%s is placing...", playerName[i%2]);
        boardDraw(i%2, shipPos);
        while(1){
            // printf("\nThis ship looks like this: ");  EKLEME YAPILACAK

            printf("\nThis ship looks like this: %s\n"
                   "                           %s\n", ships[(int)i/2].shape[0], ships[(int)i/2].shape[1]);  
    
            printf("\nSelect a position for %s. Example B5: ", ships[(int)i/2].name);
            scanf(" %c%d", &colTemp, &row);
            row -= 1;
            col = (int)colTemp - 'A';
            
            printf("\nSelect a direction to place, Horizontal or Vertical (H/V): ");
            scanf(" %c", &direction);
            
            if(placeShip(ships[(int)i/2], row, col, direction, i%2) != 0) {
                printf(" Please try again\n");
                continue;
            }
            break;
        }
        system(CLEAR);
        printf("\nWaiting for next player... %s should press 'enter' to continue\n", playerName[(i+1)%2]);
        getchar();
        getchar();
    }
    printf("\nPlacement completed for each players.");
}

void game() {
    char colTemp, direction;
    int row, col;
    char playerPoint[] = {0,0};
    char i;
    for(i=0; 1; i++) {
        srand(time(NULL));
        system(CLEAR);
        printf("\n%s's Points: %d\n%s's Points: %d\n", playerName[0], playerPoint[0], playerName[1], playerPoint[1]);
        printf("\n%s is SHOOTING...", playerName[i%2]);
        boardDraw(i%2, shootPos);
        while(1){
            printf("\nType a coordinate to hit: ");
            scanf("%c%d", &colTemp, &row);
            row -= 1;
            col = (int)colTemp - 'A';
            
            if(row < 0 || row >= BOARD_ROW || col < 0 || col >= BOARD_COL) {
                printf("\nYou typed invalid cooridinate.");
            }
        
            if(shipPos[(i-1)%2][row][col] == '*') {
                printf("\n%s", missSays[rand()%7]);
                shootPos[i%2][row][col] = 'O';
                break;
            }else{
                printf("\n%s HITTED a ship!!!", playerName[i%2]);
                shootPos[i%2][row][col] = 'X';
                playerPoint[i%2] += 1;
                break;
            }
        }

        if(playerPoint[i%2] >= 16) {
            printf("\n%s WIN this game.", playerName[i%2]);
            printf("\nGood job captain!");
            system(CLEAR);
            return;
        }

        system(CLEAR);
        printf("\nWaiting for next player... %s should press 'enter' to continue\n", playerName[(i+1)%2]);
        getchar();
        getchar();
    }
}


int main() 
{
    while(1){
        if(openingScreen() == 1)
            break;
        playerRegister();
        boardEraser();
        boardSetup();
        game();
    }
		
    return 0;
}