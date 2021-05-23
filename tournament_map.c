#include "tournament_map.h"
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



struct tournament_data{
    Location location;
    int max_games_per_player;
    bool has_ended;
    Map game_map;
    Map player_statistics_map;
    PlayerId winner;
    //int longest_game_time; // printed in tournament statistics
    //int total_game_time; // used for tournament statistics
};



static bool tournamentIdIsValid(TournamentId id){
    if (*id > 0){
        return true;
    }
    return false;
}

