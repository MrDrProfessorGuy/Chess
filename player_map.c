#include "player_map.h"
#include <assert.h>
#include <stdlib.h>

typedef PlayerId* PlayerKey;// Key = &player_id


struct player_data{
    int num_of_games; // used to calculate averages when needed
    int num_of_wins; // used to calculate Level
    int num_of_loses; // used to calculate Level
    int num_of_draws; // used to calculate Level
    int total_play_time; // used to calculate average playtime
};

/********************* static functions *********************/
///Documentation below
static MapKeyElement copyPlayerKey(MapKeyElement player_key);
static MapDataElement copyPlayerData(MapDataElement data);
static void freePlayerKey(MapKeyElement player_key);
static void freePlayerData(MapDataElement data);
static int comparePlayerKey(MapKeyElement player1_key, MapKeyElement player2_key);


static PlayerKey createPlayerKey(PlayerId id);
static PlayerData createPlayerData();

static bool playerKeyIsValid(PlayerKey player_key);
static bool playerHasGames(Map player_map, PlayerId player_id, bool remove_if_no_games);


/********************************************************************/
/********************* Function Implementation *********************/
/******************************************************************/
/**
*copyPlayerKey: Creates a copy of target key.
*
* @param player_key - Target key.
* @return
*NULL if a NULL was sent or a memory allocation failed.
* A MapKeyElement containing the same elements as player_key.
*/
static MapKeyElement copyPlayerKey(MapKeyElement player_key){
    if (!playerKeyIsValid(player_key)){
        return NULL;
    }
    PlayerKey key_copy = createPlayerKey(*(PlayerKey)player_key);//passed by value
    if (!key_copy){
        return NULL;
    }
    return key_copy;
}
/**
*copyPlayerData: Creates a copy of target data.
*
* @param data - Target data.
* @return
*NULL if a NULL was sent or a memory allocation failed.
* A MapDataElement containing the same elements as data.
*/
static MapDataElement copyPlayerData(MapDataElement data){
    if (!data){
        return NULL;
    }
    PlayerData data_copy = createPlayerData();
    if (!data_copy){
        return NULL;
    }
    data_copy->num_of_games = ((PlayerData)data)->num_of_games;
    data_copy->num_of_wins = ((PlayerData)data)->num_of_wins;
    data_copy->num_of_loses = ((PlayerData)data)->num_of_loses;
    data_copy->num_of_draws = ((PlayerData)data)->num_of_draws;
    data_copy->total_play_time = ((PlayerData)data)->total_play_time;
    return data_copy;
}
/**
* freePlayerKey: Deallocates an existing key. Clears all elements by using the
* stored free functions.
*
* @param player_key - Target key to be deallocated. If player_key is NULL nothing will be
* 		done
*/
static void freePlayerKey(MapKeyElement player_key){
    free(player_key);
}
/**
* freePlayerData: Deallocates an existing data. Clears all elements by using the
* stored free functions.
*
* @param data - Target data to be deallocated. If data is NULL nothing will be
* 		done
*/
static void freePlayerData(MapDataElement data){
    free(data);
}
/**
    * comparePlayerKey: identifies equal key elements.
    *
    *@param player1_key - first key to be compared.
    *@param player2_key - second key to be compared.
    *
    * @return-
    * 		A positive integer if the first key is greater;
    * 		0 if they're equal;
    *		A negative integer if the second key is greater.
*/
static int comparePlayerKey(MapKeyElement player1_key, MapKeyElement player2_key){
    return (*(PlayerKey)player1_key - *(PlayerKey)player2_key);
}
/**
* createPlayerData: Allocates a new empty Player data.
*
* @return
* 	NULL - if allocations failed.
* 	A new PlayerData in case of success.
*/
static PlayerData createPlayerData(){
    PlayerData player_data = malloc(sizeof(*player_data));
    if (!player_data){
        return NULL;
    }
    
    player_data->num_of_games = 0;
    player_data->num_of_wins = 0;
    player_data->num_of_loses = 0;
    player_data->num_of_draws = 0;
    player_data->total_play_time = 0;
    
    return player_data;
}
/**
* createPlayerKey: Allocates a new key as player id.
*
* @param PlayerId - the Player id to be set as the key of the player
*
* @return
* 	NULL - if allocations failed.
* 	A new PlayerKey in case of success.
*/
static PlayerKey createPlayerKey(PlayerId id){
    if (!playerIdIsValid(id)){
        return NULL;
    }
    PlayerKey player_id = malloc(sizeof(*player_id));
    if (!player_id){
        return NULL;
    }
    *player_id = id;
    return player_id;
}
/**
* playerKeyIsValid: Checks if a player_key is valid.
*
* @param player_key - the player id to check if it's valid.
*
* @return
* 	false - if id is a smaller than or equl to 0.
* 	true - if id is a positive int bigger than 0.
*/
static bool playerKeyIsValid(PlayerKey player_key){
    if (!player_key || *player_key <= 0){
        return false;
    }
    return true;
}
/**
 * playerHasGames: returns true if a player participates in at least one game
 *
 * @param player_map - pointer to the player map
 * @param player_id  - the player id
 * @param remove_if_no_games - bool value, if true the player will be deleted if the player exist and has no games
 * @return
 *      true - if a player participates in at least one game
 *      false - in one of the following cases: the player map is NULL, the player id is invalid
 *              or data retrieval failed
 */
static bool playerHasGames(Map player_map, PlayerId player_id, bool remove_if_no_games){
    if (!player_map){
        return  false;
    }
    if (!playerIdIsValid(player_id)){
        return false;
    }
    PlayerData player_data = playerGetData(player_map, player_id);
    if (!player_data){
        false;
    }
    if (remove_if_no_games && player_data->num_of_games == 0){
        playerRemove(player_map, player_id);
        false;
    }
    return true;
}


/********************* public functions *********************/

bool playerIdIsValid(PlayerId player_id){
    if (player_id > 0){
        return true;
    }
    return false;
}

bool playerExists(Map player_map, PlayerId player_id){
    assert(player_map);
    if (mapContains(player_map, &player_id)){
        return true;
    }
    return false;
}

Map playerCreateMap(){
    return mapCreate(copyPlayerData, copyPlayerKey, freePlayerData,
                     freePlayerKey, comparePlayerKey);
}

void playerDestroyMap(Map player_map){
    if (!player_map){
        return;
    }
    mapDestroy(player_map);
}

Map playerMapCopy(Map player_map){
    if (!player_map){
        return NULL;
    }
    return mapCopy(player_map);
}

bool playerExceededGames(Map player_map, PlayerId player_id, int num_of_games){
    assert(player_map);
    PlayerData player_data = playerGetData(player_map, player_id);
    if (player_data && player_data->num_of_games >= num_of_games){
        return true;
    }
    return false;
}

PlayerResult playerAdd(Map player_map, PlayerId player_id){
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    if (playerExists(player_map, player_id)){
        return PLAYER_SUCCESS;
    }
    PlayerKey player_key = createPlayerKey(player_id);
    if (!player_key){
        return PLAYER_OUT_OF_MEMORY;
    }
    PlayerData player_data = createPlayerData();
    if (!player_data){
        freePlayerKey(player_key);
        return PLAYER_OUT_OF_MEMORY;
    }
    
    if (mapPut(player_map, player_key, player_data) != MAP_SUCCESS){
        freePlayerKey(player_key);
        freePlayerData(player_data);
        return PLAYER_OUT_OF_MEMORY;
    }
    freePlayerKey(player_key);
    freePlayerData(player_data);
    return PLAYER_SUCCESS;
}

PlayerResult playerRemove(Map player_map, PlayerId player_id){
    assert(player_map);
    if (mapContains(player_map, &player_id)){
        mapRemove(player_map, &player_id);
        return PLAYER_SUCCESS;
    }
    return PLAYER_NOT_EXIST;
}

PlayerData playerGetData(Map player_map, PlayerId player_id){
    assert(player_map);
    
    return mapGet(player_map, &player_id);
}

double playerGetLevel(Map player_map, PlayerId player_id){
    assert(player_map);
    
    PlayerData data = playerGetData(player_map, player_id);
    if (data->num_of_games == 0){
        return 0;
    }
    return (double)(6*data->num_of_wins - 10*data->num_of_loses + 2*data->num_of_draws)/(data->num_of_games);
    
}

PlayerId playerGetMaxLevelAndId(Map player_map, double* max_level, bool remove){
    assert(player_map);
    double current_level = 0;
    PlayerId max_id = 0;
    *max_level = 0;
    bool first_iteration = true;
    
    MAP_FOREACH(PlayerKey , player_key, player_map){
        if (!playerHasGames(player_map, *player_key, true)){
            continue;
        }
        current_level = playerGetLevel(player_map, *player_key);
        if (current_level > *max_level || first_iteration){
            max_id = *player_key;
            *max_level = current_level;
            first_iteration = false;
        }
        freePlayerKey(player_key);
    }
    if (remove){
        playerRemove(player_map, max_id);
    }
    return max_id;
}

PlayerResult playerUpdateDuelResult(Map player_map, PlayerId first_player, PlayerId second_player, int play_time,
                                    Winner winner, UpdateMode value){
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    if (!playerIdIsValid(first_player) || !playerIdIsValid(second_player)){
        return PLAYER_INVALID_ID;
    }
    PlayerData first_player_data = playerGetData(player_map, first_player);
    PlayerData second_player_data = playerGetData(player_map, second_player);
    
    if (!first_player_data || !second_player_data){
        return PLAYER_NOT_EXIST;
    }
    
    first_player_data->num_of_games += value;
    first_player_data->total_play_time += play_time*value;
    second_player_data->num_of_games += value;
    second_player_data->total_play_time += play_time*value;
    if (winner == FIRST_PLAYER){
        first_player_data->num_of_wins += value;
        second_player_data->num_of_loses += value;
    }
    else if (winner == SECOND_PLAYER){
        first_player_data->num_of_loses += value;
        second_player_data->num_of_wins += value;
    }
    else{
        first_player_data->num_of_draws += value;
        second_player_data->num_of_draws += value;
    }
    
    return PLAYER_SUCCESS;
}

bool playerUpdateData(PlayerData player_data1, PlayerData player_data2, UpdateMode value){
    if (!player_data1 || !player_data2){
        return false;
    }
    
    player_data1->num_of_games += value * player_data2->num_of_games;
    player_data1->total_play_time += value * player_data2->total_play_time;
    player_data1->num_of_wins += value * player_data2->num_of_wins;
    player_data1->num_of_loses += value * player_data2->num_of_loses;
    player_data1->num_of_draws += value * player_data2->num_of_draws;
    
    return true;
}

PlayerResult playerMapUpdateStatistics(Map main_map, Map update_map, bool destroy, UpdateMode value){
    if (!main_map || !update_map){
        return PLAYER_NULL_ARGUMENT;
    }
    
    MAP_FOREACH(PlayerKey, player_key, update_map){
        PlayerData update_data = playerGetData(update_map, *player_key);
        assert(update_data);
        
        PlayerData player_data = playerGetData(main_map, *player_key);
        if (!player_data){
            freePlayerKey(player_key);
            continue;
        }
        
        playerUpdateData(player_data, update_data, value);
        freePlayerKey(player_key);
    }
    
    if (destroy){
        playerDestroyMap(update_map);
    }
    return PLAYER_SUCCESS;
}

PlayerResult updatePlayerDataAfterOpponentQuit(Map player_map, PlayerId player_id, DuelResult game_result){
    assert(player_map);
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    
    PlayerData player_data = playerGetData(player_map, player_id);
    if (!player_data){
        return PLAYER_INVALID_ID;
    }
    if (game_result == PLAYER_LOST){
        player_data->num_of_loses--;
        player_data->num_of_wins++;
    }
    else if (game_result == PLAYER_DRAW){
        player_data->num_of_draws--;
        player_data->num_of_wins++;
    }
    return PLAYER_SUCCESS;
}

int playerCalculateScore(PlayerData player_data){
    if (!player_data){
        return 0;
    }
    
    return (2*player_data->num_of_wins + player_data->num_of_draws);
}

PlayerId playerCalculateWinner(Map player_map){
    if (!player_map){
        return 0;
    }
    PlayerKey winner_key = mapGetFirst(player_map);
    PlayerData winner_data = playerGetData(player_map, *winner_key);
    int winner_score = playerCalculateScore(winner_data);
    
    MAP_FOREACH(PlayerKey, player_key, player_map){
        PlayerData player_data = playerGetData(player_map, *player_key);
        int player_score = playerCalculateScore(player_data);
        if (player_score >= winner_score){
            if (player_score == winner_score){
                if (player_data->num_of_loses > winner_data->num_of_loses){
                    freePlayerKey(player_key);
                    continue;
                }
                else if (player_data->num_of_loses == winner_data->num_of_loses){
                    // <= since if they are equal the winner is by smallest id, and winner_id < player_id
                    // from the implementation of Map
                    if (player_data->num_of_wins <= winner_data->num_of_wins){
                        freePlayerKey(player_key);
                        continue;
                    }//if (player_data->num_of_wins <= winner_data->num_of_wins)
                }//else if (player_data->num_of_loses == winner_data->num_of_loses)
            }//if (player_data->num_of_loses > winner_data->num_of_loses)
            freePlayerKey(winner_key);
            winner_key = player_key;
            winner_data = playerGetData(player_map, *winner_key);
            winner_score = player_score;
            continue;
        }// if (player_score == winner_score)
        
        freePlayerKey(player_key);
    }
    
    PlayerId winner_id = *winner_key;
    freePlayerKey(winner_key);
    return winner_id;
}

PlayerResult playerCalculateAveragePlayTime(Map player_map, PlayerId player_id, double* play_time){
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    if (!playerIdIsValid(player_id)){
        return PLAYER_INVALID_ID;
    }
    PlayerData player_data = playerGetData(player_map, player_id);
    if (!player_data){
        return PLAYER_NOT_EXIST;
    }
    
    if (player_data->num_of_games == 0){
        *play_time = (double) 0;
        return PLAYER_SUCCESS;
    }
    *play_time = (double)(player_data->total_play_time)/player_data->num_of_games;
    return PLAYER_SUCCESS;
}









