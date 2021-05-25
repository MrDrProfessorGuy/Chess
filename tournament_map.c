#include "tournament_map.h"
#include "game_map.h"
#include "player_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


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
    int num_of_players;
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
static TournamentData createTournamentData(const char* location, int max_games_per_player);

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
    Location location_copy = malloc(sizeof(strlen(((TournamentData)data)->location) + 1));/// can discard const?
    if (!location_copy) {
        freeTournamentData(data_copy);
        //mapDestroy(game_map_copy);
        //mapDestroy(player_map_copy);
        return NULL;
    }

    data_copy->location = strcpy(location_copy, ((TournamentData)data)->location);/// can discard const?

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
    gameDestroyMap(new_data->game_map);
    playerDestroyMap(new_data->player_map);
    free(new_data->location); /// can discard const?
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

static TournamentData createTournamentData(const char* location, int max_games_per_player){
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
    tournament_data->num_of_players = 0;
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


bool tournamentIdIsValid(TournamentId id){
    if (id > 0) {
        return true;
    }
    return false;
}

int tournamentNumOfActiveGames(Map tournament_map, TournamentId tournament_id){
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return 0;
    }
    
    Map game_map =  tournamentGetGameMap(tournament_map, tournament_id);
    if (!game_map){
        return 0;
    }
    return gameGetNumOfGames(game_map);
}

static bool noTournamentsEnded(Map tournament_map){
    if (!tournament_map){
        return true;
    }
    bool no_tournament_ended = true;
    
    MAP_FOREACH(TournamentKey, tournament_Key, tournament_map){
        if (tournamentEnded(tournament_map, *tournament_Key)){
            no_tournament_ended = false;
        }
        freeTournamentKey(tournament_Key);
    }
    return no_tournament_ended;
}

TournamentResult tournamentSaveStatistics(Map tournament_map, char* path_file){
    if (!tournament_map){
        return TOURNAMENT_NULL_ARGUMENT;
    }
    
    if (noTournamentsEnded(tournament_map)){
        return TOURNAMENT_NO_TOURNAMENTS_ENDED;
    }
    FILE* stream = fopen(path_file, "w");
    if (!stream){
        return TOURNAMENT_SAVE_FAILURE;
    }
    
    MAP_FOREACH(TournamentKey, tournament_Key, tournament_map){
        TournamentData tournament_data = tournamentGet(tournament_map, *tournament_Key);
        assert(tournament_data);
        
        double average_game_time = 0;
        if (tournament_data->num_of_games > 0){
            average_game_time = (double)tournament_data->total_game_time / tournament_data->num_of_games;
        }
        fprintf(stream, "%d\n", tournament_data->winner_id);
        fprintf(stream, "%d\n", tournament_data->longest_game_time);
        fprintf(stream, "%.2f\n", average_game_time);
        fprintf(stream, "%s\n", tournament_data->location);
        fprintf(stream, "%d\n", tournament_data->num_of_games);
        fprintf(stream, "%d\n", tournament_data->num_of_players);
        
        freeTournamentKey(tournament_Key);
    }
    
    fclose(stream);
    return TOURNAMENT_SUCCESS;
}

static TournamentResult updatePlayerDataAfterOpponentQuit(Map player_map, PlayerId player_id, DuelResult game_result){
    assert(player_map);
    if (!player_map){
        return TOURNAMENT_NULL_ARGUMENT;
    }
    
    PlayerData player_data = playerGetData(player_map, player_id);
    if (!player_data){
        return TOURNAMENT_INVALID_ID;
    }
    if (game_result == PLAYER_LOST){
        player_data->num_of_loses--;
        player_data->num_of_wins++;
    }
    else if (game_result == PLAYER_DRAW){
        player_data->num_of_draws--;
        player_data->num_of_wins++;
    }
    return TOURNAMENT_SUCCESS;
}

void tournamentPlayerRemove(Map tournament_map, Map chess_player_map, PlayerId first_player){
    assert(tournament_map && chess_player_map);
    //iterate over all tournaments
    MAP_FOREACH(TournamentKey, tournament_Key, tournament_map){
        //get tournament data
        TournamentData tournament_data = tournamentGet(tournament_map, *tournament_Key);
        assert(tournament_data);
        
        if (tournament_data->has_ended){
            Map player_map = tournament_data->player_map;///maybe recover from these errors?
            Map game_map = tournament_data->game_map;
            assert(game_map);
            assert(player_map);
            if (playerExists(player_map, first_player)){
                PlayerId second_player;
                Winner winner;
                int play_time;
                //get all games the player participated in
                while (gameGetDataByPlayerId(game_map, first_player, &second_player, &winner, &play_time, true)) {
                    if (winner == FIRST_PLAYER) {
                        updatePlayerDataAfterOpponentQuit(player_map, second_player, PLAYER_LOST);
                        updatePlayerDataAfterOpponentQuit(chess_player_map, second_player, PLAYER_LOST);
                    }
                    else if (winner == DRAW) {
                        updatePlayerDataAfterOpponentQuit(player_map, second_player, PLAYER_DRAW);
                        updatePlayerDataAfterOpponentQuit(chess_player_map, second_player, PLAYER_DRAW);
                    }
                    gameRemove(game_map, first_player, second_player);
                }//while
                playerRemove(player_map, first_player);// remove the player from the tournament
            }//if (playerExists(player_map, player_id))
            
            playerRemove(chess_player_map, first_player);
        }
        freeTournamentKey(tournament_Key);
    }//MAP_FOREACH
}

TournamentResult tournamentRemove(Map tournament_map, TournamentId tournament_id){








}

TournamentResult tournamentAdd(Map tournament_map, int tournament_id,
                                    int max_games_per_player, const char* tournament_location){
 
    if (!tournament_map){
        return TOURNAMENT_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)){
        return TOURNAMENT_INVALID_ID;
    }
    if (tournamentContains(tournament_map, tournament_id)){
        return TOURNAMENT_TOURNAMENT_ALREADY_EXISTS;
    }
    if (!tournamentLocationIsValid(tournament_location)){
        return TOURNAMENT_INVALID_LOCATION;
    }
    if (max_games_per_player <= 0){
        return TOURNAMENT_INVALID_MAX_GAMES;
    }
    /// check if key should be coppied before sending to mapPut
    /// are we supposed to free key and data if mapPut failes somehow
    /// does the implementation include copying
    TournamentData tournament_data = createTournamentData(tournament_location, max_games_per_player);
    if (!tournament_data){
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    TournamentKey tournament_key = createTournamentKey(tournament_id);
    if (!tournament_key){
        freeTournamentData(tournament_data);
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    
    mapPut(tournament_map, tournament_key, tournament_data);/// copy?
    freeTournamentData(tournament_data);
    freeTournamentKey(tournament_key);
    return TOURNAMENT_SUCCESS;
}


double tournamentGetPlayerAveragePlayTime(Map tournament_map, PlayerId player_id){
    if (!tournament_map){
        return
    }
    
    
    
    
    
    
}

TournamentResult tournamentEnd(){

}

bool tournamentLocationIsValid(const char* location){
    assert(location);
    int len = strlen(location);
    
    if ('A' <= location[0] && location[0] <= 'Z'){
        for (int letter = 1; letter < len; letter++) {
            if (!(('a' <= location[letter] && location[letter] <= 'z') || location[letter] == ' ')){
                return false;
            }
        }
    }
    
    return true;
}


/*
* @return
*     CHESS_NULL_ARGUMENT - if chess is NULL.
*     CHESS_INVALID_ID - if the tournament ID number, either the players or the winner is invalid or both players
*                        have the same ID number.
*     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
*     CHESS_TOURNAMENT_ENDED - if the tournament already ended
*     CHESS_GAME_ALREADY_EXISTS - if there is already a game in the tournament with the same two players
        *                                  (both were not removed).
*     CHESS_INVALID_PLAY_TIME - if the play time is negative.
*     CHESS_EXCEEDED_GAMES - if one of the players played the maximum number of games allowed
        *     CHESS_SUCCESS - if game was added successfully.
*/
static TournamentResult checkValidityBeforeAddingGame(Map tournament_map, int tournament_id, int first_player,
                                                      int second_player, Winner winner, int play_time){
    if (!tournament_map){
        return TOURNAMENT_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id) || !playerIdIsValid(first_player) ||
        !playerIdIsValid(second_player) || first_player == second_player){
        return TOURNAMENT_INVALID_ID;
    }
    
    TournamentData tournament_data = tournamentGet(tournament_map, tournament_id);
    if(!tournament_data){
        return TOURNAMENT_TOURNAMENT_NOT_EXIST;
    }
    
    Map player_map = tournament_data->player_map;
    Map game_map = tournament_data->game_map;
    assert(game_map);
    assert(player_map);
    
    if (tournamentEnded(tournament_map, tournament_id)){
        return TOURNAMENT_TOURNAMENT_ENDED;
    }
    
    if (gameExists(game_map, first_player, second_player)){
        return TOURNAMENT_GAME_ALREADY_EXISTS;
    }
    if (!playTimeIsValid(play_time)){
        return TOURNAMENT_INVALID_PLAY_TIME;
    }
    int max_games = tournamentGetMaxGames(tournament_map, tournament_id);
    if (playerExceededGames(player_map, first_player, max_games) ||
        playerExceededGames(player_map, second_player, max_games)){
        
        return TOURNAMENT_EXCEEDED_GAMES;
    }
    return TOURNAMENT_SUCCESS;
}



TournamentResult tournamentAddGame(Map tournament_map, TournamentId tournament_id, int first_player,
                                   int second_player, Winner winner, int play_time){
    
    TournamentResult result = checkValidityBeforeAddingGame(tournament_map, tournament_id, first_player, second_player,
                                                            winner, play_time);
    if (result != TOURNAMENT_SUCCESS){
        return result;
    }
    
    TournamentData tournament_data = tournamentGet(tournament_map, tournament_id);
    Map player_map = tournament_data->player_map;
    Map game_map = tournament_data->game_map;
    //no checks since all other errors were already checked
    if (gameAdd(game_map, play_time, winner, first_player, second_player) != GAME_SUCCESS){
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    
    // if player exists nothing happens
    if (playerAdd(player_map, first_player) != PLAYER_SUCCESS){
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    if (playerAdd(player_map, second_player) != PLAYER_SUCCESS){
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    
    playerUpdateDuelResult(player_map, first_player, second_player, play_time, winner, ADD);
    
    tournament_data->num_of_games++;
    tournament_data->total_game_time += play_time;
    if (play_time > tournament_data->longest_game_time){
        tournament_data->longest_game_time = play_time;
    }

    return TOURNAMENT_SUCCESS;
}





TournamentResult tournamentUpdateStatistics(Map tournament_map, TournamentId tournament_id,
                                            int play_time,int new_players){
    
    if (!tournament_map){
        return TOURNAMENT_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)){
        return TOURNAMENT_INVALID_ID;
    }
    TournamentData tournament_data = tournamentGet(tournament_map, tournament_id);
    if (!tournament_data){
        return TOURNAMENT_TOURNAMENT_NOT_EXIST;
    }
    
    tournament_data->num_of_games++;
    tournament_data->total_game_time += play_time;
    if (play_time > tournament_data->longest_game_time){
        tournament_data->longest_game_time = play_time;
    }
    tournament_data->num_of_players += new_players;
    return TOURNAMENT_SUCCESS;
}













