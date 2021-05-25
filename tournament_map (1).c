#include "tournament_map.h"
#include "game_map.h"
#include "player_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef TournamentId* TournamentKey;
typedef struct tournament_data* TournamentData;

struct tournament_data {
    Location location;
    int max_games_per_player;
    bool has_ended;
    Map game_map;
    Map player_map;
    PlayerId winner_id;
    int num_of_games;
    int total_game_time;
    int longest_game_time;
};


/********************* Tournament functions *********************/
static void freeTournamentData(MapDataElement data);
static void freeTournamentKey(MapKeyElement key);
static MapDataElement copyTournamentData(MapDataElement data);
static MapKeyElement copyTournamentKey(MapKeyElement key);
static int compareTournamentKey(MapKeyElement key1, MapKeyElement key2);

static TournamentKey createTournamentKey(TournamentId tournament_id);
static TournamentData createTournamentData(Location location, int max_games_per_player);

static TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id);



/**********************************************************/
/********************* Map Tournament functions ***********/

static MapDataElement copyTournamentData(MapDataElement data) {
    if (!data) {
        return NULL;
    }

    //TournamentData new_data = malloc(sizeof(*new_data)); this is the same as the next line right?
    TournamentData data_copy = createTournamentData(((TournamentData)data)->location,
        ((TournamentData)data)->max_games_per_player);
    if (!data_copy) {
        return NULL;
    }

    Map game_map_copy = mapCopy(((TournamentData)data)->game_map);
    if (!game_map_copy) {
        freeTournamentData(data_copy);
        return NULL;
    }

    //in TournamentDatacreate function we create an game_map empty
    //which includes allocation of the map and deme node
    //however our mapCopy also creates and allocates a copy of a given map 
    //so if we just store tournament_data_copy->player_map = game_map_copy;
    //we're basically leaving an empty allocated map along with the deme node in the air 
    //we need to free them and then store the address of the copied map
    //we do not want to intialize tournament_data_copy->game_map= NULL; in TournamentDatacreate 
    // because when we add games to the map we want it to start with deme node
    //same goes for player_map_copy  
    mapDestroy(data_copy->game_map);
    data_copy->player_map = game_map_copy;


    Map player_map_copy = mapCopy(((TournamentData)data)->player_map);
    if (!player_map_copy) {
        freeTournamentData(data_copy);
        //mapDestroy(game_map_copy);
        return NULL;
    }

    //look at the previous note
    mapDestroy(data_copy->player_map);
    data_copy->player_map = player_map_copy;

    //is malloc and all needed? or strcpy(tournament_data_copy->location, tournament_data->location) enough?
    Location location_copy = malloc(sizeof(strlen(((TournamentData)data)->location) + 1));
    if (!location_copy) {
        freeTournamentData(data_copy);
        //mapDestroy(game_map_copy);
        //mapDestroy(player_map_copy);
        return NULL;
    }

    data_copy->location = strcpy(location_copy, ((TournamentData)data)->location);

    data_copy->winner_id = ((TournamentData)data)->winner_id;
    data_copy->max_games_per_player = ((TournamentData)data)->max_games_per_player;
    data_copy->has_ended = ((TournamentData)data)->has_ended;
    data_copy->num_of_games = ((TournamentData)data)->num_of_games;
    data_copy->total_game_time = ((TournamentData)data)->total_game_time;
    data_copy->longest_game_time = ((TournamentData)data)->longest_game_time;

    return data_copy;
}

static MapKeyElement copyTournamentKey(MapKeyElement key) {
    if (!key) {
        return NULL;
    }
    //maybe int* and int and new_key are more readable than TournamentKey and TournamentId and new_key?
    TournamentKey new_key = malloc(sizeof(TournamentId));
    if (!new_key) {
        return NULL;
    }
    *new_key = *(TournamentKey)key;
    return new_key;
}

static void freeTournamentData(MapDataElement data) {
    if (!data) {
        return;
    }
    TournamentData new_data = (TournamentData)data;// is this true?
    mapDestroy(new_data->game_map);
    mapDestroy(new_data->player_map);
    free(new_data->location);
    free(data);
}

static void freeTournamentKey(MapKeyElement key) {
    free(key);
}

static int compareTournamentKey(MapKeyElement key1, MapKeyElement key2) {
    //what the hell? all values are valid in return +/0/-..is assert okay in this case
    /*
    if (!key1 || !key2) {
        return NULL;
    }
*/
    assert(key1 || key2);
    return (*(TournamentKey)key1 - *(TournamentKey)key2);
}




/***************************************************************/
/********************* helping Tournament functions *********************/

static TournamentData createTournamentData(Location location, int max_games_per_player) {
    TournamentData tournament_data = malloc(sizeof(*tournament_data));
    if (!tournament_data) {
        return NULL;
    }

    Map game_map = gameCreateMap();

    if (!game_map) {
        freeTournamentData(tournament_data);//not complete struct but okay right?
        return NULL;
    }
    //here we already have a game_map with deme node and tournament allocated

    Map player_map = playerCreateMap();
    if (!player_map) {
        freeTournamentData(tournament_data);
        //instead of 
        //free(tournament_data);
        //mapDestroy(player_map);
        return NULL;
    }
    //here we already have a game_map with deme node and player_map with deme node and tournament allocated

    Location tournament_location = malloc(sizeof(strlen(location) + 1));//make sure this allocation is needed

    if (!tournament_location) {
        freeTournamentData(tournament_data);//same comment
        //free(tournament_data);
        //mapDestroy(game_map);
        //mapDestroy(player_map);
        return NULL;
    }
    //store address of the copied location in tournament->location
    //make sure using str functions is allowed
    tournament_data->location = strcpy(tournament_location, location);
    tournament_data->game_map = game_map;
    tournament_data->player_map = player_map;

    //here we already have two maps with deme node and location intilized and tournament allocated

    //no need I guess? 
    /*PlayerId tournament_winner = mallloc(sizeof(strlen(****) + 1));
    if (!tournament_winner) {
        free(tournament);
        mapDestroy(game_map);
        mapDestroy(player_map);
        free(tournament_location);//enough?
        return NULL;
    }
    tournament->winner = tournament_winner;*/
    //tournament_data->winner = 0;//good? or Winner[0]? as enum..

    tournament_data->winner_id = 0;

    tournament_data->max_games_per_player = max_games_per_player;
    tournament_data->has_ended = false;
    tournament_data->num_of_games = 0;
    tournament_data->total_game_time = 0;
    tournament_data->longest_game_time = 0;

    return tournament_data;
}

static TournamentKey createTournamentKey(TournamentId tournament_id) {
    if (!(tournamentIdIsValid(tournament_id))) {
        return NULL;
    }

    TournamentKey key = malloc(sizeof(*key));
    if (!key) {
        return NULL;
    }

    *key = tournament_id;

    return key;
}




/***************************************************************/
/********************* Tournament functions *********************/

Map tournamentCreate() {

    return mapCreate(copyTournamentData, copyTournamentKey, freeTournamentData,
        freeTournamentKey, compareTournamentKey);
}

void tournamentDestroy(Map tournament_map) {
    mapDestroy(tournament_map);
}

Map tournamentCopy(Map tournament_map) {

    return mapCopy(tournament_map);
}

/**
* tournamentEnded: Checks if a tournament in the map has ended. 
*
* @param tournament_map - The map to search in
* @param tournament_id - The id tournament to look for.
* @return
* 	false - if one or more of the inputs is null, or if the tournament was not found in the map.
* 	true - if the tournament has ended.
*/
bool tournamentEnded(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
    return false;// similar to what was done in mapContains
                 //how would they know if it's false or a tournament_id is invalid or id_not_found?
                 //is assert okay in all of these casese?
    }
    //there is an conversion between any ptr and void*
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        //this case could only happen if of tournament_id does not exist
        //other cases have already been checked in this function above 
        //and data element as NULL is not allowed in our implementation due to the restrictions in mapPut
        //hence, assert
        assert(tournamentContains(tournament_map, tournament_id) == false); 
        return false;
    }

    //if we're here then for sure id exists in tournament
    assert(tournamentContains(tournament_map, tournament_id) == true);

    return (data_not_copied->has_ended);

}

/**
*tournamentGet: Returns the data associated with a specific key in the map.
*			    Iterator status unchanged
*
* @param Map - The map for which to get the data element from.
* @param TournamentId - The key element which need to be found and whos data
we want to get.
* @return
*  NULL if a NULL pointer was sent or if the map does not contain the requested key or if the id is invalid.
* 	The data element associated with the key otherwise.
*/
TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return NULL;//is this consideren double check for tournament_map because it's checked if mapGet?
    }//also what about if id inValid? should we return ERROR or NULL is okay?

    //should we return a copy here? since it's not a copy in map- maybe no because our changes are on the "node" itself
    return mapGet(tournament_map, &tournament_id);
    //mapGet can return NULL only in case it doesn't have the id
    //because NULL arg has already been checked above
    //and data cannot be NULL due to the retriction in mapPut

}

//version 1
TournamentResult tournamentGetMaxGamesPerPlayer(Map tournament_map, TournamentId tournament_id, int* max_games) {
    if (!tournament_map) {
        return TOURNAMENT_NULL_ARGUMENT;//same
    }

    if (!(tournamentIdIsValid(tournament_id))) {
        return TOURNAMENT_INVALID_ID;
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);

    //If id doesn not exist in tournament
    if (!data_not_copied) {
        return TOURNAMENT_INVALID_ID;
    }

    *max_games = data_not_copied->max_games_per_player;

    return TOURNAMENT_SUCCESS;

}

//version 2
//int tournamentGetMaxGamesPerPlayer(Map tournament_map, TournamentId tournament_id, int* max_games) {
  //  if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
   //     return -1;//similar to getSize
  //  }

   // TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);

    //If id doesn not exist in tournament
   // if (!data_not_copied) {
      //  return -1;//also here?
   // }

   // return data_not_copied->max_games_per_player;;

//}


PlayerId tournamentGetWinnerId(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return -1; //again, similar to getSize
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return -1; //..
    }
    
    //this case could only happen if tournament_id exist
    assert(tournamentContains(tournament_map, tournament_id) == true);

    return data_not_copied->max_games_per_player;//no need for a copy right?
}

Location tournamentGetLocation(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return NULL;//can I return NULL as char*? 
                    //if not then ptr char* to store location and return result?
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return NULL;// as char? 
    }
    
    //this case could only happen if tournament_id exist
    assert(tournamentContains(tournament_map, tournament_id) == true);
    
    //should we copy location first and then return it? because get works with no copies 
    // but it's an array of chars..(same as data that was a ptr)
    return data_not_copied->location;
}

bool tournamentContains(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !tournamentIdIsValid(tournament_id)) {
        return false;//make sure this is okay
    }

    return mapContains(tournament_map, &tournament_id);
}

Map tournamentGetPlayerMap(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return NULL;//is NULL okay for tournament id invalid? maybe assert?
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return NULL;
    }

    // this case could only happen if tournament_id exists
    assert(tournamentContains(tournament_map, tournament_id) == true);
    
    return data_not_copied->player_map;//again not copied because we need it this way

}

Map tournamentGetGameMap(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return NULL;//same as before
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return NULL;
    }
    // this case could only happen if tournament_id exists
    assert(tournamentContains(tournament_map, tournament_id) == true);
    return data_not_copied->game_map;//again not copied because we need it this way

}

int tournamentGetNumOfGames(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return -1; //as getSize, ok?
    }
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return -1;//same as above
    }

    // this case could only happen if tournament_id exists
    assert(tournamentContains(tournament_map, tournament_id) == true);
    return data_not_copied->num_of_games;

}

int tournamentGetTotalPlayTime(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !tournamentIdIsValid(tournament_id)) {
        return -1;//same
    }
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return -1;//same
    }

    // this case could only happen if tournament_id exists
    assert(tournamentContains(tournament_map, tournament_id) == true);
    return data_not_copied->total_game_time; 
}

int tournamentGetLongestGame(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !tournamentIdIsValid(tournament_id)) {
        return -1;//same
    }
    
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return -1;//same
    }

    // this case could only happen if tournament_id exists
    assert(tournamentContains(tournament_map, tournament_id) == true);
    return data_not_copied->longest_game_time;
}

bool tournamentIdIsValid(TournamentId id) {
    if (id > 0) {
        return true;
    }
    return false;
}


void tournamentPlayerRemove(Map tournament_map, Map chess_player_map, PlayerId first_player) {
    assert(tournament_map && chess_player_map);//is this good?


    MAP_FOREACH(TournamentKey, tournament_Key, tournament_map) {
        TournamentData tournament_data = tournamentGet(tournament_map, *tournament_Key);
        assert(tournament_data);
        Map player_map = tournament_data->player_map;
        if (playerExists(player_map, first_player)) {
            Map game_map = tournament_data->game_map;
            PlayerId second_player;
            Winner winner;
            int play_time;

            while (gameGetDataByPlayerId(game_map, first_player, &second_player, &winner, &play_time, true)) {
                if (winner == FIRST_PLAYER) {
                    playerUpdateData(player_map, second_player, PLAYER_LOST);
                    playerUpdateData(chess_player_map, second_player, PLAYER_LOST);
                }
                else if (winner == DRAW) {
                    playerUpdateData(player_map, second_player, PLAYER_DRAW);
                    playerUpdateData(chess_player_map, second_player, PLAYER_DRAW);
                }
                gameRemove(game_map, first_player, second_player);
            }//while
            playerRemove(player_map, first_player);
        }//if (playerExists(player_map, player_id))


        playerRemove(chess_player_map, first_player);
        freeTournamentKey(tournament_Key);
    }//MAP_FOREACH



}