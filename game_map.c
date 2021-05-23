#include "game_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 *
 *               ################ Return Values ################
 *
 * CHESS_NULL_ARGUMENT
 * CHESS_INVALID_ID
 * CHESS_TOURNAMENT_NOT_EXIST
 * CHESS_TOURNAMENT_ENDED
 * CHESS_TOURNAMENT_ALREADY_EXISTS
 * CHESS_INVALID_LOCATION
 * CHESS_INVALID_MAX_GAMES
 * CHESS_INVALID_PLAY_TIME
 * CHESS_N0_GAMES
 * CHESS_PLAYER_NOT_EXIST
 * CHESS_EXCEEDED_GAMES
 * CHESS_SAVE_FAILURE
 * CHESS_SUCCESS
 */


struct game_id{
    PlayerId player1_id;
    PlayerId player2_id;
};

struct game_data{
    int play_time;
    Winner winner;
};

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



/***************************************************************/
/********************* Game functions *********************/
static void freeGameData(GameData data);
static void freeGameId(GameId game_id){
    free(game_id);
}
static GameData copyGameData(GameData data){
    free(data);
}
static GameId copyGameId(GameId game_id){
    if (!game_id){
        return NULL;
    }
    GameId id_copy = malloc(sizeof(*game_id));
    if (!id_copy){
        return NULL;
    }
    id_copy->player1_id = game_id->player1_id;
    id_copy->player2_id = game_id->player2_id;
    return id_copy;
}
static int compareGameId(GameId game1_id, GameId game2_id){
    assert(game1_id && game2_id);
    
    if (game1_id->player1_id == game2_id->player1_id){
        if (game1_id->player2_id == game2_id->player2_id){
            return 0;
        }
        return (game1_id->player2_id - game2_id->player2_id);
    }
    return (game1_id->player1_id - game2_id->player1_id);
}

static GameId createGameId(PlayerId player1_id, PlayerId player2_id, ChessResult* result){
    *result = CHESS_SUCCESS;
    if (!playerIdIsValid(player1_id) || !playerIdIsValid(player2_id)){
        *result = CHESS_INVALID_ID;
        return NULL;
    }
    GameId game_id = malloc(sizeof(*game_id));
    if (!game_id){
        *result = CHESS_OUT_OF_MEMORY;
        return NULL;
    }
    
    //orderPlayerIds(&player1_id, &player2_id);
    game_id->player1_id = player1_id;
    game_id->player2_id = player2_id;
    return game_id;
}
static GameData createGameData(int play_time, Winner winner, ChessResult* result){
    *result = CHESS_SUCCESS;
    if (!validPlayTime(play_time)){
        *result = CHESS_INVALID_PLAY_TIME;
        return NULL;
    }
    
    GameData game_data = malloc(sizeof(*game_data));
    if (!game_data){
        *result = CHESS_OUT_OF_MEMORY;
        return NULL;
    }
    game_data->play_time = play_time;
    game_data->winner = winner;
    return game_data;
}
static ChessResult addGame(Map game_map, int play_time, Winner winner,
                           PlayerId player1_id, PlayerId player2_id){
    if (!game_map){
        return CHESS_NULL_ARGUMENT;
    }
}

static bool gameIdIsValid(GameId game_id){
    assert(game_id);
    if (!playerIdIsValid(game_id->player1_id) || !playerIdIsValid(game_id->player2_id)){
        return false;
    }
    
    return true;
}


static void orderPlayerIds(PlayerId* id1, PlayerId* id2){
    if (*id1 > *id2){
        PlayerId tmp = *id1;
        *id1 = *id2;
        *id2 = tmp;
    }
}





