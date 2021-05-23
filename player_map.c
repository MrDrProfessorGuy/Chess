#include "player_map.h"
#include <assert.h>
#include <stdlib.h>

typedef PlayerId* PlayerKey;// Key = &player_id


/******** player_Key functions ********/
static MapKeyElement copyPlayerKey(MapKeyElement player_key);
static MapDataElement copyPlayerData(MapDataElement data);
static void freePlayerKey(MapKeyElement player_key);
static void freePlayerData(MapDataElement data);
static int comparePlayerKey(MapKeyElement player1_key, MapKeyElement player2_key);


static PlayerKey createPlayerKey(PlayerId id);
static PlayerData createPlayerData();

static bool playerKeyIsValid(PlayerKey player_key);



/******** player_Key functions ********/
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


static bool playerKeyIsValid(PlayerKey player_key){
    if (!player_key || *player_key <= 0){
        return false;
    }
    return true;
}
bool playerIdIsValid(PlayerId player_id){
    if (player_id > 0){
        return true;
    }
    return false;
}

/******** chess_player_data functions ********/
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



Map playerCreateMap(){
    return mapCreate(copyPlayerData, copyPlayerKey, freePlayerData,
                     freePlayerKey, comparePlayerKey);
}


void playerDestroyMap(Map player_map){
    mapDestroy(player_map);
}







