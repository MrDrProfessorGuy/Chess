#ifndef CHESS_CHESS_TYPES_H
#define CHESS_CHESS_TYPES_H

#include "chessSystem.h"
#include "map.h"

/**************************************************************/
/********************* Type definitions *********************/
typedef int* PlayerId;// Key = player_id
typedef double PlayerLevel;

typedef struct player_statistics* PlayerStatistics;
typedef struct chess_player_data* ChessPlayerData;

typedef char* GameId; /// key = "player1_id"+ '#' +"player2_id"
typedef struct game_data* GameData;

typedef int* TournamentId;// keys = 1,2,3,....
typedef struct tournament_data* TournamentData;

typedef const char* Location;

struct player_statistics{
    int num_of_games; // used to calculate averages when needed
    int num_of_wins; // used to calculate Level
    int num_of_loses; // used to calculate Level
    int num_of_draws; // used to calculate Level
    int total_play_time; // used to calculate average playtime
};

#endif //CHESS_CHESS_TYPES_H
