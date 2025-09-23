/*
Write a two-player 3x3 tic-tac-toe console game using shared memory.
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "tictactoe_com.h"

int main() {
    void *shared_memory = (void*)0;
    struct tictactoe_shared_st *game_data;
    int shmid;
    int row, col;

    shmid = shmget((key_t)1234, sizeof(struct tictactoe_shared_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void*)0, 0);
    if (shared_memory == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    game_data = (struct tictactoe_shared_st*)shared_memory;

    if(!game_data->game_initialized) {
        initialize_board(game_data->board);
        game_data->current_player = 1;
        game_data->game_status = GAME_ONGOING;
        game_data->move_made = 0;
        game_data->player1_ready = 1;
        game_data->player2_ready = 0;
        game_data->game_initialized = 1;
        printf("Player 1 (X): Game initialized. Waiting for Player 2...\n");
    }

    while(!game_data->player2_ready) {
        sleep(1);
        printf("Player 1: Waiting for Player 2 to join...\n");
    }

    printf("Player 1 (X): Game started!\n");
    display_board(game_data->board);

    while(game_data->game_status == GAME_ONGOING) {
        if(game_data->current_player == 1) {
            printf("Player 1 (X): Your turn. Enter row and column (0-2): ");
            scanf("%d %d", &row, &col);

            if(row < 0 || row >= 3 || col < 0 || col >= 3 || 
               game_data->board[row][col] != ' ') {
                printf("Player 1: Invalid move! Try again.\n");
                continue;
            }

            game_data->board[row][col] = 'X';
            game_data->current_player = 2;
            game_data->move_made = 1;

            game_data->game_status = check_winner(game_data->board);
            
            display_board(game_data->board);

            if(game_data->game_status != GAME_ONGOING) break;
            
            printf("Player 1: Waiting for Player 2's move...\n");
        } else {
            while(game_data->current_player == 2 && game_data->game_status == GAME_ONGOING) sleep(1);
            if(game_data->game_status != GAME_ONGOING) break;
            display_board(game_data->board);
        }
    }

    switch(game_data->game_status) {
        case PLAYER_X_WON:
            printf("Player 1 (X): You won!\n");
            break;
        case PLAYER_O_WON:
            printf("Player 1 (X): Player 2 won!\n");
            break;
        case GAME_DRAW:
            printf("Player 1 (X): It's a draw!\n");
            break;
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Game ended\n");
    exit(EXIT_SUCCESS);
}
