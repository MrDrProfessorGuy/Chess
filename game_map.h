#ifndef CHESS_GAME_MAP_H
#define CHESS_GAME_MAP_H
#include "map.h"

typedef enum {// make sure no changes need to be done to this result enum
    GAME_OUT_OF_MEMORY,
    GAME_NULL_ARGUMENT,
    GAME_INVALID_ID,
    GAME_INVALID_LOCATION,
    GAME_INVALID_MAX_GAMES,
    GAME_TOURNAMENT_ALREADY_EXISTS,
    GAME_TOURNAMENT_NOT_EXIST,
    GAME_GAME_ALREADY_EXISTS,
    GAME_INVALID_PLAY_TIME,
    GAME_EXCEEDED_GAMES,
    GAME_PLAYER_NOT_EXIST,
    GAME_TOURNAMENT_ENDED,
    GAME_NO_TOURNAMENTS_ENDED,
    GAME_NO_GAMES,
    GAME_SAVE_FAILURE,
    GAME_SUCCESS
} GameResult ;

typedef enum {
    FIRST_PLAYER,
    SECOND_PLAYER,
    DRAW
} Winner;

typedef int PlayerId;
#define PLAYER_DELETED -1

GameResult gameAdd(Map game_map, int play_time, Winner winner,
                    PlayerId player1_id, PlayerId player2_id);
int gameGetLongestPlayTime(); // printed in tournament statistics
int gameGetTotalPlayTime(); // used for tournament statistics
GameResult gameRemovePlayer();





#endif //CHESS_GAME_MAP_H
