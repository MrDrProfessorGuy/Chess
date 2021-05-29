#ifndef CHESS_PLAYER_MAP_H
#define CHESS_PLAYER_MAP_H
#include "map.h"

/** Key element data type for map container */
typedef int PlayerId;

/** Data element data type for map container */
typedef struct player_data* PlayerData;

/** Type used for returning error codes from player map functions */
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


/**
* playerCreateMap: Allocates a new empty player map.
*
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map playerCreateMap();
/**
* playerDestroyMap: Deallocates an existing player map. Clears all elements by using the
* stored free functions.
*
* @param player_map - Target tournament_map to be deallocated. If player_map is NULL nothing will be
* 		done
*/
void playerDestroyMap(Map player_map);
/**
* playerExists: checks if a player exists in a given players map.
*
* @param player_map -  the players map to search in.
* @param player_id -  the player_id associated with a player- must be valid.
* @return
* 	true - if player exist in the players map
* 	false - if not.
*/
bool playerExists(Map player_map, PlayerId player_id);
/**
* playerIdIsValid: checks if a id player is valid.
*
* @param player_id -  the player_id associated with a player.
* @return
* 	true - if player_id>0.
* 	false - otherwise.
*/
bool playerIdIsValid(PlayerId player_id);
/**
* playerGetData: Returns the data associated with a specific player_id in the player_map.
*			Iterator status unchanged.
*
* @param player_map -  the player_map to look into.
* @param player_id -  the player_id associated with a player.
*
* @return
*      NULL - if the player does not exist.
*      PlayerData - otherwise.
*/
PlayerData playerGetData(Map player_map, PlayerId player_id);
/**
* playerExceededGames: Returns if the player has reached maximum number of games he is allowed to play
*
* @param player_map -  the player_map to look into - must not be NULL.
* @param player_id -  the player_id associated with a player.
* @param num_of_games - new num of players game to be compared
* @return
*      true - if the player has reached maximum number of gamest.
*      false - if the player_map does not contain the requested player_id.
*/
bool playerExceededGames(Map player_map, PlayerId player_id, int num_of_games);
/**
 *  playerAdd- adds a player to the players map, does nothing if already exists
 *
* @param player_map -  the player_map to look into.
* @param player_id -  the player_id associated with a player.
 * @return
 *      PLAYER_NULL_ARGUMENT - if player_map is NULL.
 *      PLAYER_SUCCESS - if the player associated with the player_id already exist in the player_map
 *                       or if he wad addded successfuly.
 *      PLAYER_OUT_OF_MEMORY - if alllocations fail
 */
PlayerResult playerAdd(Map player_map, PlayerId player_id);
/**
* playerRemove: removes a player from the players map.
*
* @param player_map -  the player_map to look into- must not be NULL.
* @param player_id -  the player_id associated with a player to be removed.
* @return
*      PLAYER_NOT_EXIST - if player doesn't exist in the player_map
*      PLAYER_SUCCESS - removement was successful.
*/
PlayerResult playerRemove(Map player_map, PlayerId player_id);
/**
* playerGetLevel: Returns the level of a player.
*
* @param player_map -  the player_map to look into - must not be NULL.
* @param player_id -  the player_id associated with a player to calculate and return its level.
*
* @return
*      0 - if num of games of the given player is zero.
*      double - the calculated level of the player associated with the player_id.
*/
double playerGetLevel(Map player_map, PlayerId player_id);
/**
* playerMapCopy: Creates a copy of player_map.
* Iterator values for both player_map is undefined after this operation.
*
* @param player_map - Target player_map.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Map containing the same elements as player_map otherwise.
*/
Map playerMapCopy(Map player_map);
/**
* playerGetMaxLevelAndId: Returns highest level of all players and the player's id associated with it.
*
* @param player_map - the player_map to look into - must not be NULL.
* @param max_level  -  pointer to store the max level in.
* @param remove     -  a boolean flag that indicates whether should the player be removed or not.
*
* @return
*      PlayerId - the player's id with the highest level in the players map.
*/
PlayerId playerGetMaxLevelAndId(Map player_map, double* max_level, bool remove);
/**
 *playerUpdateDuelResult - update the results of two players
 *
 * @param player_map    - the player_map to look into.
 * @param first_player  - the id of the first player to update his results.
 * @param second_player - the id of the second player to update his results
 * @param play_time     - time of the game
 * @param winner        - the winner up to this stage in the game of the two players
 * @param value         - 1 or -1
 *
 * @return
 *      PLAYER_NULL_ARGUMENT - if player_map is NULL.
 *      PLAYER_INVALID_ID - if the first player's id or the second player's id are invalid.
 *      PLAYER_NOT_EXIST - if the one of the player's id does not exist in the players map
 *      PLAYER_SUCCESS   - the update has been successfuly done.
 */
PlayerResult playerUpdateDuelResult(Map player_map, PlayerId first_player, PlayerId second_player, int play_time,
                                    Winner winner, UpdateMode value);
/**
 *playerUpdateData - updates statistics map(update_map) with values of the first map (player_map).
 *
 * @param main_map        - pointer to player map with statistics values
 * @param update_map      - pointer to second player map.
 * @param destroy         - remove the players in player_map after updating the update_map.
 * @param value           - states whether an update should be made
 * @return
 *      PLAYER_NULL_ARGUMENT - if player_map or update_map is NULL.
 *      PLAYER_SUCCESS   - the update has been successfully done.
 */
PlayerResult playerMapUpdateStatistics(Map main_map, Map update_map, bool destroy, UpdateMode value);
/**
 *updatePlayerDataAfterOpponentQuit - update the results of the remaining player's data after his opponent quit
 *
 * @param player_map    - the player_map to look into.
 * @param player_id     - the id of the player that has quit.
 * @param game_result   - DuelResult value indicating if the player has won, lost or there was a draw
 *
 * @return
 *      PLAYER_NULL_ARGUMENT - if player_map is NULL.
 *      PLAYER_INVALID_ID - if the player's id is invalid.
 *      PLAYER_SUCCESS   - the update has been successfuly done.
 */
PlayerResult updatePlayerDataAfterOpponentQuit(Map player_map, PlayerId player_id, DuelResult game_result);
/**
 *playerCalculateScore - calculate the score of a player
 *
 * @param player_data    - pointer to player_data to calculate the score from.
 *
 * @return
 *      0 - if player_map is NULL.
 *      int - the calculated score of a given data player.
 *            player_score = ( num_of_wins * 2 + num_of_draws * 1 ) / ( num_of_games_of_player )
 * */
int playerCalculateScore(PlayerData player_data);
/**
*   The winner is the player with the highest score:
*   player_score = ( num_of_wins * 2 + num_of_draws * 1 ) / ( num_of_games_of_player )
*   If two players have the same score, the player with least losses will be chosen.
*   If two players have the same number of losses, the player with the most wins will be chosen
*   If two players have the same number of wins and losses,
*   the player with smaller id will be chosen.
*   Once the tournament is over, no games can be added for that tournament.
*
* @param player_map - the player_map to search from.
*
* @return-
*       0- if player_map is NULL.
*       PlayerId - the winner's id otherwise.
 */
PlayerId playerCalculateWinner(Map player_map);
/**
 *playerUpdateDuelResult - update the results of two players
 *
 * @param player_map    - the player_map to look into.
 * @param player_id     - the player's id to calculate the average play time to.
 * @param play_time     - a pointer to store the play time average in.
 *
 * @return
 *      PLAYER_NULL_ARGUMENT - if player_map is NULL.
 *      PLAYER_INVALID_ID - if the player's id in invalid.
 *      PLAYER_NOT_EXIST - if the one of the player's id does not exist in the players map
 *          0            -  if nums of games is zero.
 *      PLAYER_SUCCESS   - the storement of the play_time has been successfuly done.
 */
PlayerResult playerCalculateAveragePlayTime(Map player_map, PlayerId player_id, double* play_time);


#endif //CHESS_PLAYER_MAP_H
