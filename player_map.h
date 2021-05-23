#ifndef CHESS_PLAYER_MAP_H
#define CHESS_PLAYER_MAP_H
#include "map.h"

typedef int PlayerId;
typedef struct player_data* PlayerData;

typedef enum {
    PLAYER_OUT_OF_MEMORY,
    PLAYER_NULL_ARGUMENT,
    PLAYER_INVALID_ID,
    PLAYER_INVALID_LOCATION,
    PLAYER_INVALID_MAX_GAMES,
    PLAYER_TOURNAMENT_ALREADY_EXISTS,
    PLAYER_TOURNAMENT_NOT_EXIST,
    PLAYER_GAME_ALREADY_EXISTS,
    PLAYER_INVALID_PLAY_TIME,
    PLAYER_EXCEEDED_GAMES,
    PLAYER_NOT_EXIST,
    PLAYER_TOURNAMENT_ENDED,
    PLAYER_NO_TOURNAMENTS_ENDED,
    PLAYER_NO_GAMES,
    PLAYER_SAVE_FAILURE,
    PLAYER_SUCCESS
} PlayerResult ;

struct player_data{
    int num_of_games; // used to calculate averages when needed
    int num_of_wins; // used to calculate Level
    int num_of_loses; // used to calculate Level
    int num_of_draws; // used to calculate Level
    int total_play_time; // used to calculate average playtime
};

Map playerCreateMap();
void playerDestroyMap(Map player_map);

bool playerExists(Map player_map, PlayerId player_id);

bool playerIdIsValid(PlayerId player_id);
//can manipulate the data and update statistics
PlayerData playerGetData(Map player_map, PlayerId player_id);
bool playerExceededGames(Map player_map, PlayerId player_id, int num_of_games);
PlayerResult playerAdd(Map player_map, PlayerId player_id);
PlayerResult playerRemove(Map player_map, PlayerId player_id);


#endif //CHESS_PLAYER_MAP_H
