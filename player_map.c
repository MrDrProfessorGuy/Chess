#include "player_map.h"
#include <assert.h>
#include <stdlib.h>

typedef PlayerId* PlayerKey;// Key = &player_id


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
    assert(player_data);
    if (player_data->num_of_games < num_of_games){
        return false;
    }
    return true;
}

PlayerResult playerAdd(Map player_map, PlayerId player_id){
    assert(player_map);
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


Map playerGetMapCopy(Map player_map){
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
    if (remove){
        playerRemove(player_map, max_id);
    }
    return max_id;
}

void playerUpdateData(Map player_map, PlayerId first_player, DuelResult result){
    assert(player_map);
    assert(playerIdIsValid(first_player));
    
    PlayerData second_player_data = playerGetData(player_map, first_player);
    if (result == PLAYER_LOST){
        second_player_data->num_of_loses--;
        second_player_data->num_of_wins++;
    }
    else if (result == PLAYER_DRAW){
        second_player_data->num_of_draws--;
        second_player_data->num_of_wins++;
    }
}









