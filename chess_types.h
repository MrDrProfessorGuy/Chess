#ifndef CHESS_CHESS_TYPES_H
#define CHESS_CHESS_TYPES_H

#include "chessSystem.h"
#include "map.h"
/**************************************************************/
/********************* Type definitions *********************/

typedef struct player_statistics* PlayerStatistics;






struct player_statistics{
    int num_of_games; // used to calculate averages when needed
    int num_of_wins; // used to calculate Level
    int num_of_loses; // used to calculate Level
    int num_of_draws; // used to calculate Level
    int total_play_time; // used to calculate average playtime
};

bool playerIdIsValid(PlayerId player1_id);
bool validPlayTime(int play_time){
    if (play_time > 0){
        return true;
    }
    return false;
}





#endif //CHESS_CHESS_TYPES_H
