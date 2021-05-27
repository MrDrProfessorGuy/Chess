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
static MapKeyElement copyPlayerKey(MapKeyElement player_key);
static MapDataElement copyPlayerData(MapDataElement data);
static void freePlayerKey(MapKeyElement player_key);
static void freePlayerData(MapDataElement data);
static int comparePlayerKey(MapKeyElement player1_key, MapKeyElement player2_key);


static PlayerKey createPlayerKey(PlayerId id);
static PlayerData createPlayerData();

static bool playerKeyIsValid(PlayerKey player_key);



/********************************************************************/
/********************* Function Implementation *********************/
/******************************************************************/

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
static void freePlayerKey(MapKeyElement player_key){
    free(player_key);
}
static void freePlayerData(MapDataElement data){
    free(data);
}
static int comparePlayerKey(MapKeyElement player1_key, MapKeyElement player2_key){
    return (*(PlayerKey)player1_key - *(PlayerKey)player2_key);
}


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
static PlayerKey createPlayerKey(PlayerId id){
    if (!playerIdIsValid(id)){// can remove if checked everywhere else
        return NULL;
    }
    PlayerKey player_id = malloc(sizeof(*player_id));
    if (!player_id){
        return NULL;
    }
    *player_id = id;
    return player_id;
}

static bool playerKeyIsValid(PlayerKey player_key){
    if (!player_key || *player_key <= 0){
        return false;
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
    mapDestroy(player_map);
}

bool playerExceededGames(Map player_map, PlayerId player_id, int num_of_games){
    assert(player_map);
    PlayerData player_data = mapGet(player_map, &player_id);
    //assert(player_data);
    if (!player_data || player_data->num_of_games <= num_of_games){
        return false;
    }
    return true;
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
    return (double)(6*data->num_of_wins - 10*data->num_of_loses + 2*data->num_of_loses)/(data->num_of_games);
    
}

Map playerMapCopy(Map player_map){
    assert(player_map);
    return mapCopy(player_map);
}

PlayerId playerGetMaxLevelAndId(Map player_map, double* max_level, bool remove){
    assert(player_map);
    double current_level = 0;
    PlayerId max_id = 0;
    *max_level = 0;
    
    MAP_FOREACH(PlayerKey , player_key, player_map){
        current_level = playerGetLevel(player_map, *player_key);
        if (current_level > *max_level){
            max_id = *player_key;
            *max_level = current_level;
        }
        freePlayerKey(player_key);
    }
    if (remove && *max_level > 0){
        playerRemove(player_map, max_id);
    }
    return max_id;
}

 /*
PlayerResult playerUpdateData(Map player_map, PlayerId first_player, int play_time, DuelResult result){
    assert(player_map);
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    
    PlayerData player_data = playerGetData(player_map, first_player);
    if (!player_data){
        return PLAYER_INVALID_ID;
    }
    
    if (result == PLAYER_LOST){
        player_data->num_of_loses--;
        
    }
    else if (result == PLAYER_DRAW){
        player_data->num_of_draws--;
        player_data->num_of_wins++;
    }
    return PLAYER_SUCCESS;
}
*/

PlayerData playerGetDataC(Map player_map, PlayerId player_id){
    if (!player_map){
        return NULL;
    }
    PlayerData data = playerGetData(player_map, player_id);
    if (!data){
        return NULL;
    }
     return copyPlayerData(data);
}

PlayerResult playerPutData(Map player_map, PlayerId player_id, PlayerData player_data){
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    
    MapResult result = mapPut(player_map, &player_id, player_data);
    if (result == MAP_OUT_OF_MEMORY){
        return PLAYER_OUT_OF_MEMORY;
    }
    else if (result == MAP_NULL_ARGUMENT){
        return PLAYER_NULL_ARGUMENT;
    }
    
    return PLAYER_SUCCESS;
}



PlayerResult playerUpdateDuelResult(Map player_map, PlayerId first_player, PlayerId second_player, int play_time,
                                    Winner winner, UpdateMode value){
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    if (playerIdIsValid(first_player) && playerIdIsValid(second_player)){
        return PLAYER_INVALID_ID;
    }
    PlayerData first_player_data = playerGetData(player_map, first_player);
    PlayerData second_player_data = playerGetData(player_map, first_player);
    
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



PlayerResult playerChangeDuelResult(Map player_map, PlayerId first_player, PlayerId second_player,
                                    Winner old_result, Winner new_result){
    if (!player_map){
        return PLAYER_NULL_ARGUMENT;
    }
    if (playerIdIsValid(first_player) && playerIdIsValid(second_player)){
        return PLAYER_INVALID_ID;
    }
    PlayerData first_player_data = playerGetData(player_map, first_player);
    PlayerData second_player_data = playerGetData(player_map, first_player);
    
    if (!first_player_data || !second_player_data){
        return PLAYER_NOT_EXIST;
    }
    
    if (old_result == FIRST_PLAYER){
        first_player_data->num_of_wins--;
        second_player_data->num_of_loses--;
    }
    else if (old_result == SECOND_PLAYER){
        first_player_data->num_of_loses--;
        second_player_data->num_of_wins--;
    }
    else{
        first_player_data->num_of_draws--;
        second_player_data->num_of_draws--;
    }
    
    if (new_result == FIRST_PLAYER){
        first_player_data->num_of_wins++;
        second_player_data->num_of_loses++;
    }
    else if (new_result == SECOND_PLAYER){
        first_player_data->num_of_loses++;
        second_player_data->num_of_wins++;
    }
    else{
        first_player_data->num_of_draws++;
        second_player_data->num_of_draws++;
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



/**
*   The winner of the tournament is the player with the highest score:
*   player_score = ( num_of_wins * 2 + num_of_draws * 1 ) / ( num_of_games_of_player )
*   If two players have the same score, the player with least losses will be chosen.
*   If two players have the same number of losses, the player with the most wins will be chosen
*   If two players have the same number of wins and losses,
*   the player with smaller id will be chosen.
*   Once the tournament is over, no games can be added for that tournament.
*
 */

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
    
    Map player_map_copy = playerMapCopy(player_map);
    if (!player_map_copy){
        return 0;
    }
    
    PlayerKey winner_key = mapGetFirst(player_map_copy);///remember to free
    PlayerData winner_data = playerGetData(player_map_copy, *winner_key);
    int winner_score = playerCalculateScore(winner_data);
    
    MAP_FOREACH(PlayerKey, player_key, player_map_copy){
        PlayerData player_data = playerGetData(player_map_copy, *player_key);
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
            winner_data = playerGetData(player_map_copy, *winner_key);
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
        return PLAYER_OUT_OF_MEMORY;
    }
    
    if (player_data->num_of_games == 0){
        return 0;
    }
    *play_time = (double)(player_data->total_play_time)/player_data->num_of_games;
    return PLAYER_SUCCESS;
}









