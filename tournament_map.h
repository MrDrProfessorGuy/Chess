#ifndef TOURNAMENT_MAP_H
#define TOURNAMENT_MAP_H

#include "map.h"
#include "player_map.h"
#include "game_map.h"

/** Type used for returning error codes from tournament map functions */
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

/** Key element data type for tournament map container */
typedef int TournamentId;
/** Data element data type for tournament map container */
typedef struct tournament_data* TournamentData;
/** typedef for tournament Location */
typedef char* Location;


/**
* tournamentCreate: Allocates a new empty tournament map.
*
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map tournamentCreate();
/**
* tournamentDestroy: Deallocates an existing tournament map. Clears all elements by using the
* stored free functions.
*
* @param tournament_map - Target tournament_map to be deallocated. If tournament_map is NULL nothing will be
* 		done
*/
void tournamentDestroy(Map tournament);
/**
*  tournamentAdd: Gives a specified tournament id a specific data.
*  Iterator's value is undefined after this operation.
*
* @param tournament_map - The tournament_map for which to reassign the data element
* @param tournament_id - The tournament_id which need to be reassigned
* @param data - The new data element to associate with the given tournament_id.
*      A copy of the element will be inserted as supplied by the copying function
*      which is given at initialization and old data memory would be
*      deleted using the free function given at initialization.
* @return
* 	TOURNAMENT_NULL_ARGUMENT if a NULL was sent to the function as tournament_map
*   TOURNAMENT_INVALID_ID if tournament_id<=0
*   TOURNAMENT_ALREADY_EXISTS if tournament_id does not already exists in the tournament_map.
* 	TOURNAMENT_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
*   TOURNAMENT_INVALID_LOCATION if the location is not in the right form
*   TOURNAMENT_INVALID_MAX_GAMES if max_games_per_player<=0
* 	TOURNAMENT_SUCCESS the paired elements had been inserted successfully
*/
TournamentResult tournamentAdd(Map tournament_map, int tournament_id,
                               int max_games_per_player, const char* tournament_location);
/**
*  tournamentRemove: Removes a pair of tournament id and data elements from the tournament map. The elements
*  are found using the comparison function given at initialization. Once found,
*  the elements are removed and deallocated using the free functions
*  supplied at initialization.
*  Iterator's value is undefined after this operation.
*
* @param tournament_map - The tournament_map to remove the elements from.
* @param tournament_id  - The tournament_id to find and remove from the tournament_map.
*   The element will be freed using the free function given at initialization.
* 	The data element associated with this tournament_id will also be freed
*   using the free function given at initialization.
* @return
*  TOURNAMENT_NULL_ARGUMENT if a NULL was sent to the function.
*  TOURNAMENT_INVALID_ID- if tournament_id<=0.
*  TOURNAMENT_NOT_EXIST if tournament_id does not already exists in the tournament_map.
*  TOURNAMENT_SUCCESS the paired elements had been removed successfully.
*/
TournamentResult tournamentRemove(Map tournament_map, TournamentId tournament_id);
/**
    * tournamentEnded: Checks if a tournament_map has ended.
    *
    * @param tournament_map - The tournament_map to search in.
    * @param tournament_id - The id tournament to look for.
    * @return
    * 	false - if one or more of the inputs is null, or if the tournament was not found in the map.
    * 	true - if the tournament has ended.
*/
bool tournamentEnded(Map tournament_map, TournamentId tournament_id);
/**
*	tournamentGet: Returns the data associated with a specific key in the map.
*			Iterator status unchanged
*
* @param tournament_map - The tournament_map for which to get the data element from.
* @param tournament_id - The tournament_id element which need to be found and whose data
we want to get.
* @return
*  NULL if a NULL pointer was sent, or if tournament_id<=0, or if the map does not contain the requested key.
* 	The data element associated with the tournament_id otherwise.
*/
TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id);
/**
* tournamentGetMaxGamesPerPlayer: stores max games allowed per player in tournament_map in a pointer.
*
* @param tournament_map - The tournament_map to search in.
* @param tournament_id - The id tournament to look for.
* @param max_games - The pointer in which to store the max games per player.

*
* @return
*  TOURNAMENT_NULL_ARGUMENT if a NULL was sent to the function.
*  TOURNAMENT_NOT_EXIST if tournament_id does not already exists in the tournament_map.
*  TOURNAMENT_SUCCESS if the tournament was found in the tournament_map and the max games was stored.
*/
TournamentResult tournamentGetMaxGamesPerPlayer(Map tournament_map, TournamentId tournament_id, int* max_games);
/**
* tournamentContains: Checks if a tournament exists in the tournament_map. The key element will be
* considered in the map if one of the key elements in the map it determined equal
* using the comparison function used to initialize the map.
*
* @param tournament_map - The tournament_map to search in
* @param tournament_id - The tournament_id to look for in the tournament_map.
*         Will be compared using the comparison function.
* @return
* 	false - if one or more of the inputs is null, or if the tournament_id was not found.
* 	true - if the tournament_id was found in the map.
*/
bool tournamentContains(Map tournament_map, TournamentId tournament_id);
/**
* tournamentGetPlayerMap: returns players map, not copied.
*
* @param tournament_map - The tournament_map to search in.
* @param tournament_id - The tournament_id to look for in the tournament_map.
*
* @return
* 	NULL - if tournament_map is null, or if the tournament_id was not found.
* 	Map - if the tournament_id was found in the map.
*/
Map tournamentGetPlayerMap(Map tournament_map, TournamentId tournament_id);
/**
* tournamentGetPlayerMap: returns games map, not copied.
*
* @param tournament_map - The tournament_map to search in.
* @param tournament_id - The tournament_id to look for in the tournament_map.
*
* @return
* 	NULL - if tournament_map is null, or if the tournament_id was not found.
* 	Map - if the tournament_id was found in the map.
*/
Map tournamentGetGameMap(Map tournament_map, TournamentId tournament_id);
/**
* tournamentIdIsValid: Checks if a tournament id is valid.
*
* @param id - the tournament id to check if it's valid.
*
* @return
* 	false - if id is a smaller than or equl to 0.
* 	true - if id is a positive int bigger than 0.
*/
bool tournamentIdIsValid(TournamentId id);
/**
* tournamentLocationIsValid: Checks if a tournament location is valid.
*
* @param location - the tournament location to check.
*
* @return
* 	true - if the location starts with capital letter and followed by either small letter or tabs.
* 	false - otherwise.
*/
bool tournamentLocationIsValid(const char* location);
/**
    *  tournamentPlayerRemove: removes the player from a given tournament in a given tournament_map.
     *                         In games where the player has participated and not yet ended,
     *                         the opponent is the winner automatically after removal.
     *                         If both player of a game were removed, the game still exists in the system.
     *
    *  Iterator's value is undefined after this operation.
    *
    * @param tournament_map - The tournament_map to search in for the tournament to remove the player from.
    * @param tournament_id  - The tournament_id associated with the tournament to find and remove from.
    * @param player_statistics_map - pointer to a player map with the statistics of all players.
*/
void tournamentPlayerRemove(Map tournament_map, Map player_statistics_map, PlayerId first_player);
/**
* tournamentNumOfActiveGames: returns the num of games in a given tournament.
*
* @param tournament_map - The tournament_map to search in.
* @param tournament_id - The tournament_id to look for.
* @return
*   0 - if tournament_map is NULL or if tournament_id<=0
        or if the tournament associated with the tournament_id does not exist in the tournament map
* 	int - the num of games otherwise.
*
*/
int tournamentNumOfGames(Map tournament_map, TournamentId tournament_id);
/**
* tournamentSaveStatistics: prints statistics of each tournament in tournament_map into a file .
*
* @param tournament_map - The tournament_map to search in.
* @param path_file - The path to file into which to write the statistics of each tournament.
*
* @return
*  TOURNAMENT_NULL_ARGUMENT if tournament_map is NULL.
*  TOURNAMENT_NO_TOURNAMENTS_ENDED if none of the tournaments in the tournament_map has ended
*  TOURNAMENT_SAVE_FAILURE if open/allocation of path_file has failed.
*  TOURNAMENT_SUCCESS- otherwise.
*/
TournamentResult tournamentSaveStatistics(Map tournament_map, char* path_file);
/*
* tournamentUpdateStatistics: updates statistics in a given tournament.
*
* @param tournament_map - The tournament_map to search in.
* @param tournament_id - the tournament, to update, associated with tournament_id to update.
* @param play_time - play time to compare to longest play time.
* @param new_players - num of new player to add.
* @return
*  TOURNAMENT_NULL_ARGUMENT if tournament_map is NULL.
*  TOURNAMENT_INVALID_ID if tournament_id is<=0
*  TOURNAMENT_NOT_EXIST if the tournament associated with tournament_id does not exist in the tournament_map.
*  TOURNAMENT_SUCCESS- otherwise.
*/
TournamentResult tournamentUpdateStatistics(Map tournament_map, TournamentId tournament_id,
                                            int play_time,int new_players);
/**
 * tournamentEnded: The function will end the tournament if it has at least one game and
 *                     calculate the id of the winner.
 *                     The winner of the tournament is the player with the highest score:
 *                     player_score = ( num_of_wins * 2 + num_of_draws * 1 ) / ( num_of_games_of_player )
 *                     If two players have the same score, the player with least losses will be chosen.
 *                     If two players have the same number of losses, the player with the most wins will be chosen
 *                     If two players have the same number of wins and losses,
 *                     the player with smaller id will be chosen.
 *                     Once the tournament is over, no games can be added for that tournament.
 *
 * @param tournament_map - the tournament of which must be ended and calulate its id winner
 * @param tournament_id - the tournament id. Must be positive, and unique.
 *
 * @return
 *     TOURNAMENT_NULL_ARGUMENT - if tournament_map is NULL.
 *     TOURNAMENT_INVALID_ID - if the tournament_id number is invalid.
 *     TOURNAMENT_NOT_EXIST - if the tournament does not exist.
 *     TOURNAMENT_ENDED - if the tournament already ended
 *     TOURNAMENT_NO_GAMES - if the tournament does not have any games.
 *     TOURNAMENT_SUCCESS - if tournament was ended successfully.
 */
TournamentResult tournamentEnd(Map tournament_map, TournamentId tournament_id);



#endif //TOURNAMENT_MAP_H
