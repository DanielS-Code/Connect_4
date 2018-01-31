//
// Created by Vlad on 5/28/2017.
//
#include <stdio.h>
#include "SPFIARGame.h"
#include "SPArrayList.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

SPFiarGame* spFiarGameCreate(int historySize){
    //char board[6][7] = {""};
    SPFiarGame* game=(SPFiarGame*)calloc(1,sizeof(SPFiarGame));
    if(historySize<=0 || game==NULL)
        return NULL;
    game->historyList = spArrayListCreate(historySize);
    game->currentPlayer='X';
    for(int i=0; i<7 ; i++)
    {
        game->tops[i] = 0;
        for(int j=0; j<6; j++)
        {

            game->gameBoard[j][i]= ' ';
        }
    }
    game->level = 0;
    return game;

}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
    SPFiarGame* copy=(SPFiarGame*)calloc(1,sizeof(SPFiarGame));
    if(src==NULL || copy==NULL){
        return NULL;
    }
    memcpy(copy, src, sizeof(*src));
    copy->historyList=spArrayListCopy(src->historyList); //NOT SURE ITS NEEDED WITH MEMCPY@@@@ DANIEL TO CHECK

    return  copy;

}

void spFiarGameDestroy(SPFiarGame* src){

    if(src!=NULL){
        spArrayListDestroy(src->historyList);
        free(src);
    }
}

/**
* Sets the next move in a given game by specifying column index. The
        * columns are 0-based and in the range [0,SP_FIAR_GAME_N_COLUMNS -1].
*
* @param src - The target game
* @param col - The target column, the columns are 0-based
* @return
* SP_FIAR_GAME_INVALID_ARGUMENT - if src is NULL or col is out-of-range
        * SP_FIAR_GAME_INVALID_MOVE - if the given column is full.
*/

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
    if (src == NULL || col > SP_FIAR_GAME_N_COLUMNS - 1 || col < 0)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    if (spFiarGameIsValidMove(src,col)==false)
    {
        return SP_FIAR_GAME_INVALID_MOVE;
    }
    src->gameBoard[(src->tops[col])][col] = src->currentPlayer;/**Put Disk*/
    src->tops[col] = src->tops[col] + 1;/**Update top list*/
    if (spArrayListIsFull(src->historyList) == true)
    {                                                     /**add to history*/
        spArrayListRemoveFirst(src->historyList);

    }

    spArrayListAddLast(src->historyList, col);

    if (src->currentPlayer == 'X')
    {
        src->currentPlayer = 'O';
    }
    else
    {
        src->currentPlayer = 'X';
    }
    return SP_FIAR_GAME_SUCCESS;
}




/**
 * Checks if a disk can be put in the specified column.
 *
 * @param src - The source game
 * @param col - The specified column
 * @return
 * true  - if the a disc can be put in the target column
 * false - otherwise.
 */
bool spFiarGameIsValidMove(SPFiarGame* src, int col){

    char Check=src->gameBoard[5][col];
    if(Check=='X' || Check=='O')
        return false;
    return  true;

}

/**
 * Removes a disc that was put in the previous move and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_FIAR_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * SP_FIAR_GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */


SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src)
{
    if(src==NULL)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    if(spArrayListIsEmpty(src->historyList)==true)
        return SP_FIAR_GAME_NO_HISTORY;

    int move = spArrayListGetLast(src->historyList);
    src->gameBoard[src->tops[move]-1][move]=' ';//clean board comp turn
    src->tops[move]=(src->tops[move])-1;//update tops
    spArrayListRemoveLast(src->historyList);


    if (src->currentPlayer == 'O')
    {
        printf("Remove disc: remove user's disc at column %d",move+1);
        src->currentPlayer = 'X';

    }
    else
    {

        printf("Remove disc: remove computer's disc at column %d\n",move+1);
        src->currentPlayer = 'O';
    }
    return SP_FIAR_GAME_SUCCESS;

}


/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The characters 'X' and 'O' are used to represent
 * the discs of player 1 and player 2, respectively.
 *
 * @param src - the target game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_FIAR_GAME_SUCCESS - otherwise
 *
 */
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
    if (src == NULL)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    char arr[8][17] = {" "};
    int counter = 49;
    for (int i=0;i<17;i++)
    {
        arr[6][i] = '-';
    }
    for (int i = 0; i < 6; i++) {
        arr[i][0] = '|';
        arr[i][16] = '|';
    }
    for (int i = 2; i <= 14; i = i + 2) {
        arr[7][i] = (char)counter;
        counter++;
        for (int j = 0; j < 6; j++) {
            arr[j][i] = src->gameBoard[5 - j][(i / 2) - 1];
        }
    }
    for (int i=0;i<8;i++)
    {
        printf("\n");
        for(int j=0;j<17;j++)
        {
            printf("%c",arr[i][j]);
        }
    }
    printf("\n");
    return SP_FIAR_GAME_SUCCESS;
}



/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if it's player one's turn
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if it's player two's turn
 * SP_FIAR_GAME_EMPTY_ENTRY     - otherwise
 */
char spFiarGameGetCurrentPlayer(SPFiarGame* src){
    if(src==NULL){
        return  SP_FIAR_GAME_EMPTY_ENTRY;
    }
    if(src->currentPlayer=='X')
        return SP_FIAR_GAME_PLAYER_1_SYMBOL;
    else
        return SP_FIAR_GAME_PLAYER_2_SYMBOL;

}
/**
* Checks if there's a winner in the specified game status. The function returns either
* SP_FIAR_GAME_PLAYER_1_SYMBOL or SP_FIAR_GAME_PLAYER_2_SYMBOL in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value SP_FIAR_GAME_TIE_SYMBOL is returned. in any other case the null characters
* is returned.
* @param src - the source game
* @return
* SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 won
* SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 won
* SP_FIAR_GAME_TIE_SYMBOL - If the game is over and there's a tie
* null character - otherwise
*/
char spFiarCheckWinner(SPFiarGame* src) {
    int lastMoveColumn = spArrayListGetLast(src->historyList);
    int lastMoveRow = src->tops[lastMoveColumn] - 1;
    char ch1, ch2, ch3, ch4;
    int fBoard = 1;
    char check = ' ';
    if (src->currentPlayer == 'X') {
        check = 'O';
    } else {
        if (src->currentPlayer == 'O') {
            check = 'X';
        }
    }
    for (int i = 0; i < 4; i++) //check row
    {
        ch1 = src->gameBoard[lastMoveRow][i];
        ch2 = src->gameBoard[lastMoveRow][i + 1];
        ch3 = src->gameBoard[lastMoveRow][i + 2];
        ch4 = src->gameBoard[lastMoveRow][i + 3];
        if ((ch1 == check) && (ch2 == check) && (ch3 == check) && (ch4 == check)) {
            return check;
        }
    }
    for (int i = 0; i < 3; i++) //check column
    {
        ch1 = src->gameBoard[i][lastMoveColumn];
        ch2 = src->gameBoard[i + 1][lastMoveColumn];
        ch3 = src->gameBoard[i + 2][lastMoveColumn];
        ch4 = src->gameBoard[i + 3][lastMoveColumn];
        if ((ch1 == check) && (ch2 == check) && (ch3 == check) && (ch4 == check)) {
            return check;
        }
    }
    for (int i = 3; i < 6; i++) { //check right diagonal
        for (int j = 6; j >= 3; j--) {
            ch1 = src->gameBoard[i][j];
            ch2 = src->gameBoard[i - 1][j - 1];
            ch3 = src->gameBoard[i - 2][j - 2];
            ch4 = src->gameBoard[i - 3][j - 3];
            if ((ch1 == check) && (ch2 == check) && (ch3 == check) && (ch4 == check)) {
                return check;
            }
        }
    }
    for (int i = 3; i < 6; i++) {//check left diagonal
        for (int j = 0; j <= 3; j++) {
            ch1 = src->gameBoard[i][j];
            ch2 = src->gameBoard[i - 1][j + 1];
            ch3 = src->gameBoard[i - 2][j + 2];
            ch4 = src->gameBoard[i - 3][j + 3];
            if ((ch1 == check) && (ch2 == check) && (ch3 == check) && (ch4 == check)) {
                return check;
            }
        }

        for (int i = 0; i < 7 && fBoard == 1; i++) //check if board is full and there is a tie
        {
            if (src->gameBoard[5][i] != 'X' || src->gameBoard[5][i] != 'O') {
                fBoard = 0;
            }
        }
        if (fBoard == 1) {
            return SP_FIAR_GAME_TIE_SYMBOL;
        }

    }
    return ' ';
}


