#ifndef CHESS_GAME_MAP_H
#define CHESS_GAME_MAP_H
#include "map.h"

/** Type used for returning error codes from game map functions */
typedef enum {
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

#ifndef _CHESSSYSTEM_H
#ifndef CHESS_PLAYER_MAP_H
/*
    Type for specifying who is the winner in a certain game
*/
typedef enum {
    FIRST_PLAYER,
    SECOND_PLAYER,
    DRAW
} Winner;
#endif
#endif

#ifndef TOURNAMENT_MAP_H
#ifndef CHESS_PLAYER_MAP_H
/** Key element data type for game map container */
typedef int PlayerId;
#endif
#endif

/** Data element data type for game map container */
typedef struct game_data* GameData;

/**
* gameCreateMap: Allocates a new empty game map.
*
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map gameCreateMap();
/**
* playerDestroyMap: Deallocates an existing game map. Clears all elements by using the
* stored free functions.
*
* @param game_map - Target game_map to be deallocated. If game_map is NULL nothing will be
* 		done
*/
void gameDestroyMap(Map game_map);
/**
* gameMapCopy: Creates a copy of game_map.
* Iterator values for both game_map is undefined after this operation.
*
* @param game_map - Target game_map - Must not be NULL
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Map containing the same elements as game_map otherwise.
*/
Map gameMapCopy(Map game_map);
/**
* gameExists: checks if a game exists in a given games map.
*
* @param game_map -  the games map to search in- must not be NULL.
* @param player1_id -  the first player id associated with the first player- must be valid.
* @param player2_id -  the second player id associated with the second player- must be valid.
* @return
* 	true - if a game exist in the game_map.
* 	false - if not or if allocation fails.
*/
bool gameExists(Map game_map, PlayerId player1_id, PlayerId player2_id);
/**
* playTimeIsValid: checks if a given play time is valid.
*
* @param play_time -  the play time.
* @return
* 	true - if play_time>=0.
* 	false - otherwise.
*/
bool playTimeIsValid(int play_time);
/**
 * gameAdd: adds a game to a given game map
 *
 * @param game_map -the game_map to add the new game to.
 * @param play_time - play time of the new game
 * @param winner - the default winner as the game begins
 * @param player1_id - the first player's id
 * @param player2_id - the second player's id.
 * @return GameResult
 *      GAME_NULL_ARGUMENT - if game_map is NULL
 *      GAME_INVALID_ID - one of the players id is invalid, or the same id was given
 *                        for both players.
 *      GAME_ALREADY_EXISTS - the game exists
 *      GAME_INVALID_PLAY_TIME - play time is invalid
 *      GAME_OUT_OF_MEMORY - allocation failed
 *      GAME_SUCCESS - otherwise
 */
GameResult gameAdd(Map game_map, int play_time, Winner winner,
                   PlayerId player1_id, PlayerId player2_id);
/**
 * gameRemove: removes a game from a given game map.
 *
 * @param game_map - the map from which to remove the game.
 * @param player1_id - the first player's id int the game to remove.
 * @param player2_id - the first player's id int the game to remove.
 * @return GameResult
 *      GAME_INVALID_ID - one of the players id is invalid, or the same id was given
 *                        for both players.
 *      GAME_OUT_OF_MEMORY - if key allocation failed.
 *      GAME_SUCCESS - otherwise
 */
GameResult gameRemove(Map game_map, PlayerId player1_id, PlayerId player2_id);
/**
 * gameRemovePlayerParticipated: remove all games that a player participates in.
 *
 * @param game_map - the games map to remove from - must not be NULL.
 * @param player_id - the player's id that is asscoiated with all games to remove from the map.
 * @return
 *      GAME_OUT_OF_MEMORY - if an allocation failed.
 *      GAME_SUCCESS- otherwise.
 */
bool gameGetDataByPlayerId(Map game_map, PlayerId first_player, PlayerId* second_player,
                           int* winner, int* play_time, bool remove);
/**
* gameGetNumOfGames: returns the num of games in a given tournament.
*
* @param game_map - The game_map to look into.
*
* @return
*   0 - if tournament_map is NULL.
* 	int - the num of games otherwise.
*
*/
int gameGetNumOfGames(Map game_map);



#endif //CHESS_GAME_MAP_H
