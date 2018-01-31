#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include "SPFIARGame.h"
#include "SPFIARParser.h"
//put auxiliary functions and constants used by the main function here.
void getCommand(SPCommand* command, char* commandText, int gameOver);
int performCommand (SPCommand* command, SPFiarGame* Game);

#endif
