#include "game_map.h"
#include <assert.h>
#include <stdlib.h>

#define PLAYER_NOT_PARTICIPATES 0

typedef struct game_key* GameKey;

struct game_key{
    PlayerId player1_id;
    PlayerId player2_id;
};

struct game_data{
    int play_time;
    Winner winner;
};

/***************************************************************/
/********************* static functions *********************/
/* documentaion below */
static void freeGameData(MapDataElement game_data);
static void freeGameKey(MapKeyElement game_key);
static MapDataElement copyGameData(MapDataElement game_data);
static MapKeyElement copyGameKey(MapKeyElement game_key);
static int compareGameKey(MapKeyElement game1_key, MapKeyElement game2_key);

static GameKey createGameKey(PlayerId player1_id, PlayerId player2_id);
static GameData createGameData(int play_time, Winner winner);
static bool playerIdIsValid(PlayerId player_id);
static bool reorderPlayers(PlayerId* id1, PlayerId* id2);
static Winner switchWinner(Winner winner);
static PlayerId playerParticipatesInGame(GameKey game_key, PlayerId player_id);
static Winner playerGameResult(GameKey game_key, GameData game_data, PlayerId first_player);




/***********************************************************/
/********************* Game functions *********************/
/*********************************************************/
/**
* freeGameData: Deallocates an existing data. Clears all elements by using the
* stored free functions.
*
* @param game_data - Target data to be deallocated. If game_data is NULL nothing will be
* 		done
*/
static void freeGameData(MapDataElement game_data){
    free(game_data);
}
/**
* freeGameKey: Deallocates an existing key. Clears all elements by using the
* stored free functions.
*
* @param game_key - Target key to be deallocated. If game_key is NULL nothing will be
* 		done
*/
static void freeGameKey(MapKeyElement game_key){
    free(game_key);
}
/**
*copyGameData: Creates a copy of target data.
*
* @param game_data - Target data.
* @return
*NULL if a NULL was sent or a memory allocation failed.
* A MapDataElement containing the same elements as game_data.
*/
static MapDataElement copyGameData(MapDataElement game_data){
    if (!game_data){
        return NULL;
    }
    GameData data_copy = createGameData(((GameData)game_data)->play_time,
                                        ((GameData)game_data)->winner);
    if (!data_copy){
        return NULL;
    }
    return data_copy;
}
/**
*copyGameKey: Creates a copy of target key.
*
* @param game_key - Target key.
* @return
*NULL if a NULL was sent or a memory allocation failed.
* A MapKeyElement containing the same elements as player_key.
*/
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
/**
    * compareGameKey: identifies equal key elements.
    *
    *@param game1_key - first key to be compared.
    *@param game2_key - second key to be compared.
    *
    * @return-
    * 		A positive integer if the first key is greater;
    * 		0 if they're equal;
    *		A negative integer if the second key is greater.
*/
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
/**
* createGameKey: Allocates a new key.
*
* @param player1_id1 - the first Player's id to be set in the key.
* @param player1_id2 - the second Player's id to be set in the key.
*
* @return
* 	NULL - if allocations failed.
* 	A new GameKey in case of success.
*/
static GameKey createGameKey(PlayerId player1_id, PlayerId player2_id){
    reorderPlayers(&player1_id, &player2_id);
    GameKey game_key = malloc(sizeof(*game_key));
    if (!game_key){
        return NULL;
    }
    game_key->player1_id = player1_id;
    game_key->player2_id = player2_id;
    return game_key;
}
/**
* createGameData: Allocates a new empty Player data.
*
* @param play_time - play time.
* @param winner - the default winnet of the data.
* @return
* 	NULL - if allocations failed.
* 	A new GameData in case of success.
*/
static GameData createGameData(int play_time, Winner winner){
    if (!playTimeIsValid(play_time)){
        return NULL;
    }
    GameData game_data = malloc(sizeof(*game_data));
    if (!game_data){
        return NULL;
    }
    game_data->play_time = play_time;
    game_data->winner = winner;
    return game_data;
}
/**
    * playerIdIsValid: Checks if a player_id is valid.
    *
    * @param player_key - the player id to check if it's valid.
    *
    * @return
    * 	false - if id is a smaller than or equl to 0.
    * 	true - if id is a positive int bigger than 0.
*/
static bool playerIdIsValid(PlayerId player_id){
    if (player_id > 0){
        return true;
    }
    return false;
}
/**
    * reorderPlayers: reorder two players.
                     given valid id's and play_time pointers, the id's will be reordered
                     such that id1 < id2 and winner will be set accordingly
    *
    * @param id1 - a pointer to store the id2 in- must not be NULL.
    * @param id2 - a pointer to store the id1 in- must not be NULL.
    *
    * @return
    * 	true - if the exchanging values succeeds.
    *  false - if the given id's were already in the correct order.
*/
static bool reorderPlayers(PlayerId* id1, PlayerId* id2){
    if (*id1 > *id2){
        PlayerId tmp = *id1;
        *id1 = *id2;
        *id2 = tmp;
        
        return true;
    }
    return false;
}
/**
    * switchWinner: switches the winner.
    *
    * @param winner - a pointer to store the new wineer in. in- must not be NULL.
*/
static Winner switchWinner(Winner winner){
    if (winner == FIRST_PLAYER){
        return SECOND_PLAYER;
    }
    else if (winner == SECOND_PLAYER){
        return FIRST_PLAYER;
    }
    return DRAW;
}
/**
    * playerParticipatesInGame: returns the second player id.
    *
    * @param game_key - The game_key to look into- must not be NULL.
    * @param player_id - the first player id.
    *
    * @return
    *   0 - if the player_id does not exist in game_key.
    * 	PlayerId - the id if the second player in game_key otherwise.
*
*/
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
/**
    * playerGameResult: returns the winner.
    *
    * @param game_key - The game_key to look into.
    * @param game_data - the game_data to get it's winner from.
    * @param first_player - the first player id.
    *
    * @return
    * 	Winner - the winner.
    *
*/
static Winner playerGameResult(GameKey game_key, GameData game_data, PlayerId first_player){
    assert(!game_key || !game_data);
    if (first_player == game_key->player2_id){
        game_data->winner = switchWinner(game_data->winner);
    }
    return game_data->winner;
}

/********************* Public functions *********************/

Map gameCreateMap(){
    return mapCreate(copyGameData, copyGameKey, freeGameData, freeGameKey, compareGameKey);
}

void gameDestroyMap(Map game_map){
    mapDestroy(game_map);
}

Map gameMapCopy(Map game_map) {
    assert(game_map);
    return mapCopy(game_map);
}

bool playTimeIsValid(int play_time){
    if (play_time >= 0){
        return true;
    }
    return false;
}

bool gameExists(Map game_map, PlayerId player1_id, PlayerId player2_id){
    assert(game_map);
    if (!playerIdIsValid(player1_id) || !playerIdIsValid(player2_id)){
        return false;
    }
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
    GameKey game_key = createGameKey(player1_id, player2_id);
    if (!game_key){
        return GAME_OUT_OF_MEMORY;
    }
    GameData game_data = createGameData(play_time, winner);
    if (!game_data){
        freeGameKey(game_key);
        return GAME_OUT_OF_MEMORY;
    }
    
    if (mapPut(game_map, game_key, game_data) != MAP_SUCCESS){
        freeGameKey(game_key);
        freeGameData(game_data);
        return GAME_OUT_OF_MEMORY;
    }
    freeGameKey(game_key);
    freeGameData(game_data);
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




