//
// Created by Daniel on 15-Jun-17.
//
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include "SPMiniMax.h"
#include <stdio.h>
#include <stdlib.h>

int performCommand (SPCommand* command, SPFiarGame* Game)
{
    int colSuggest;
    SP_FIAR_GAME_MESSAGE message= SP_FIAR_GAME_NULL;

    switch (command->cmd)
    {
        case SP_SUGGEST_MOVE:
            colSuggest = spMinimaxSuggestMove(Game, Game->level);
            printf("Suggested move: drop a disc to column %d\n", colSuggest+1);
            return -999;

        case SP_RESTART:
            spFiarGameDestroy(Game);
            printf("Game restarted!\n");
            return  1;

        case SP_UNDO_MOVE:    //to finish this

            message = spFiarGameUndoPrevMove(Game);
            if (message==SP_FIAR_GAME_INVALID_ARGUMENT || message==SP_FIAR_GAME_NO_HISTORY)
            {
                printf("Error: cannot undo previous move!\n");
                return  -999;
            }
            else
            {
                spFiarGameUndoPrevMove(Game);
                return 1;
            }

        case SP_ADD_DISC:
            message = spFiarGameSetMove(Game, command->arg -1);
            if (message == SP_FIAR_GAME_INVALID_ARGUMENT) {
                printf("Error: column number must be in range 1-7\n");
                return -999;
            }
            if (message == SP_FIAR_GAME_INVALID_MOVE)
            {
                printf("Error: column %d is full\n", command->arg);
                return -999;
            }
            return 1;

        case SP_QUIT:
            spFiarGameDestroy(Game);
            printf("Exiting...\n");
            exit(0);




        default: return 0;

    }
}
/**
 *
 * gets command in text and checks 2 cases:
 * if the game is over checks that the command is one of 3 (quit,restart,undp_ and its valid.
 * else checks only if the command is valid.
 * returns nothing -> updates command.
 */
void getCommand(SPCommand* command, char* commandText, int gameOver)
{
    *command = spParserPraseLine(commandText);
    while ( (command->cmd == SP_INVALID_LINE) || (gameOver==1 && (command->cmd!=SP_QUIT ||command->cmd!=SP_RESTART ||command->cmd!=SP_UNDO_MOVE)))
    {
        if ((!(command->cmd == SP_INVALID_LINE)) && (gameOver==1 && (command->cmd!=SP_QUIT ||command->cmd!=SP_RESTART ||command->cmd!=SP_UNDO_MOVE)))
        {
            printf("Error: the game is over\n");
        }
        else
        {
            printf("Error: invalid command\n");
        }
        int num;

        while ((num = getchar()) != '\n' && num != EOF);

        //printf("Please make the next move:\n");

        scanf("%[^\n]", commandText);
        *command = spParserPraseLine(commandText);
    }

}



