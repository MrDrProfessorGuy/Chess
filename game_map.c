#include "game_map.h"
#include <assert.h>
#include <stdlib.h>

#define PLAYER_NOT_PARTICIPATES 0


typedef struct game_key* GameKey;
typedef struct game_data* GameData;


struct game_key{
    PlayerId player1_id;
    PlayerId player2_id;
};

struct game_data{
    int play_time;
    Winner winner;
    //const PlayerId player1_id;
    //const PlayerId player2_id;
};

/***************************************************************/
/********************* static functions *********************/

static void freeGameData(MapDataElement game_data);
static void freeGameKey(MapKeyElement game_key);
static MapDataElement copyGameData(MapDataElement game_data);
static MapKeyElement copyGameKey(MapKeyElement game_key);
static int compareGameKey(MapKeyElement game1_key, MapKeyElement game2_key);

/**
 * createGameKey: create a new sorted GameKey
 * assuming player id's is valid
 *
 * @param player1_id
 * @param player2_id
 * @return
 *      NULL - if allocation failed
 *      GameKey otherwise
 */
static GameKey createGameKey(PlayerId player1_id, PlayerId player2_id);
/**
 * GameData:
 * assuming play_time is valid
 * @param play_time
 * @param winner
 * @return
 *      NULL - if allocation failed
 *      GameData otherwise
 */
static GameData createGameData(int play_time, Winner winner);


//static bool gameKeyIsValid(GameKey game_key);
static bool playerIdIsValid(PlayerId player_id);
/**
 * reorderPlayers: given valid id's and play_time pointers, the id's will be reordered
 *      such that id1 < id2 and winner will be set accordingly
 * @param id1
 * @param id2
 * @param winner
 * @return
 *      true - if a reorder was made
 *      false - if the given id's were already in the correct order
 */
static bool reorderPlayers(PlayerId* id1, PlayerId* id2);
static Winner switchWinner(Winner winner);
static PlayerId playerParticipatesInGame(GameKey game_key, PlayerId player_id);
static Winner playerGameResult(GameKey game_key, GameData game_data, PlayerId first_player);

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
/*********************************************************/

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
    GameData data_copy = createGameData(((GameData)game_data)->play_time,
                                        ((GameData)game_data)->winner);
    if (!data_copy){
        return NULL;
    }
    return game_data;
}
static MapKeyElement copyGameKey(MapKeyElement game_key){
    if (!game_key){
        return NULL;
    }
    GameKey key_copy = createGameKey(((GameKey)game_key)->player1_id,
                                     ((GameKey)game_key)->player2_id);
    if (!key_copy){
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

static GameKey createGameKey(PlayerId player1_id, PlayerId player2_id){
    GameKey game_key = malloc(sizeof(*game_key));
    if (!game_key){
        return NULL;
    }
    reorderPlayers(&player1_id, &player2_id);
    game_key->player1_id = player1_id;
    game_key->player2_id = player2_id;
    return game_key;
}
static GameData createGameData(int play_time, Winner winner){
    GameData game_data = malloc(sizeof(*game_data));
    if (!game_data){
        return NULL;
    }
    game_data->play_time = play_time;
    game_data->winner = winner;
    return game_data;
}

/*
static bool gameKeyIsValid(GameKey game_key){
    assert(game_key);
    if (!playerIdIsValid(game_key->player1_id) || !playerIdIsValid(game_key->player2_id)){
        return false;
    }
    assert(game_key->player1_id < game_key->player2_id);
    
    return true;
}
 */
static bool playerIdIsValid(PlayerId player_id){
    if (player_id > 0){
        return true;
    }
    return false;
}

static bool reorderPlayers(PlayerId* id1, PlayerId* id2){
    if (*id1 > *id2){
        PlayerId tmp = *id1;
        *id1 = *id2;
        *id2 = tmp;
        
        return true;
    }
    return false;
}

static Winner switchWinner(Winner winner){
    if (winner == FIRST_PLAYER){
        return SECOND_PLAYER;
    }
    else if (winner == SECOND_PLAYER){
        return FIRST_PLAYER;
    }
    return DRAW;
}
//returns second player id
static PlayerId playerParticipatesInGame(GameKey game_key, PlayerId player_id){
    assert(game_key);
    if (game_key->player1_id == player_id){
        return game_key->player2_id;
    }
    if (game_key->player2_id == player_id){
        return game_key->player1_id;
    }
    return PLAYER_NOT_PARTICIPATES;
}

static Winner playerGameResult(GameKey game_key, GameData game_data, PlayerId first_player){
    if (first_player == game_key->player2_id){
        game_data->winner = switchWinner(game_data->winner);
    }
    return game_data->winner;
}
/*
static GameData gameGetDataPointer(Map game_map, PlayerId player1_id, PlayerId player2_id){
    GameKey game_key = createGameKey(player1_id, player2_id);
    if (!game_key){
        return NULL;
    }
    GameData game_data = mapGet(game_map, game_key);
    freeGameKey(game_key);
    
    if (!game_data){
        return NULL;
    }
    
    return game_data;
}
*/
static bool gameCreateKeyDataPair(int play_time, Winner winner, PlayerId player1_id, PlayerId player2_id,
                                  GameKey* game_key_ptr, GameData* game_data_ptr){
    if (!game_key_ptr || !game_data_ptr){
        return false;
    }
    *game_key_ptr = createGameKey(player1_id, player2_id);
    if (!*game_key_ptr){
        return false;
    }
    *game_data_ptr = createGameData(play_time, winner);
    if (!*game_data_ptr){
        freeGameKey(*game_key_ptr);
        return false;
    }
    return true;
}

/********************* Public functions *********************/

Map gameCreateMap(){
    return mapCreate(copyGameData, copyGameKey, freeGameData, freeGameKey, compareGameKey);
}

void gameDestroyMap(Map game_map){
    mapDestroy(game_map);
}

bool playTimeIsValid(int play_time){
    if (play_time >= 0){
        return true;
    }
    return false;
}

bool gameExists(Map game_map, PlayerId player1_id, PlayerId player2_id){
    assert(game_map);
    GameKey game_key = createGameKey(player1_id, player2_id);
    if (!game_key || !mapContains(game_map, game_key)){
        freeGameKey(game_key);
        return false;
    }
    freeGameKey(game_key);
    return true;
}

GameResult gameAdd(Map game_map, int play_time, Winner winner,
                   PlayerId player1_id, PlayerId player2_id){
    if (!game_map){
        return GAME_NULL_ARGUMENT;
    }
    if (!playerIdIsValid(player1_id) || !playerIdIsValid(player2_id) ||
        player1_id == player2_id){
        return GAME_INVALID_ID;
    }
    if (gameExists(game_map, player1_id, player2_id)){
        return GAME_ALREADY_EXISTS;
    }
    if (!playTimeIsValid(play_time)){
        return GAME_INVALID_PLAY_TIME;
    }
    
    if (reorderPlayers(&player1_id, &player2_id)){
        winner = switchWinner(winner);
    }
    GameKey game_key;
    GameData game_data;

    if (!gameCreateKeyDataPair(play_time, winner, player1_id, player2_id, &game_key, &game_data)){
        return GAME_OUT_OF_MEMORY;
    }
    
    if (mapPut(game_map, game_key, game_data) != MAP_SUCCESS){
        freeGameKey(game_key);
        freeGameData(game_data);
        return GAME_OUT_OF_MEMORY;
    }
    
    return GAME_SUCCESS;
}

GameResult gameRemove(Map game_map, PlayerId player1_id, PlayerId player2_id){
    if (!playerIdIsValid(player1_id) || !playerIdIsValid(player2_id) ||
        player1_id == player2_id){
        return GAME_INVALID_ID;
    }
    GameKey game_key = createGameKey(player1_id, player2_id);
    if (!game_key){
        return GAME_OUT_OF_MEMORY;
    }
    
    // game_map and game_key != NULL
    // so mapRemove will return NOT_EXIST or SUCCESS
    // we should return SUCCESS in both cases
    mapRemove(game_map, game_key);
    freeGameKey(game_key);
    return GAME_SUCCESS;
}

GameResult gameRemoveAllPlayerParticipatedGames(Map game_map, PlayerId player_id){
    assert(game_map);
    
    MAP_FOREACH(GameKey, game_key, game_map){
        if (playerParticipatesInGame(game_key, player_id)){
            if (gameRemove(game_map, game_key->player1_id, game_key->player2_id) != GAME_SUCCESS){
                return GAME_OUT_OF_MEMORY;
            }
        }
        freeGameKey(game_key);
    }
    
    return GAME_SUCCESS;
}

bool gameGetSecondPlayerId(Map game_map, PlayerId player_id, PlayerId* second_player){
    assert(game_map);
    
    MAP_FOREACH(GameKey, game_key, game_map){
        *second_player = playerParticipatesInGame(game_key, player_id);
        if (*second_player){
            freeGameKey(game_key);
            return true;
        }
    }
    return false;
}

bool gameGetDataByPlayerId(Map game_map, PlayerId first_player, PlayerId* second_player,
                                 int* winner, int* play_time, bool remove){
    assert(game_map);
    
    MAP_FOREACH(GameKey, game_key, game_map){
        *second_player = playerParticipatesInGame(game_key, first_player);
        if (*second_player != PLAYER_NOT_PARTICIPATES){
            GameData game_data = mapGet(game_map, game_key);
            assert(game_data);
            if (!game_data){
                freeGameKey(game_key);
                continue;
            }
            *winner = playerGameResult(game_key, game_data, first_player);
            *play_time = game_data->play_time;
            
            if (remove){
                gameRemove(game_map, game_key->player1_id, game_key->player2_id);
            }
            freeGameKey(game_key);
            return true;
        }
        freeGameKey(game_key);
    }
    return false;
}

int gameGetNumOfGames(Map game_map){
    if (!game_map){
        return 0;
    }
    return mapGetSize(game_map);
}


/*
GameData gameGetData(Map game_map, PlayerId player1_id, PlayerId player2_id){
    if (!game_map){
        return NULL;
    }
    GameData data = gameGetDataPointer(game_map, player1_id, player2_id);
    if (!data){
        return NULL;
    }
    return copyGameData(data);
}

GameResult gamePutData(Map player_map, GameData game_data){
    if (!player_map || !game_data){
        return GAME_NULL_ARGUMENT;
    }
    GameKey game_key = createGameKey(game_data->player1_id, game_data->player2_id);
    if (!game_key){
        return GAME_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(player_map, game_key, game_data);
    if (result == MAP_OUT_OF_MEMORY){
        freeGameKey(game_key);
        return GAME_OUT_OF_MEMORY;
    }
    else if (result == MAP_NULL_ARGUMENT){
        freeGameKey(game_key);
        return GAME_NULL_ARGUMENT;
    }
    freeGameKey(game_key);
    return GAME_SUCCESS;
}

*/





