#include "tournament_map.h"
#include "game_map.h"
#include "player_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ID_TOURNAMENT_NOT_EXIST_IN_CHESS -1;

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



/*Element copyInt(Element element) {
    if (element == NULL) {
        return NULL;
    }
    int* newInt = malloc(sizeof(int));
    if (newInt == NULL) {
        return NULL;
    }
    *newInt = *(int*)element;
    return newInt;
}
void freeInt(Element element) {
    free(element);
}


*/

/********************* Tournament functions *********************/
static void freeTournamentData(MapDataElement data);
static void freeTournamentKey(MapKeyElement key);
static MapDataElement copyTournamentData(MapDataElement data);
static MapKeyElement copyTournamentKey(MapKeyElement key);
static int compareTournamentKey(MapKeyElement key1, MapKeyElement key2);

static TournamentKey createTournamentKey();
static TournamentData createTournamentData(Location location, int max_games_per_player);

static TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id);








/**********************************************************/
/********************* Map Tournament functions ***********/

static MapDataElement copyTournamentData(MapDataElement data) {
    if (!data) {
        return NULL;
    }
   
    //TournamentData new_data = malloc(sizeof(*new_data)); - this is the same as the next line right?
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
    TournamentKey new_key= malloc(sizeof(TournamentId));
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

static TournamentData createTournamentData(Location location, int max_games_per_player){
    TournamentData tournament_data = malloc(sizeof(*tournament_data));
    if (!tournament_data) {
        return NULL;
    }
    //change to mapGameCreate
    Map game_map = gameCreateMap();

    if (!game_map) {
        free(tournament_data);
        return NULL;
    }
    //here we already have a game_map with deme node and tournament allocated
        
    //change to mapPlayerCreate
    Map player_map = playerCreateMap();
    if (!player_map) {
        free(tournament_data);
        mapDestroy(game_map);
        return NULL;
    }
    //here we already have a game_map with deme node and player_map with deme node and tournament allocated

    Location tournament_location = malloc(sizeof(strlen(location) + 1));//make sure this allocation is needed

    if (!tournament_location) {
        free(tournament_data);
        mapDestroy(game_map);
        mapDestroy(player_map);
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
    tournament_data->num_of_games=0;
    tournament_data->total_game_time=0;
    tournament_data->longest_game_time=0;
    
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

//changed tournament to tournament_map due to tournamentEnded - being consistant 
void tournamentDestroy(Map tournament_map) {
    mapDestroy(tournament_map);
}

Map tournamentCopy(Map tournament_map) {
    
    return mapCopy(tournament_map);
}

bool tournamentEnded(Map tournament_map,TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return -1;// not good how would they know if it's false or a null argument was sent?
                  //is assert okay?
    }
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (data_not_copied) {
        return (data_not_copied->has_ended);
    }

    assert(!tournamentContains(tournament_map, tournament_id));
    return false;

}


TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id) {
    //is there a need for (!map return NULL etc..) it's checked in mapGet..
    
    //should we return a copy here? since it's not a copy in map
    return mapGet(tournament_map, &tournament_id);

}

TournamentResult tournamentGetMaxGamesPerPlayer(Map tournament_map, TournamentId tournament_id, int* max_games) {
    if (!tournament_map) {
        return TOURNAMENT_NULL_ARGUMENT;//same
    }
    
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    
    //and again make sure the case (!data) should/n't happen
    if (!data_not_copied) {
        return TOURNAMENT_INVALID_ID;
    }
    
    *max_games = data_not_copied->max_games_per_player;
    
    return TOURNAMENT_SUCCESS;

}

//here playerID is int..so maybe int like above? 
PlayerId tournamentGetWinnerId(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return 0; //again here what to return?
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
        //and again make sure the case (!data) should/n't happen
        if (data_not_copied) {
            return (data_not_copied->max_games_per_player);//no need for a copy right? 
        }
    return -1; //here NULL ARGUMENT CANNOT HAPPEN DUE TO THE CHECK ABOVE
        // !tournament_map

}

Location tournamentGetLocation(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return NULL;//can I return NULL as cont char*?
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    //and again make sure the case (!data) should/n't happen
    if (!data_not_copied) {
        return NULL;
    }
    
    return data_not_copied->location;
}

bool tournamentContains(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return 0;//make sure this is okay
    }

    return mapContains(tournament_map, &tournament_id);
}

Map tournamentGetPlayerMap(Map tournament_map, TournamentId tournament_id) {
    //!tournament_map is checked in maps functions so is there a need to check it here?
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return NULL;
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    //and again make sure the case (!data) should/n't happen
    if (!data_not_copied) {
        return NULL;
    }
    
    return data_not_copied->player_map;

}

Map tournamentGetGameMap(Map tournament_map, TournamentId tournament_id) {
    //!tournament_map is checked in maps functions so is there a need to check it here?
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return NULL;
    }

    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    //and again make sure the case (!data) should/n't happen
    if (!data_not_copied) {
        return NULL;
    }

    return data_not_copied->game_map;

}

int tournamentGetNumOfGames(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return 0;//or -1 or what?
    }
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    //and again make sure the case (!data) should/n't happen
    if (!data_not_copied) {
        return 0;//same as above
    }

    return data_not_copied->num_of_games;//should this be copied? it's int..

}

int tournamentGetTotalPlayTime(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return 0;//same
    }
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    //and again make sure the case (!data) should/n't happen
    if (!data_not_copied) {
        return 0;//same
    }

    return data_not_copied->total_game_time;//should this be copied? it's int..no right?
                                            //the user doesn't get an address just a value of int 

}

int tournamentGetLongetGame(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return 0;//same
    }
    Map game_map = tournamentGetPlayerMap(tournament_map, tournament_id);
    int tournament_game_map_size = mapGetSize(game_map);//why couldn't do
                                                   //mapGetSize(tournament_map->game_map)
    int count = 1;// 1 because there's always the deme node 

    TournamentKey key_id = mapGetFirst(tournament_map);// since we have automatic hamara from void* to any kind

    while (count <= tournament_game_map_size) {
        
        if (compareTournamentKey(key_id, &tournament_id) == 0) {//is this okay? sending (void*, int*)?
            TournamentData not_copied_data = tournamentGet(tournament_map, *key_id);
            //check if valid
            freeTournamentKey(key_id);
            return not_copied_data->longest_game_time;
        }
        freeTournamentKey(key_id);
        key_id = mapGetNext(game_map);
        count++;
    }
    

}

bool tournamentIdIsValid(TournamentId id){
    if (id > 0) {
        return true;
    }
    return false;
}



void tournamentPlayerRemove(Map tournament_map, Map chess_player_map, PlayerId first_player){
    assert(tournament_map && chess_player_map);
    
    
    MAP_FOREACH(TournamentKey, tournament_Key, tournament_map){
        TournamentData tournament_data = tournamentGet(tournament_map, *tournament_Key);
        assert(tournament_data);
        Map player_map = tournament_data->player_map;
        if (playerExists(player_map, first_player)){
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









