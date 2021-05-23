#include "player_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

struct player_data{
    PlayerLevel level;
};

/******** player_Id functions ********/
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



/******** chess_player_data functions ********/
static PlayerData createChessPlayerData(){
    PlayerData player_data = malloc(sizeof(*player_data));
    if (!player_data){
        return NULL;
    }
    player_data->level = 0;
    return player_data;
}
static void freeChessPlayerData(PlayerData data){
    free(data);
}
static PlayerData copyChessPlayerData(PlayerData data){
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












