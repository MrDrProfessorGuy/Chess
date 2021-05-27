#ifndef TOURNAMENT_MAP_H
#define TOURNAMENT_MAP_H

#include "map.h"


typedef enum {
    TOURNAMENT_OUT_OF_MEMORY,
    TOURNAMENT_NULL_ARGUMENT,
    TOURNAMENT_INVALID_ID,
    TOURNAMENT_INVALID_LOCATION,
    TOURNAMENT_INVALID_MAX_GAMES,
    TOURNAMENT_TOURNAMENT_ALREADY_EXISTS,
    TOURNAMENT_TOURNAMENT_NOT_EXIST,
    TOURNAMENT_GAME_ALREADY_EXISTS,
    TOURNAMENT_INVALID_PLAY_TIME,
    TOURNAMENT_EXCEEDED_GAMES,
    TOURNAMENT_PLAYER_NOT_EXIST,
    TOURNAMENT_TOURNAMENT_ENDED,
    TOURNAMENT_NO_TOURNAMENTS_ENDED,
    TOURNAMENT_NO_GAMES,
    TOURNAMENT_SAVE_FAILURE,
    TOURNAMENT_SUCCESS
} TournamentResult ;

#ifndef CHESS_GAME_MAP_H
#ifndef CHESS_PLAYER_MAP_H
typedef int PlayerId;
#endif
#endif

typedef int TournamentId;
typedef struct tournament_data* TournamentData;
typedef char* Location;




/// assign specific functions to map create
Map tournamentCreate();
void tournamentDestroy(Map tournament);
Map tournamentCopy(Map tournament);
TournamentResult tournamentAdd(Map tournament_map, int tournament_id,
                               int max_games_per_player, const char* tournament_location);
TournamentResult tournamentRemove(Map tournament_map, TournamentId tournament_id);


bool tournamentEnded(Map tournament_map, TournamentId tournament_id);
TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id);
TournamentResult tournamentGetMaxGamesPerPlayer(Map tournament_map, TournamentId tournament_id, int* max_games);
PlayerId tournamentGetWinnerId(Map tournament_map, TournamentId tournament_id);
Location tournamentGetLocation(Map tournament_map, TournamentId tournament_id);
bool tournamentContains(Map tournament_map, TournamentId tournament_id);
Map tournamentGetPlayerMap(Map tournament_map, TournamentId tournament_id);
Map tournamentGetGameMap(Map tournament_map, TournamentId tournament_id);
bool tournamentIdIsValid(TournamentId id);
bool tournamentLocationIsValid(const char* location);

void tournamentPlayerRemove(Map tournament_map, Map player_statistics_map, PlayerId first_player);


int tournamentNumOfActiveGames(Map tournament_map, TournamentId tournament_id);
TournamentResult tournamentSaveStatistics(Map tournament_map, char* path_file);
TournamentResult tournamentUpdateStatistics(Map tournament_map, TournamentId tournament_id,
                                            int play_time,int new_players);

TournamentResult tournamentEnd(Map tournament_map, TournamentId tournament_id);



#endif //TOURNAMENT_MAP_H
