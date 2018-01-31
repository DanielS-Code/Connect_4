
#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"
#include "SPFIARParser.h"
#include <string.h>


/**
 *
 * @param str
 * @return true if int else false
 */
bool spParserIsInt(const char* str){
    int i=0;
    if(str[0]=='-') {
        i++;

    }
    while(1){
        if(str[i]==0) {
            break;
        }

        if(str[i]<'0'||str[i]>'9')
            return false;

        i++;
    }

    return true;
}
/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */

SPCommand spParserPraseLine(const char* str) {
    const char *SUGGESTMOVE = "suggest_move";
    const char *UNDOMOVE = "undo_move";
    const char *ADDDISC = "add_disc";
    const char *QUIT = "quit";
    const char *RESTARTGAME = "restart_game";
    int num;
    char commandText[1024];
    SPCommand com;
    if(strcmp(SUGGESTMOVE,str)==0) {//suggest

        com.validArg = false;
        com.cmd = SP_SUGGEST_MOVE;
        return com;
    }
    if(strcmp(RESTARTGAME,str)==0) {//restart

        com.validArg = false;
        com.cmd = SP_RESTART;
        return com;
    }
    if(strcmp(QUIT,str)==0) {//QUIT GAME LIKE A BOSS

        com.validArg = false;
        com.cmd = SP_QUIT;
        return com;
    }
    if(strcmp(UNDOMOVE,str)==0) {//Undo

        com.validArg = false;
        com.cmd = SP_UNDO_MOVE;
        return com;
    }
     sscanf(str, "%s%d", commandText, &num);

    if(strcmp(commandText,ADDDISC)==0) {//Undo

        com.validArg = true;
        com.cmd = SP_ADD_DISC;
        com.arg=num;
        return com;
    }
    com.validArg=false;
    com.cmd=SP_INVALID_LINE;
    return  com ;

}





