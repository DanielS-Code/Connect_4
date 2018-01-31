#include "SPMiniMax.h"
#include "SPMiniMaxNode.h"
#include <stdio.h>
#define SIZE 8
#define MIN -1000
#define MAX 1000

void ucTiles(char c1, char c2, char c3, char c4, int countVector[8])
{
    int sum = 0;

    if (c1 != SP_FIAR_GAME_EMPTY_ENTRY)
    {
        sum += c1 == SP_FIAR_GAME_PLAYER_1_SYMBOL;
        sum -= c1 == SP_FIAR_GAME_PLAYER_2_SYMBOL;
    }
    if (c2 != SP_FIAR_GAME_EMPTY_ENTRY)
    {
        sum += c2 == SP_FIAR_GAME_PLAYER_1_SYMBOL;
        sum -= c2 == SP_FIAR_GAME_PLAYER_2_SYMBOL;
    }
    if (c3 != SP_FIAR_GAME_EMPTY_ENTRY)
    {
        sum += c3 == SP_FIAR_GAME_PLAYER_1_SYMBOL;
        sum -= c3 == SP_FIAR_GAME_PLAYER_2_SYMBOL;
    }
    if (c4 != SP_FIAR_GAME_EMPTY_ENTRY)
    {
        sum += c4 == SP_FIAR_GAME_PLAYER_1_SYMBOL;
        sum -= c4 == SP_FIAR_GAME_PLAYER_2_SYMBOL;
    }

    if (sum == 0){
        return;
    }

    if (sum > 0) {
        countVector[sum + 3]++;
    }
    else if (sum < 0){
        countVector[sum + 4]++;
    }
    return;
}
/**
 *
 * @param src
 * @param Vector
 * @param row
 * @param col
 * @param checkDown
 * @param checkRight
 * @param checkLeft
 *
 */

void updateCount(SPFiarGame* src, int Vector[8], int row, int col, bool checkDown, bool checkRight, bool checkLeft){

    if (checkRight){
        ucTiles(src->gameBoard[row][col], src->gameBoard[row][col + 1],
                           src->gameBoard[row][col + 2], src->gameBoard[row][col + 3], Vector);
    }
    if (checkDown){
        ucTiles(src->gameBoard[row][col], src->gameBoard[row-1][col ],
                           src->gameBoard[row-2][col ], src->gameBoard[row-3][col], Vector);
    }
    if (checkLeft && checkDown){
        ucTiles(src->gameBoard[row][col], src->gameBoard[row - 1][col-1],
                           src->gameBoard[row - 2][col-2], src->gameBoard[row - 3][col-3], Vector);
    }
    if (checkRight && checkDown){
        ucTiles(src->gameBoard[row][col], src->gameBoard[row - 1][col+1],
                           src->gameBoard[row - 2][col+2], src->gameBoard[row - 3][col+3], Vector);
    }
}

/**
 *
 * @param currentGame
 * @return score of the current board
 */

int spMinimaxScoringFunction(SPFiarGame* currentGame){
    int weightVector[SIZE] = { MIN, -5, -2, -1, 1, 2, 5, MAX };
    int countVector[SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int i = 0, j = 0;
    int score = 0;

    if (currentGame == NULL)
    {
        return -1;
    }

    for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++){
        for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++){
            updateCount(currentGame, countVector, i, j, i >= 3, j <= SP_FIAR_GAME_N_COLUMNS - 4, j >= 3);
        }
    }

    if (countVector[0] > 0)
    {
        return MIN;
    }

    if (countVector[SIZE-1] > 0)
    {
        return MAX;
    }


    for (i = 1; i < SIZE-1; i++)
    {
        score += countVector[i] * weightVector[i];
    }

    return score;
}

/**
 *
 * @param num1
 * @param num2
 * @return
 * max between
 */
int Maximum2(int num1, int num2)
{
    if (num1 > num2)
    {
        return num1;
    }
    return num2;
}


int Minimum2(int num1, int num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;
}

/**
 * fake move
 * @param currentGame
 * @param depth
 * @param maximize
 * @return bestscore
 */
int spMinimaxSingleMove(SPFiarGame* currentGame,unsigned int depth,bool maximize){
    int i = 0;
    int score;
    int bestScore;
    bool isScoreSet = false;
    char winner;

    if (depth == 0)
    {
        return spMinimaxScoringFunction(currentGame);
    }

    //is winning board
    winner = spFiarCheckWinner(currentGame);
    if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL)
    {
        return MAX;
    }
    if (winner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
    {
        return MIN;
    }

    for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
    {
        if (spFiarGameIsValidMove(currentGame, i))
        {
            spFiarGameSetMove(currentGame, i);
            score = spMinimaxSingleMove(currentGame, depth - 1, !maximize);
            MinMaxUndo(currentGame);
            if (!isScoreSet)
            {
                bestScore = score;
                isScoreSet = true;
            }
            else
            {
                if (maximize)
                {
                    bestScore = Maximum2(bestScore, score);
                }
                else
                {
                    bestScore = Minimum2(bestScore, score);
                }
            }
        }
    }

    if (!isScoreSet)
    {
        return spMinimaxScoringFunction(currentGame);
    }

    return bestScore;
}


/**
 *
 * @param currentGame
 * @param maxDepth
 * @return place to make best move
 */
int SuggestMove(SPFiarGame* currentGame,unsigned int maxDepth){
    SPFiarGame* gameCopy;
    int i = 0;
    bool maximize = false;
    int score, bestScore , bestColumn = -1;
    bool isScoreSet = false;

    if (maxDepth <= 0 || currentGame == NULL)
    {
        return -1;
    }

    maximize = spFiarGameGetCurrentPlayer(currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL;

    for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
    {
        if (spFiarGameIsValidMove(currentGame, i))
        {
            gameCopy = spFiarGameCopy(currentGame);
            spFiarGameSetMove(gameCopy, i);
            score = spMinimaxSingleMove(gameCopy, maxDepth - 1, !maximize);
            if (!isScoreSet)
            {
                bestScore = score;
                bestColumn = i;
                isScoreSet = true;
            }
            else
            {
                if (maximize)
                {
                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestColumn = i;
                    }
                }
                else
                {
                    if (score < bestScore)
                    {
                        bestScore = score;
                        bestColumn = i;
                    }
                }
            }
            spFiarGameDestroy(gameCopy);
        }
    }

    if (!isScoreSet)
    {
        return -1; //error
    }

    return bestColumn;
}

/**
 *
 * @param src
 * @return Satus of Printing Message during MiniMax
 */

SP_FIAR_GAME_MESSAGE MinMaxUndo(SPFiarGame* src)
{
    if(src==NULL)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    if(spArrayListIsEmpty(src->historyList)==true)
        return SP_FIAR_GAME_NO_HISTORY;

    int move = spArrayListGetLast(src->historyList);
    src->gameBoard[src->tops[move]-1][move]=' ';//clean board comp turn
    src->tops[move]=(src->tops[move])-1;//update tops
    spArrayListRemoveLast(src->historyList);


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