
#ifndef CHESS_GAME_MAP_H
#define CHESS_GAME_MAP_H

#define PLAYER_SEPARATOR '#'
#include "chess_types.h"



/***************************************************************/
/********************* Game functions *********************/
static void freeGameData(GameData data);
static void freeGameId(GameId id);
static GameData copyGameData(GameData data);
static GameId copyGameId(GameId id);
static int compareGameId(GameId id1, GameId id2);

static GameId createGameId(PlayerId player1_id, PlayerId player2_id, ChessResult* result);
static GameData createGameData(int play_time, Winner winner, ChessResult* result);
static ChessResult addGame(Map game_map, int play_time, Winner winner,
                           PlayerId player1_id, PlayerId player2_id);

static bool gameIdIsValid(GameId game_id);
static void orderPlayerIds(PlayerId* id1, PlayerId* id2);





#endif //CHESS_GAME_MAP_H
