#include<stdio.h>

enum game_status {
    GAME_ONGOING = 0,
    PLAYER_X_WON = 1,
    PLAYER_O_WON = 2,
    GAME_DRAW = 3
};

struct tictactoe_shared_st {
    char board[3][3];
    int current_player;
    int game_status;
    int move_made;
    int game_initialized;
    int player1_ready;
    int player2_ready;
};

void display_board(char board[3][3]) {
    printf("\n   0   1   2\n");
    for(int i = 0; i < 3; i++) {
        printf("%d  %c | %c | %c\n", i, board[i][0], board[i][1], board[i][2]);
        if(i < 2) printf("  ---|---|---\n");
    }
    printf("\n");
}

void initialize_board(char board[3][3]) {
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

int check_winner(char board[3][3]) {
    for(int i = 0; i < 3; i++) {
        if(board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) return (board[i][0] == 'X') ? PLAYER_X_WON : PLAYER_O_WON;
        if(board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) return (board[0][i] == 'X') ? PLAYER_X_WON : PLAYER_O_WON;
    }
    
    if(board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) return (board[0][0] == 'X') ? PLAYER_X_WON : PLAYER_O_WON;
    if(board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) return (board[0][2] == 'X') ? PLAYER_X_WON : PLAYER_O_WON;
    
    int moves_made = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] != ' ') moves_made++;
    
    if(moves_made == 9) return GAME_DRAW;
    
    return GAME_ONGOING;
}