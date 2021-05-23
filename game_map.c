#include "game_map.h"
#include <assert.h>
#include <stdlib.h>


typedef struct game_key* GameKey;
typedef struct game_data* GameData;

/***************************************************************/
/********************* Game functions *********************/
static void freeGameData(MapDataElement game_data);
static void freeGameKey(MapKeyElement game_key);
static MapDataElement copyGameData(MapDataElement game_data);
static MapKeyElement copyGameKey(MapKeyElement game_key);
static int compareGameKey(MapKeyElement game1_key, MapKeyElement game2_key);

static GameKey createGameKey(PlayerId player1_id, PlayerId player2_id, GameResult* result);
static GameData createGameData(int play_time, Winner winner, GameResult* result);


static bool gameKeyIsValid(GameKey game_key);
static bool playerIdIsValid(PlayerId player_id);
static bool reorderPlayers(PlayerId* id1, PlayerId* id2, Winner* winner);


struct game_key{
    PlayerId player1_id;
    PlayerId player2_id;
};

struct game_data{
    int play_time;
    Winner winner;
};
/*
static int countDigits(int num){
    int count = 0;
    do{
        num = num / 10;
        count++;
    }while (num != 0);
    return count;
}
static char* playersIdToGameId(int player1_id, int player2_id){
    int player1_digits = countDigits(player1_id);
    int player2_digits = countDigits(player2_id);
    // 1 for null terminator and 1 for PLAYER_SEPARATOR (a.k.a '#')
    char* str = malloc(sizeof(char) * (player1_digits + player2_digits + 2));
    if (str == NULL){
        return NULL;
    }
    
    
    int index = player1_digits - 1;
    do{
        str[index] = (char)((player1_id % 10) + '0');
        player1_id = player1_id / 10;
        index--;
    }while (index >= 0);
    str[player1_digits] = PLAYER_SEPARATOR;
    index = player1_digits + player2_digits + 1;
    str[index--] = '\n';
    do{
        str[index] = (char)((player2_id % 10) + '0');
        player2_id = player2_id / 10;
        index--;
    }while (index > player1_digits);
    
    return str;
}
*/


/***********************************************************/
/********************* Game functions *********************/
static void freeGameData(MapDataElement game_data){
    free(game_data);
}
static void freeGameKey(MapKeyElement game_key){
    free(game_key);
}
static MapDataElement copyGameData(MapDataElement game_data){
    if (!game_data){
        return NULL;
    }
    GameResult result;
    GameData data_copy = createGameData(((GameData)game_data)->play_time,
                                        ((GameData)game_data)->winner, &result);
    if (!data_copy){
        assert(result != GAME_SUCCESS);
        return NULL;
    }
    return game_data;
}
static MapKeyElement copyGameKey(MapKeyElement game_key){
    if (!game_key){
        return NULL;
    }
    GameResult result;
    GameKey key_copy = createGameKey(((GameKey)game_key)->player1_id,
                                     ((GameKey)game_key)->player2_id, &result);
    if (!key_copy){
        assert(result != GAME_SUCCESS);
        return NULL;
    }
    
    return key_copy;
}
static int compareGameKey(MapKeyElement game1_key, MapKeyElement game2_key){
    assert(game1_key && game2_key);
    
    if (((GameKey)game1_key)->player1_id == ((GameKey)game2_key)->player1_id){
        if (((GameKey)game1_key)->player2_id == ((GameKey)game2_key)->player2_id){
            return 0;
        }
        return (((GameKey)game1_key)->player2_id - ((GameKey)game2_key)->player2_id);
    }
    return (((GameKey)game1_key)->player1_id - ((GameKey)game2_key)->player1_id);
}

static GameKey createGameKey(PlayerId player1_id, PlayerId player2_id, GameResult* result){
    *result = GAME_SUCCESS;
    if (!playerIdIsValid(player1_id) || !playerIdIsValid(player2_id)){
        *result = GAME_INVALID_ID;
        return NULL;
    }
    GameKey game_key = malloc(sizeof(*game_key));
    if (!game_key){
        *result = GAME_OUT_OF_MEMORY;
        return NULL;
    }
    
    game_key->player1_id = player1_id;
    game_key->player2_id = player2_id;
    return game_key;
}
static GameData createGameData(int play_time, Winner winner, GameResult* result){
    *result = GAME_SUCCESS;
    if (!playTimeIsValid(play_time)){
        *result = GAME_INVALID_PLAY_TIME;
        return NULL;
    }
    
    GameData game_data = malloc(sizeof(*game_data));
    if (!game_data){
        *result = GAME_OUT_OF_MEMORY;
        return NULL;
    }
    game_data->play_time = play_time;
    game_data->winner = winner;
    return game_data;
}


static bool gameKeyIsValid(GameKey game_key){
    assert(game_key);
    if (!playerIdIsValid(game_key->player1_id) || !playerIdIsValid(game_key->player2_id)){
        return false;
    }
    assert(game_key->player1_id < game_key->player2_id);
    
    return true;
}
bool playTimeIsValid(int play_time){
    if (play_time >= 0){
        return true;
    }
    return false;
}
static bool playerIdIsValid(PlayerId player_id){
    if (player_id > 0){
        return true;
    }
    return false;
}

static bool reorderPlayers(PlayerId* id1, PlayerId* id2, Winner* winner){
    if (*id1 > *id2){
        PlayerId tmp = *id1;
        *id1 = *id2;
        *id2 = tmp;
    
        if (*winner == FIRST_PLAYER){
            *winner = SECOND_PLAYER;
        }
        else if (*winner == SECOND_PLAYER){
            *winner = FIRST_PLAYER;
        }
        return true;
    }
    return false;
}


GameResult gameAdd(Map game_map, int play_time, Winner winner,
                   PlayerId player1_id, PlayerId player2_id){
    GameResult result = GAME_SUCCESS;
    if (gameExists(game_map, player1_id, player2_id)){
        return GAME_ALREADY_EXISTS;
    }
    if (!playTimeIsValid(play_time)){
        return GAME_INVALID_PLAY_TIME;
    }
    
    reorderPlayers(&player1_id, &player2_id, &winner);
    GameKey game_key = createGameKey(player1_id, player2_id, &result);
    if (result != GAME_SUCCESS){
        return result;
    }
    GameData game_data = createGameData(play_time, winner, &result);
    if (result != GAME_SUCCESS){
        freeGameKey(game_key);
        return result;
    }
    
    if (mapPut(game_map, game_key, game_data) != MAP_SUCCESS){
        freeGameKey(game_key);
        freeGameData(game_data);
        return GAME_OUT_OF_MEMORY;
    }
    assert(result == GAME_SUCCESS);
    return result;
}

bool gameExists(Map game_map, PlayerId player1_id, PlayerId player2_id){
    assert(game_map);
    GameResult result;
    GameKey game_key = createGameKey(player1_id, player2_id, *result);
    if (mapContains(game_map, game_key)){
        freeGameKey(game_key);
        return true;
    }
    freeGameKey(game_key);
    return false;
}

