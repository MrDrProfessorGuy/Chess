#ifndef CHESS_PLAYER_MAP_H
#define CHESS_PLAYER_MAP_H
#include "map.h"

#ifndef TOURNAMENT_MAP_H
#ifndef CHESS_GAME_MAP_H
typedef int PlayerId;
#endif
#endif
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


#ifndef _CHESSSYSTEM_H
typedef enum {
    FIRST_PLAYER,
    SECOND_PLAYER,
    DRAW
} Winner;
#endif

typedef enum {
    PLAYER_WON,
    PLAYER_LOST,
    PLAYER_DRAW
} DuelResult;

typedef enum {
    ADD = 1,
    UNDO = -1
} UpdateMode;



Map playerCreateMap();
void playerDestroyMap(Map player_map);

bool playerExists(Map player_map, PlayerId player_id);

bool playerIdIsValid(PlayerId player_id);
/**
 * return a pointer to the player data
 * @param player_map
 * @param player_id
 * @return
 *      NULL - if the player does not exist
 *      PlayerData - otherwise
 */
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

double playerGetLevel(Map player_map, PlayerId player_id);

Map playerMapCopy(Map player_map);

PlayerId playerGetMaxLevelAndId(Map player_map, double* max_level, bool remove);
/**
 *
 * @param player_map
 * @param first_player
 * @param second_player
 * @param play_time
 * @param winner
 * @param value
 * @return
 *      PLAYER_NULL_ARGUMENT
 *      PLAYER_INVALID_ID
 *      PLAYER_NOT_EXIST
 *      PLAYER_SUCCESS
 */
PlayerResult playerUpdateDuelResult(Map player_map, PlayerId first_player, PlayerId second_player, int play_time,
                                    Winner winner, UpdateMode value);

PlayerResult playerMapUpdateStatistics(Map main_map, Map update_map, bool destroy, UpdateMode value);

PlayerResult updatePlayerDataAfterOpponentQuit(Map player_map, PlayerId player_id, DuelResult game_result);

int playerCalculateScore(PlayerData player_data);

PlayerId playerCalculateWinner(Map player_map);

PlayerResult playerCalculateAveragePlayTime(Map player_map, PlayerId player_id, double* play_time);


#endif //CHESS_PLAYER_MAP_H
