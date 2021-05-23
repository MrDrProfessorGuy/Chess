#ifndef CHESS_PLAYER_MAP_H
#define CHESS_PLAYER_MAP_H

#include "chess_types.h"

/******** player_Id functions ********/
static PlayerId createPlayerId(int id){
    if (!playerIdIsValid(&id)){// can remove if checked everywhere else
        return NULL;
    }
    PlayerId player_id = malloc(sizeof(*player_id));
    if (!player_id){
        return NULL;
    }
    *player_id = id;
    return player_id;
}
static void freePlayerId(PlayerId id){
    free(id);
}
static PlayerId copyPlayerId(PlayerId id){
    if (!playerIdIsValid(id)){
        return NULL;
    }
    PlayerId id_copy = createPlayerId(*id);//passed by value
    if (!id_copy){
        return NULL;
    }
    return id_copy;
}
static int comparePlayerId(PlayerId id1, PlayerId id2){
    return (*id1 - *id2);
}
static bool playerIdIsValid(PlayerId id){
    if (!id || *id <= 0){
        return false;
    }
    return true;
}



/******** chess_player_data functions ********/
static ChessPlayerData createChessPlayerData(){
    ChessPlayerData chess_player_data = malloc(sizeof(*chess_player_data));
    if (!chess_player_data){
        return NULL;
    }
    chess_player_data->level = 0;
    return chess_player_data;
}
static void freeChessPlayerData(ChessPlayerData data){
    free(data);
}
static ChessPlayerData copyChessPlayerData(ChessPlayerData data){
    if (!data){
        return NULL;
    }
    ChessPlayerData data_copy = createChessPlayerData();
    if (!data_copy){
        return NULL;
    }
    data_copy->level = data->level;
    return data_copy;
}




#endif //CHESS_PLAYER_MAP_H
