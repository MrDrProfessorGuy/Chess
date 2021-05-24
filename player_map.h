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
/**
 *  add a player to the map, does nothing if already exists
 *
 * @param player_map - note: assuming the map is valid
 * @param player_id - note: assuming player_id is valid
 * @return
 *      PLAYER_OUT_OF_MEMORY - if allocation error occurred
 *      PLAYER_SUCCESS - otherwise
 */
PlayerResult playerAdd(Map player_map, PlayerId player_id);
/**
 * playerRemove: remove a player from the map
 *      assuming valid player_map
 * @param player_map
 * @param player_id
 * @return
 *      PLAYER_NOT_EXIST - if player doesn't exist
 *      PLAYER_SUCCESS
 */
PlayerResult playerRemove(Map player_map, PlayerId player_id);


#endif //CHESS_PLAYER_MAP_H
