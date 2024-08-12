#include <stdio.h>
#include <stdbool.h> // for boolean types.
#include <stdlib.h>  // for system function.
#include <unistd.h>  // for sleep  function.

//standart grid size. 
const int grid_rows = 6;
const int grid_cols = 7;


typedef enum player {RED = 0, BlUE} Player; 
Player Player_Mark = RED; //starts with RED.

//Direction array.
typedef enum direction {Down = 0, Up, Right, Left, Down_Right, Down_Left, Up_Right, Up_Left} Direction;
int dir_r[] =          {       1, -1,     0,    0,          1,         1,       -1,      -1};
int dir_c[] =          {       0,  0,     1,   -1,          1,        -1,        1,      -1};
Direction dir = Down; //starts with Down.


void  show          ( int Connect_four[grid_rows][grid_cols] );
bool  checkDraw     ( int Connect_four[grid_rows][grid_cols] );
int   checkWinner   ( int Connect_four[grid_rows][grid_cols], int current_row, int current_col, Player current_player );
void  Game          (                             void       );






int main() {
    
    Game();        

    return 0;
}








void show ( int Connect_four[grid_rows][grid_cols] ) {
    

    for (int j = 0; j < grid_cols; j++)
        printf("  Col%d   ", j + 1);


    printf("\n");
    for (int i = 0; i < grid_rows; i++) {
        for (int j = 0; j < grid_cols; j++) {
            
            
            if      (Connect_four[i][j] == 0) { printf( "\033[38;5;1m(  RED ) " ); } 
            else if (Connect_four[i][j] == 1) { printf( "\033[38;5;4m( BLUE ) " ); } 
           
            else { printf( "\033[38;5;15m(      ) " ); }
            
        }
        printf("\n\033[38;5;15m---------------------------------------------------------------\n");
    }

}


bool checkDraw ( int Connect_four[grid_rows][grid_cols] ) {
   
    for (int j = 0; j < grid_cols; j++) {
    
        if (Connect_four[0][j] == -1) return false; //Because the Connect four works like a stack if one Column has an Empty Upper Cell
                                                    //that means There's an Empty cell Under that Cell.

    }


    return true;
}


int checkWinner  ( int Connect_four[grid_rows][grid_cols], int current_row, int current_col, Player current_player ) {

    for (int i = 0; i < 8; i++) {
        
        int r = current_row, c = current_col;
        
        //ensuring that the next 3 checkers in the Direction are not outside the 2D matrix boundaries.
        bool boundry_safe = ( (r + (dir_r[dir] * 3) > -1 && r + (dir_r[dir] * 3) < grid_rows) && (c + (dir_c[dir] * 3) > -1 && c + (dir_c[dir] * 3) < grid_cols) );
        bool four_checkers_found = true; 

    
        if ( boundry_safe ) {
            
            for (int j = 0; j < 3; j++) {
        
                r += dir_r[dir];
                c += dir_c[dir];
                
                if ( Connect_four[r][c] != (int)current_player ) { four_checkers_found = false; break; } 
            }
        
        } else { four_checkers_found = false; }
        

        if ( four_checkers_found ) { return (int)current_player; }

    
        dir = (dir + 1) % 8;
    } 


    return -1; //nobody wins!!
}



void Game (void) {
        
    int Connect_four[grid_rows][grid_cols];
    
    for (int i = 0; i < grid_rows; i++)
        for (int j = 0; j < grid_cols; j++)
                Connect_four[i][j] = -1; //fill all the empty cells with -1.
    

    while (1) {
    
        system ("clear");
        show   ( Connect_four );
    

        int player_column;
        printf(Player_Mark == 0 ? "player Red Enter Column : " : "Player Blue Enter Column : ");
        scanf("%d", &player_column); //Notice that `player_column` is 1 based must make it always 0 based. 
           while ( !(player_column <= grid_cols && player_column >= 1) ) { printf("invalid Option, Enter Exits Column : "); scanf("%d", &player_column); }
        
    
        while ( Connect_four[0][player_column - 1] != -1 ) { //check if the column is full or not. 
                                                             //its full check if draw or not.
    
            bool isDraw = checkDraw ( Connect_four );
            if (isDraw) {
                system ("clear");
                printf ("\033[38;5;3mD R A W\n");
                sleep  (3);
                return;
            }

            printf("Column is Full, Choose Another Column : ");
            scanf("%d", &player_column);

        } 


        int cell_row = -1;
            while ( Connect_four[cell_row + 1][player_column - 1] == -1 && cell_row < grid_rows ) { cell_row += 1; } //first look then increase.
        Connect_four[cell_row][player_column - 1] = Player_Mark;
    
        int status = checkWinner (Connect_four, cell_row, player_column - 1, Player_Mark);

        switch ( status ) {
    
            case -1: //in case nobody wins change the player.
            
                Player_Mark = ( Player_Mark + 1 ) % 2; //change the player if (0) --> 1 and otherwise.
                break;
            
            case 0:
                
                system  ("clear");
                show    ( Connect_four );
                printf  ("Player \033[38;5;1mRED WON!!\n");
                sleep   (3);
                return;
            
            case 1:
                
                system  ("clear");
                show    ( Connect_four );
                printf  ("Player \033[38;5;4mBLUE WON!!\n");
                sleep   (3);
                return;
            
            default:
                break;
        }
    }
}
