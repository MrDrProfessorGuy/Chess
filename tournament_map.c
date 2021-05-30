#include "tournament_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef TournamentId* TournamentKey;

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

/********************* Tournament functions *********************/
static void freeTournamentData(MapDataElement data);
static void freeTournamentKey(MapKeyElement key);
static MapDataElement copyTournamentData(MapDataElement data);
static MapKeyElement copyTournamentKey(MapKeyElement key);
static int compareTournamentKey(MapKeyElement key1, MapKeyElement key2);

static TournamentKey createTournamentKey();
static TournamentData createTournamentData(const char* location, int max_games_per_player);




/**********************************************************/
/********************* Map Tournament functions ***********/
/**
* copyTournamentData: Creates a copy of target data.
*
* @param data - Target data.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A MapDataElement containing the same elements as data.
*/
static MapDataElement copyTournamentData(MapDataElement data) {
    if (!data) {
        return NULL;
    }
    TournamentData data_copy = createTournamentData(((TournamentData)data)->location,
                                          ((TournamentData)data)->max_games_per_player);
    if (!data_copy) {
        return NULL;
    }
    //location copy and max games assignment accured in createTournamentData
    assert(data_copy->location);
    assert(data_copy->max_games_per_player == ((TournamentData)data)->max_games_per_player);
    
    if (!data_copy->location) {
        freeTournamentData(data_copy);
        return NULL;
    }
    
    gameDestroyMap(data_copy->game_map);
    data_copy->game_map = gameMapCopy(((TournamentData)data)->game_map);
    if (!data_copy->game_map) {
        freeTournamentData(data_copy);
        return NULL;
    }
    
    playerDestroyMap(data_copy->player_map);
    data_copy->player_map = playerMapCopy(((TournamentData)data)->player_map);
    if (!data_copy->player_map) {
        freeTournamentData(data_copy);
        return NULL;
    }
    
    data_copy->winner_id = ((TournamentData)data)->winner_id; 
    data_copy->has_ended = ((TournamentData)data)->has_ended;
    data_copy->num_of_games = ((TournamentData)data)->num_of_games;
    data_copy->total_game_time = ((TournamentData)data)->total_game_time;
    data_copy->longest_game_time = ((TournamentData)data)->longest_game_time;
    data_copy->num_of_players = ((TournamentData)data)->num_of_players;
    
    return data_copy;
}
/**
* copyTournamentKey: Creates a copy of target key.
*
* @param key - Target key.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A MapKeyElement containing the same elements as data.
*/
static MapKeyElement copyTournamentKey(MapKeyElement key) {
    if (!key) {
        return NULL;
    }
    TournamentKey new_key= malloc(sizeof(TournamentId));
    if (!new_key) {
        return NULL;
    }
    *new_key = *(TournamentKey)key;
    return new_key;
}
/**
* freeTournamentData: Deallocates an existing data. Clears all elements by using the
* stored free functions.
*
* @param data - Target data to be deallocated. If data is NULL nothing will be
* 		done
*/
static void freeTournamentData(MapDataElement data) {
    if (!data) {
        return;
    }
    TournamentData new_data = (TournamentData)data;
    gameDestroyMap(new_data->game_map);
    playerDestroyMap(new_data->player_map);
    free(new_data->location);
    free(data);
}
/**
* freeTournamentKey: Deallocates an existing key. Clears all elements by using the
* stored free functions.
*
* @param key - Target key to be deallocated. If key is NULL nothing will be
* 		done
*/
static void freeTournamentKey(MapKeyElement key) {
    if (!key) {
        return;
    }
    free(key); 
}
/**
* compareTournamentKey: identifies equal key elements.
*
*@param key1 - first key to be compared.
*@param key2 - second key to be compared.
*
* @return-
* 		A positive integer if the first key is greater;
* 		0 if they're equal;
*		A negative integer if the second key is greater.
*/
static int compareTournamentKey(MapKeyElement key1, MapKeyElement key2) {
    assert(key1 && key2);
    return (*(TournamentKey)key1 - *(TournamentKey)key2);
}




/***************************************************************/
/********************* helping Tournament functions *********************/
/**
* createTournamentData: Allocates a new empty tournament data.
*
* @param const char* - the location to be set in the tournament
* @param int - the max number of games allowed for each player in tournament
*
* @return
* 	NULL - if allocations failed.
* 	A new TournamentData in case of success.
*/
static TournamentData createTournamentData(const char* location, int max_games_per_player){
    assert(location);
    
    TournamentData tournament_data = malloc(sizeof(*tournament_data));
    if (!tournament_data) {
        return NULL;
    }
    
    tournament_data->game_map = gameCreateMap();
    if (!tournament_data->game_map) {
        free(tournament_data);
        return NULL;
    }
    
    tournament_data->player_map = playerCreateMap();
    if (!tournament_data->player_map) {
        gameDestroyMap(tournament_data->game_map);
        free(tournament_data);
        return NULL;
    }
    
    tournament_data->location = malloc(strlen(location) + 1);
    strcpy(tournament_data->location, location);
    if (!tournament_data->location) {
        playerDestroyMap(tournament_data->player_map);
        gameDestroyMap(tournament_data->game_map);
        free(tournament_data);
        return NULL;
    }
    
    tournament_data->winner_id = 0;
    tournament_data->max_games_per_player = max_games_per_player;
    tournament_data->has_ended = false;
    tournament_data->num_of_games = 0;
    tournament_data->num_of_players = 0;
    tournament_data->total_game_time=0;
    tournament_data->longest_game_time=0;
    
    return tournament_data;
}
/**
* createTournamentKey: Allocates a new key as tournament id.
*
* @param TournamentId - the Tournament id to be set as the key of the tournament
*
* @return
* 	NULL - if allocations failed.
* 	A new TournamentKey in case of success.
*/
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
    if (!tournament_map){
        return;
    }
    mapDestroy(tournament_map);
}

TournamentResult tournamentRemove(Map tournament_map, TournamentId tournament_id){
    if (!tournament_map){
        return TOURNAMENT_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)){
        return TOURNAMENT_INVALID_ID;
    }

    if (mapRemove(tournament_map, &tournament_id) != MAP_SUCCESS){
        return TOURNAMENT_TOURNAMENT_NOT_EXIST;
    }
    
    return TOURNAMENT_SUCCESS;
}

bool tournamentEnded(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !(tournamentIdIsValid(tournament_id))) {
        return false;
    }
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        assert(tournamentContains(tournament_map, tournament_id) == false);
        return false;
    }
    
    assert(tournamentContains(tournament_map, tournament_id) == true);
    return (data_not_copied->has_ended);
    
}

TournamentData tournamentGet(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !tournamentIdIsValid(tournament_id)) {
        return NULL;
    }
    
    return mapGet(tournament_map, &tournament_id);
}

TournamentResult tournamentGetMaxGamesPerPlayer(Map tournament_map, TournamentId tournament_id, int* max_games) {
    if (!tournament_map) {
        return TOURNAMENT_NULL_ARGUMENT;
    }
    
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return TOURNAMENT_INVALID_ID;
    }
    
    *max_games = data_not_copied->max_games_per_player;
    return TOURNAMENT_SUCCESS;

}

bool tournamentContains(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || !tournamentIdIsValid(tournament_id)) {
        return false;
    }
    
    return mapContains(tournament_map, &tournament_id);
}

Map tournamentGetPlayerMap(Map tournament_map, TournamentId tournament_id) {
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return NULL;
    }
    
    TournamentData data_not_copied = tournamentGet(tournament_map, tournament_id);
    if (!data_not_copied) {
        return NULL;
    }
    
    assert(tournamentContains(tournament_map, tournament_id) == true);
    return data_not_copied->player_map;
    
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

bool tournamentIdIsValid(TournamentId id){
    if (id > 0) {
        return true;
    }
    return false;
}

int tournamentNumOfGames(Map tournament_map, TournamentId tournament_id){
    if (!tournament_map || (!tournamentIdIsValid(tournament_id))) {
        return 0;
    }
    
    TournamentData tournament_data =  tournamentGet(tournament_map, tournament_id);
    if (!tournament_data){
        return 0;
    }
    return tournament_data->num_of_games;
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

void tournamentPlayerRemove(Map tournament_map, Map player_statistics_map, PlayerId first_player){
    assert(tournament_map && player_statistics_map);
    //iterate over all tournaments
    MAP_FOREACH(TournamentKey, tournament_Key, tournament_map){
        //get tournament data
        TournamentData tournament_data = tournamentGet(tournament_map, *tournament_Key);
        assert(tournament_data);
        
        if (tournament_data->has_ended == false){
            Map player_map = tournament_data->player_map;///maybe recover from these errors?
            Map game_map = tournament_data->game_map;
            assert(game_map);
            assert(player_map);
            if (playerExists(player_map, first_player)){
                PlayerId second_player;
                int winner;
                int play_time;
                //get all games the player participated in
                while (gameGetDataByPlayerId(game_map, first_player, &second_player, &winner, &play_time, true)) {
                    if (winner == FIRST_PLAYER) {
                        updatePlayerDataAfterOpponentQuit(player_map, second_player, PLAYER_LOST);
                        updatePlayerDataAfterOpponentQuit(player_statistics_map, second_player, PLAYER_LOST);
                    }
                    else if (winner == DRAW) {
                        updatePlayerDataAfterOpponentQuit(player_map, second_player, PLAYER_DRAW);
                        updatePlayerDataAfterOpponentQuit(player_statistics_map, second_player, PLAYER_DRAW);
                    }
                    gameRemove(game_map, first_player, second_player);
                }//while
                playerRemove(player_map, first_player);// remove the player from the tournament
            }//if (playerExists()
        }
        freeTournamentKey(tournament_Key);
    }//MAP_FOREACH
    playerRemove(player_statistics_map, first_player);
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
    TournamentData tournament_data = createTournamentData(tournament_location, max_games_per_player);
    if (!tournament_data){
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    TournamentKey tournament_key = createTournamentKey(tournament_id);
    if (!tournament_key){
        freeTournamentData(tournament_data);
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    if (mapPut(tournament_map, tournament_key, tournament_data) != MAP_SUCCESS){
        freeTournamentKey(tournament_key);
        freeTournamentData(tournament_data);
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    freeTournamentKey(tournament_key);
    freeTournamentData(tournament_data);
    return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentEnd(Map tournament_map, TournamentId tournament_id){

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
    if (tournamentEnded(tournament_map, tournament_id)){
        return TOURNAMENT_TOURNAMENT_ENDED;
    }
    assert(tournament_data->game_map);
    if (tournament_data->num_of_games <= 0){
        return TOURNAMENT_NO_GAMES;
    }
    
    PlayerId winner_id = playerCalculateWinner(tournament_data->player_map);
    assert(playerIdIsValid(winner_id));
    
    tournament_data->has_ended = true;
    tournament_data->winner_id = winner_id;
    return TOURNAMENT_SUCCESS;
}

bool tournamentLocationIsValid(const char* location){
    assert(location);
    int len = (int)strlen(location);
    
    if ('A' <= location[0] && location[0] <= 'Z'){
        for (int letter = 1; letter < len-1; letter++) {
            if (!(('a' <= location[letter] && location[letter] <= 'z') || location[letter] == ' ')){
                return false;
            }
        }
        return true;
    }
    
    return false;
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













