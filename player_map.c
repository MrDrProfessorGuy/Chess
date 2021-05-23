#include "player_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

/********************* Player functions *********************/
static ChessResult chessSystemAddPlayer(Map player_map, PlayerId player_id);

/******** player_Id functions ********/
static PlayerId createPlayerId(int id);
static void freePlayerId(PlayerId id);
static PlayerId copyPlayerId(PlayerId id);
static int comparePlayerId(PlayerId id1, PlayerId id2);
static bool playerIdIsValid(PlayerId id);



struct chess_player_data{
    PlayerLevel level;
};





