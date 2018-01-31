#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include "SPFIARGame.h"
//Put all decleartions and constants here
int SuggestMove(SPFiarGame* currentGame,unsigned int maxDepth);
int spMinimaxSingleMove(SPFiarGame* currentGame,unsigned int depth,bool maximize);
int Maximum2(int num1, int num2);
int spMinimaxScoringFunction(SPFiarGame* currentGame);
void updateCount(SPFiarGame* src, int countVector[8], int row, int col, bool checkDown, bool checkRight, bool checkLeft);
void ucTiles(char c1, char c2, char c3, char c4, int countVector[8]);
SP_FIAR_GAME_MESSAGE MinMaxUndo(SPFiarGame* src);
int Minimum2(int num1, int num2);




#endif
