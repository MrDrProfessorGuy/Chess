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
#define PLAYER_DELETED -1


int gameGetLongestPlayTime(); // printed in tournament statistics
int gameGetTotalPlayTime(); // used for tournament statistics
GameResult gameRemovePlayer();
bool gameExists(Map game_map, PlayerId player1_id, PlayerId player2_id);

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

bool playTimeIsValid(int play_time);

#endif //CHESS_GAME_MAP_H
