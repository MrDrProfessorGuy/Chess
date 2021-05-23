#ifndef CHESS_PLAYER_MAP_H
#define CHESS_PLAYER_MAP_H

#include "chess_types.h"

/******** player_Id functions ********/
static PlayerKey createPlayerKey(PlayerId id);

static void freePlayerKey(PlayerKey player_key);
static PlayerKey copyPlayerKey(PlayerKey player_key);
static int comparePlayerKey(PlayerKey player1_key, PlayerKey player2_key);

static bool playerKeyIsValid(PlayerKey player_key);


/******** chess_player_data functions ********/
static PlayerData createChessPlayerData();
static void freeChessPlayerData(PlayerData data);
static PlayerData copyChessPlayerData(PlayerData data);




#endif //CHESS_PLAYER_MAP_H
