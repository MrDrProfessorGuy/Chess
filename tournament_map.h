#ifndef TOURNAMENT_MAP_H
#define TOURNAMENT_MAP_H

#include "map.h"

typedef enum {
    TOURNAMENT_OUT_OF_MEMORY,
    CHESS_NULL_ARGUMENT,
    CHESS_INVALID_ID,
    CHESS_INVALID_LOCATION,
    CHESS_INVALID_MAX_GAMES,
    CHESS_TOURNAMENT_ALREADY_EXISTS,
    CHESS_TOURNAMENT_NOT_EXIST,
    CHESS_GAME_ALREADY_EXISTS,
    CHESS_INVALID_PLAY_TIME,
    CHESS_EXCEEDED_GAMES,
    CHESS_PLAYER_NOT_EXIST,
    CHESS_TOURNAMENT_ENDED,
    CHESS_NO_TOURNAMENTS_ENDED,
    CHESS_NO_GAMES,
    CHESS_SAVE_FAILURE,
    CHESS_SUCCESS
} TournamentResult ;

typedef int TournamentId;
typedef int PlayerId;
typedef const char* Location;



/// assign specific functions to map create
Map tournamentCreate(...);
void tournamentDestroy(Map tournament);
Map tournamentCopy(Map tournament);

bool tournamentEnded(TournamentId tournament_id);
int tournamentGetMaxGames(TournamentId tournament_id);
PlayerId tournamentGetWinnerId(TournamentId tournament_id);
Location tournamentGetLocation(TournamentId tournament_id);













#endif //TOURNAMENT_MAP_H
