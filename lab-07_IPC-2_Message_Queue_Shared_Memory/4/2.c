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

    while(!game_data->game_initialized) {
        sleep(1);
        printf("Player 2: Waiting for Player 1 to initialize game...\n");
    }

    game_data->player2_ready = 1;
    printf("Player 2 (O): Joined the game!\n");
    display_board(game_data->board);

    while(game_data->game_status == GAME_ONGOING) {
        if(game_data->current_player == 2) {
            printf("Player 2 (O): Your turn. Enter row and column (0-2): ");
            scanf("%d %d", &row, &col);

            if(row < 0 || row >= 3 || col < 0 || col >= 3 || 
               game_data->board[row][col] != ' ') {
                printf("Player 2: Invalid move! Try again.\n");
                continue;
            }

            game_data->board[row][col] = 'O';
            game_data->current_player = 1;
            game_data->move_made = 1;

            game_data->game_status = check_winner(game_data->board);
            
            display_board(game_data->board);

            if(game_data->game_status != GAME_ONGOING) break;
            
            printf("Player 2: Waiting for Player 1's move...\n");
        } else {
            while(game_data->current_player == 1 && game_data->game_status == GAME_ONGOING) sleep(1);
            if(game_data->game_status != GAME_ONGOING) break;
            display_board(game_data->board);
        }
    }

    switch(game_data->game_status) {
        case PLAYER_X_WON:
            printf("Player 2 (O): Player 1 won!\n");
            break;
        case PLAYER_O_WON:
            printf("Player 2 (O): You won!\n");
            break;
        case GAME_DRAW:
            printf("Player 2 (O): It's a draw!\n");
            break;
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Game ended\n");
    exit(EXIT_SUCCESS);
}
