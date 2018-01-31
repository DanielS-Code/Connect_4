#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARParser.h"
#include "SPMainAux.h"
#include "SPMiniMax.h"
#include "SPFIARGame.h"

int main() {
    SPFiarGame *Game;
    SPCommand command;
    char level[5] = {0};
    int colSuggest;
    int gameOverFlag;
    char winner;
    int commandSuc;
    char commandText[1024] = {0};
   // int arg;
    while (1)
    {
        /**initialize game**/
        gameOverFlag = 0;
        command.cmd=SP_NULL_COMMAND;
        command.arg = false;
        command.arg=0;
        winner = ' ';
        commandSuc = 0;
        Game = spFiarGameCreate(20);
        int num;
        while (1)
        {
            printf("Please enter the difficulty level between [1-7]:");  //do you need to print more then once
            scanf("%s", level);
            if (strcmp(level, "quit") == 0)
            {
                spFiarGameDestroy(Game);
                printf("\nExiting...\n");
                exit(0);
            }
            if (level[1] == '\0' && level[0] <= 55 && level[0] >= 49)
            {
                break;
            }
            printf("\nError: invalid level (should be between 1 to 7)\n");
        }

        Game->level =(unsigned int) level[0]-'0';  //check casting works -- NO!

        /**end of initialization*/

        /**start game*/
        while (command.cmd!=SP_RESTART)
        {
            commandSuc =0;
            if (Game->currentPlayer == 'X')
            {
                spFiarGamePrintBoard(Game);
                while (commandSuc!=1 || command.cmd==SP_SUGGEST_MOVE ) {

                    while ((num = getchar()) != '\n' && num != EOF);
                    if (commandSuc != -999) {
                        if (command.cmd != SP_INVALID_LINE )//command.cmd==SP_UNDO_MOVE &&
                    {
                        printf("Please make the next move:\n");
                    }

                }
                    scanf("%[^\n]", commandText);

                    getCommand(&command, commandText,0);    //in this point we got a ligit command
                    commandSuc = performCommand(&command,Game);
                }
            }
            else
            {
                colSuggest = spMinimaxSuggestMove(Game, Game->level);
                printf("Computer move: add disc to column %d",colSuggest+1);
                spFiarGameSetMove(Game,colSuggest); //NEED TO ADD CHECK????????
            }
            /**game is over check*/

            if(command.cmd!=SP_RESTART)
            {
                winner = spFiarCheckWinner(Game);
                switch (winner)
                {
                    case SP_FIAR_GAME_PLAYER_1_SYMBOL:
                        printf("%s\n%s\n", "Game over: you win", "Please enter 'quit' to exit or 'restart' to start a new game!");
                        gameOverFlag = 1;
                        break;
                    case SP_FIAR_GAME_PLAYER_2_SYMBOL:
                        printf("%s\n%s\n", "Game over: computer wins", "Please enter 'quit' to exit or 'restart' to start a new game!");
                        gameOverFlag = 1;
                        break;
                    case SP_FIAR_GAME_TIE_SYMBOL:
                        printf("%s\n%s\n", "Game over: it’s a tie", "Please enter 'quit' to exit or 'restart' to start a new game!");
                        gameOverFlag = 1;
                        break;
                    default:;

                }
                commandSuc = 0;
                if (gameOverFlag==1)
                {
                    while (commandSuc==0)
                    {
                        scanf("%s", commandText);
                        getCommand(&command, commandText,0);    //in this point we got a ligit command
                        commandSuc = performCommand(&command,Game);
                    }
                }
                /**switch players*/

            }
        }

    }

}







