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
    GAME_ALREADY_EXISTS,
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

Map gameCreateMap();
void gameDestroyMap(Map game_map);

bool gameExists(Map game_map, PlayerId player1_id, PlayerId player2_id);
bool playTimeIsValid(int play_time);
/**
 * gameAdd: add a game to the given game map
 *
 * @param game_map
 * @param play_time
 * @param winner
 * @param player1_id
 * @param player2_id
 * @return GameResult
 *      GAME_INVALID_ID - one of the players id is invalid, or the same id was given
 *                        for both players
 *      GAME_ALREADY_EXISTS - the game exists
 *      GAME_INVALID_PLAY_TIME - play time is invalid
 *      GAME_OUT_OF_MEMORY - allocation failed
 *      GAME_SUCCESS - otherwise
 */
GameResult gameAdd(Map game_map, int play_time, Winner winner,
                   PlayerId player1_id, PlayerId player2_id);
/**
 * gameRemove: remove a game from the map
 * @param game_map
 * @param player1_id
 * @param player2_id
 * @return GameResult
 *      GAME_OUT_OF_MEMORY - if key allocation failed
 *      GAME_SUCCESS - otherwise
 */
GameResult gameRemove(Map game_map, PlayerId player1_id, PlayerId player2_id);
/**
 * gameRemovePlayerParticipated:
 *      remove all games a player participates in
 *
 * @param game_map
 * @param player_id
 * @return
 *      GAME_OUT_OF_MEMORY - if an allocation failed
 */
GameResult gameRemoveAllPlayerParticipatedGames(Map game_map, PlayerId player_id);

//bool gameGetSecondPlayerId(Map game_map, PlayerId player_id, PlayerId* second_player);
/**
 * get game data given the "first player"
 *
 * @param game_map
 * @param first_player
 * @param second_player
 * @param winner
 * @param play_time
 * @return
 */
bool gameGetDataByPlayerId(Map game_map, PlayerId first_player, PlayerId* second_player,
                           Winner* winner, int* play_time, bool remove);

int gameGetNumOfGames(Map game_map);



#endif //CHESS_GAME_MAP_H
