#include "player_map.h"
#include <assert.h>
#include <stdlib.h>

typedef PlayerLevel* PlayerData;
typedef PlayerId* PlayerKey;// Key = &player_id


/******** player_Key functions ********/
static PlayerKey createPlayerKey(PlayerId id);

static void freePlayerKey(PlayerKey player_key);
static PlayerKey copyPlayerKey(PlayerKey player_key);
static int comparePlayerKey(PlayerKey player1_key, PlayerKey player2_key);

static bool playerKeyIsValid(PlayerKey player_key);
static bool playerIdIsValid(PlayerId player_id);
/******** chess_player_data functions ********/
static PlayerData createPlayerData();
static void freePlayerData(PlayerData data);
static PlayerData copyPlayerData(PlayerData data);


/******** player_Key functions ********/
static void freePlayerKey(PlayerKey player_key){
    free(player_key);
}
static PlayerKey copyPlayerKey(PlayerKey player_key){
    if (!playerKeyIsValid(player_key)){
        return NULL;
    }
    PlayerKey key_copy = createPlayerKey(*player_key);//passed by value
    if (!key_copy){
        return NULL;
    }
    return key_copy;
}
static int comparePlayerKey(PlayerKey player1_key, PlayerKey player2_key){
    return (*player1_key - *player2_key);
}

bool playerKeyIsValid(PlayerKey player_key){
    if (!player_key || *player_key <= 0){
        return false;
    }
    return true;
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


/******** chess_player_data functions ********/
static PlayerData createPlayerData(){
    PlayerData player_data = malloc(sizeof(*player_data));
    if (!player_data){
        return NULL;
    }
    *player_data = 0;
    return player_data;
}
static void freePlayerData(PlayerData data){
    free(data);
}
static PlayerData copyPlayerData(PlayerData data){
    if (!data){
        return NULL;
    }
    PlayerData data_copy = createChessPlayerData();
    if (!data_copy){
        return NULL;
    }
    data_copy->level = data->level;
    return data_copy;
}












