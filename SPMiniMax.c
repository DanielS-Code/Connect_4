#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"
#include "SPFIARGame.h"


int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth)
{
    int NextMove=SuggestMove(currentGame,maxDepth);
    return NextMove;

}

