#include "chessSystem.h"
#include "game_map.h"
#include "player_map.h"
#include "tournament_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/**
 *   ################ Return Values ################
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

struct chess_system_t{
    Map tournament_map;
    Map player_map;
};



/***************************************************************/
/********************* Helper Functions *********************/
/***************************************************************/




/*************************************************************/
/********************* Static Functions *********************/
/***********************************************************/
static ChessResult chessResultToTournamentResult(TournamentResult result){
    if (result == TOURNAMENT_OUT_OF_MEMORY){
        return CHESS_OUT_OF_MEMORY;
    }
    else if (result == TOURNAMENT_NULL_ARGUMENT){
        return CHESS_NULL_ARGUMENT;
    }
    else if (result == TOURNAMENT_INVALID_ID){
        return CHESS_INVALID_ID;
    }
    else if (result == TOURNAMENT_INVALID_LOCATION){
        return CHESS_INVALID_LOCATION;
    }
    else if (result == TOURNAMENT_INVALID_MAX_GAMES){
        return CHESS_INVALID_MAX_GAMES;
    }
    else if (result == TOURNAMENT_TOURNAMENT_ALREADY_EXISTS){
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    else if (result == TOURNAMENT_TOURNAMENT_NOT_EXIST){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    else if (result == TOURNAMENT_GAME_ALREADY_EXISTS){
        return CHESS_GAME_ALREADY_EXISTS;
    }
    else if (result == TOURNAMENT_INVALID_PLAY_TIME){
        return CHESS_INVALID_PLAY_TIME;
    }
    else if (result == TOURNAMENT_EXCEEDED_GAMES){
        return CHESS_EXCEEDED_GAMES;
    }
    else if (result == TOURNAMENT_PLAYER_NOT_EXIST){
        return CHESS_PLAYER_NOT_EXIST;
    }
    else if (result == TOURNAMENT_TOURNAMENT_ENDED){
        return CHESS_TOURNAMENT_ENDED;
    }
    else if (result == TOURNAMENT_NO_TOURNAMENTS_ENDED){
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    else if (result == TOURNAMENT_NO_GAMES){
        return CHESS_NO_GAMES;
    }
    else if (result == TOURNAMENT_SAVE_FAILURE){
        return CHESS_SAVE_FAILURE;
    }
    assert(result == TOURNAMENT_SUCCESS);
    return CHESS_SUCCESS;
}

static ChessResult chessResultToPlayerResult(PlayerResult result){
    if (result == PLAYER_OUT_OF_MEMORY){
        return CHESS_OUT_OF_MEMORY;
    }
    else if (result == PLAYER_NULL_ARGUMENT){
        return CHESS_NULL_ARGUMENT;
    }
    else if (result == PLAYER_INVALID_ID){
        return CHESS_INVALID_ID;
    }
    else if (result == PLAYER_INVALID_LOCATION){
        return CHESS_INVALID_LOCATION;
    }
    else if (result == PLAYER_INVALID_MAX_GAMES){
        return CHESS_INVALID_MAX_GAMES;
    }
    else if (result == PLAYER_TOURNAMENT_ALREADY_EXISTS){
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    else if (result == PLAYER_TOURNAMENT_NOT_EXIST){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    else if (result == PLAYER_GAME_ALREADY_EXISTS){
        return CHESS_GAME_ALREADY_EXISTS;
    }
    else if (result == PLAYER_INVALID_PLAY_TIME){
        return CHESS_INVALID_PLAY_TIME;
    }
    else if (result == PLAYER_EXCEEDED_GAMES){
        return CHESS_EXCEEDED_GAMES;
    }
    else if (result == PLAYER_NOT_EXIST){
        return CHESS_PLAYER_NOT_EXIST;
    }
    else if (result == PLAYER_TOURNAMENT_ENDED){
        return CHESS_TOURNAMENT_ENDED;
    }
    else if (result == PLAYER_NO_TOURNAMENTS_ENDED){
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    else if (result == PLAYER_NO_GAMES){
        return CHESS_NO_GAMES;
    }
    else if (result == PLAYER_SAVE_FAILURE){
        return CHESS_SAVE_FAILURE;
    }
    assert(result == PLAYER_SUCCESS);
    return CHESS_SUCCESS;
}



static ChessResult checkValidityForAddGame(ChessSystem chess, int tournament_id, int first_player,
                                           int second_player, int play_time){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id) || !playerIdIsValid(first_player) ||
        !playerIdIsValid(second_player) || first_player == second_player){
        return CHESS_INVALID_ID;
    }
    Map tournament_map = chess->tournament_map;
    assert(tournament_map);
    Map tournament_player_map = tournamentGetPlayerMap(tournament_map, tournament_id);
    Map tournament_game_map = tournamentGetGameMap(tournament_map, tournament_id);
    
    if(!tournament_game_map){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (tournamentEnded(tournament_map, tournament_id)){
        return CHESS_TOURNAMENT_ENDED;
    }
    
    if (gameExists(tournament_game_map, first_player, second_player)){
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if (!playTimeIsValid(play_time)){
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games = 0;
    tournamentGetMaxGamesPerPlayer(tournament_map, tournament_id, &max_games);
    if (playerExceededGames(tournament_player_map, first_player, max_games) ||
        playerExceededGames(tournament_player_map, second_player, max_games)){
        
        return CHESS_EXCEEDED_GAMES;
    }
    
    return CHESS_SUCCESS;
}


/*
 *

**
 * Update 2 competing players data based on the game result .aka winner
 * @param player_map
 * @param player1_id
 * @param player2_id
 * @param winner
 *
bool updatePlayersData(Map player_map, PlayerId player1_id, PlayerId player2_id,
                              Winner winner, int play_time, UpdateMode value){
    assert(player_map);
    assert(playerIdIsValid(player1_id) && playerIdIsValid(player2_id));
    
    PlayerData player1_data = playerGetData(player_map, player1_id);
    PlayerData player2_data = playerGetData(player_map, player2_id);
    if (player1_data && player2_data){
        player1_data->num_of_games += value;
        player2_data->num_of_games += value;
        player1_data->total_play_time += value*play_time;
        player2_data->total_play_time += value*play_time;
        if (winner == FIRST_PLAYER){
            player1_data->num_of_wins += value;
            player2_data->num_of_loses += value;
        }
        else if(winner == SECOND_PLAYER){
            player1_data->num_of_loses += value;
            player2_data->num_of_wins += value;
        }
        else{
            player1_data->num_of_draws += value;
            player2_data->num_of_draws += value;
        }
        return true;
    }
    return false;
}

*/
static ChessResult addAndUpdatePlayersData(Map player_map,PlayerId player1_id,
                                           PlayerId player2_id, Winner winner, int play_time){
    assert(player_map);
    assert(playerIdIsValid(player1_id) && playerIdIsValid(player2_id));
    
    if (playerAdd(player_map, player1_id) != PLAYER_SUCCESS){
        return CHESS_OUT_OF_MEMORY;
    }
    if (playerAdd(player_map, player2_id) != PLAYER_SUCCESS){
        playerRemove(player_map, player1_id);
        return CHESS_OUT_OF_MEMORY;
    }
    
    playerUpdateDuelResult(player_map, player1_id, player2_id, play_time, winner, ADD);
    return CHESS_SUCCESS;
}

/*************************************************************/
/********************* Public Functions *********************/
/***********************************************************/




/**
 * chessCreate: create an empty chess system.
 *
 * @return A new chess system in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
ChessSystem chessCreate(){
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    if (!chess_system){
        return NULL;
    }
    
    chess_system->player_map = playerCreateMap();
    if (!chess_system->player_map){
        chessDestroy(chess_system);
        return NULL;
    }
    chess_system->tournament_map = tournamentCreate();
    if (!chess_system->tournament_map){
        chessDestroy(chess_system);
        return NULL;
    }
    
    return chess_system;
}

/**
 * chessDestroy: free a chess system, and all its contents, from
 * memory.
 *
 * @param chess - the chess system to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void chessDestroy(ChessSystem chess){
    if (!chess){
        return;
    }
    playerDestroyMap(chess->player_map);
    tournamentDestroy(chess->tournament_map);
    free(chess);
}

/**
 * chessAddTournament: add a new tournament to a chess system.
 *
 * @param chess - chess system to add the tournament to. Must be non-NULL.
 * @param tournament_id - new tournament id. Must be positive, and unique.
 * @param max_games_per_player - maximum number of games a player is allow to play in the specified tournament.
 *                               Must be postivie/
 * @param tournament_location - location in which the tournament take place. Must be non-empty.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess/tournament_location are NULL.
 *     CHESS_INVALID_ID - the tournament ID number is invalid.
 *     CHESS_TOURNAMENT_ALREADY_EXISTS - if a tournament with the given id already exist.
 *     CHESS_INVALID_LOCATION - if the name is empty or doesn't start with a capital letter (A -Z)
 *                      followed by small letters (a -z) and spaces (' ').
 *     CHESS_INVALID_MAX_GAMES - if the maximum number of games allowed is not positive
 *     CHESS_SUCCESS - if tournament was added successfully.
 */
ChessResult chessAddTournament (ChessSystem chess, int tournament_id,
                                int max_games_per_player, const char* tournament_location){
    
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)){
        return CHESS_INVALID_ID;
    }
    if (!tournamentLocationIsValid(tournament_location)){
        return CHESS_INVALID_LOCATION;
    }
    if (max_games_per_player <= 0){
        return CHESS_INVALID_MAX_GAMES;
    }
    
    TournamentResult result = tournamentAdd(chess->tournament_map,
                                            tournament_id, max_games_per_player, tournament_location);
    
    return chessResultToTournamentResult(result);
    
}




/**
 * chessAddGame: add a new match to a chess tournament.
 *
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @param tournament_id - the tournament id. Must be positive, and unique.
 * @param first_player - first player id. Must be positive.
 * @param second_player - second player id. Must be positive.
 * @param winner - indicates the winner in the match. if it is FIRST_PLAYER, then the first player won.
 *                 if it is SECOND_PLAYER, then the second player won, otherwise the match has ended with a draw.
 * @param play_time - duration of the match in seconds. Must be non-negative.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the tournament ID number, either the players or the winner is invalid or both players
 *                        have the same ID number.
 *     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
 *     CHESS_TOURNAMENT_ENDED - if the tournament already ended
 *     CHESS_GAME_ALREADY_EXISTS - if there is already a game in the tournament with the same two players
 *                                  (both were not removed).
 *     CHESS_INVALID_PLAY_TIME - if the play time is negative.
 *     CHESS_EXCEEDED_GAMES - if one of the players played the maximum number of games allowed
 *     CHESS_SUCCESS - if game was added successfully.
 */
ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time){
    ChessResult validity_check = checkValidityForAddGame(chess, tournament_id, first_player, second_player,
                                                       play_time);
    if (validity_check != CHESS_SUCCESS){
        return validity_check;
    }
    //no checks from here since all the conditions were already checked
    Map tournament_map = chess->tournament_map;
    Map tournament_player_map = tournamentGetPlayerMap(tournament_map, tournament_id);
    Map tournament_game_map = tournamentGetGameMap(chess->tournament_map, tournament_id);
    
    
    GameResult result = gameAdd(tournament_game_map, play_time, winner, first_player, second_player);
    if (result != GAME_SUCCESS){
        return CHESS_OUT_OF_MEMORY;
    }
    int new_players = 0;
    new_players += playerExists(tournament_player_map, first_player);
    new_players += playerExists(tournament_player_map, second_player);
    
    if (addAndUpdatePlayersData(chess->player_map, first_player, second_player, winner, play_time) != CHESS_SUCCESS){
        gameRemove(tournament_game_map, first_player, second_player);
        return CHESS_OUT_OF_MEMORY;
    }
    if (addAndUpdatePlayersData(tournament_player_map, first_player, second_player, winner, play_time)
        != CHESS_SUCCESS){
        playerUpdateDuelResult(chess->player_map, first_player, second_player, play_time, winner, UNDO);
        gameRemove(tournament_game_map, first_player, second_player);
        return CHESS_OUT_OF_MEMORY;
    }
    tournamentUpdateStatistics(tournament_map,tournament_id, play_time, new_players);
    return CHESS_SUCCESS;
}
/*
ChessResult chessAddGame2(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    
    // if player exists nothing happens
    if (playerAdd(chess->player_map, first_player) != PLAYER_SUCCESS){
        return CHESS_OUT_OF_MEMORY;
    }
    if (playerAdd(chess->player_map, second_player) != PLAYER_SUCCESS){
        return CHESS_OUT_OF_MEMORY;
    }
    
    playerUpdateDuelResult(chess->player_map, first_player, second_player, play_time, winner, ADD);
    
    
    return CHESS_SUCCESS;
}

 */
 
/**
 * chessRemoveTournament: removes the tournament and all the games played in it from the chess system
 *                        updates all players statistics (wins, losses, draws, average play time).
 *
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @param tournament_id - the tournament id. Must be positive, and unique.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the tournament ID number is invalid.
 *     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
 *     CHESS_SUCCESS - if tournament was removed successfully.
 */
ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)){
        return CHESS_INVALID_ID;
    }
    if (tournamentContains(chess->tournament_map, tournament_id) == false){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Map tournament_player_map = tournamentGetPlayerMap(chess->tournament_map, tournament_id);
    if (!tournament_player_map){
        return CHESS_OUT_OF_MEMORY;
    }
    assert(chess->player_map);
    PlayerResult player_result = playerMapUpdateStatistics(chess->player_map, tournament_player_map, false, UNDO);
    assert(player_result == PLAYER_SUCCESS);

    TournamentResult tournament_result = tournamentRemove(chess->tournament_map, tournament_id);
    return chessResultToTournamentResult(tournament_result);
}

/**
 * chessRemovePlayer: removes the player from the chess system.
 *                      In games where the player has participated and not yet ended,
 *                      the opponent is the winner automatically after removal.
 *                      If both player of a game were removed, the game still exists in the system.
 *
 * @param chess - chess system that contains the player. Must be non-NULL.
 * @param player_id - the player id. Must be positive.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not exist in the system.
 *     CHESS_SUCCESS - if player was removed successfully.
 */
ChessResult chessRemovePlayer(ChessSystem chess, int player_id){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    if (!playerIdIsValid(player_id)){
        return CHESS_INVALID_ID;
    }
    if (!playerExists(chess->player_map, player_id)){
        return CHESS_PLAYER_NOT_EXIST;
    }
    
    ///iterate over all tournaments that have not ended
    /// and remove the player from the map and all games he played in
    tournamentPlayerRemove(chess->tournament_map, chess->player_map, player_id);
    return CHESS_SUCCESS;
}

/**
 * chessEndTournament: The function will end the tournament if it has at least one game and
 *                     calculate the id of the winner.
 *                     The winner of the tournament is the player with the highest score:
 *                     player_score = ( num_of_wins * 2 + num_of_draws * 1 ) / ( num_of_games_of_player )
 *                     If two players have the same score, the player with least losses will be chosen.
 *                     If two players have the same number of losses, the player with the most wins will be chosen
 *                     If two players have the same number of wins and losses,
 *                     the player with smaller id will be chosen.
 *                     Once the tournament is over, no games can be added for that tournament.
 *
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @param tournament_id - the tournament id. Must be positive, and unique.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the tournament ID number is invalid.
 *     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
 *     CHESS_TOURNAMENT_ENDED - if the tournament already ended
 *     CHESS_N0_GAMES - if the tournament does not have any games.
 *     CHESS_SUCCESS - if tournament was ended successfully.
 */
ChessResult chessEndTournament (ChessSystem chess, int tournament_id){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)){
        return CHESS_INVALID_ID;
    }
    if (!tournamentContains(chess->tournament_map, tournament_id)){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (tournamentEnded(chess->tournament_map, tournament_id)){
        return CHESS_TOURNAMENT_ENDED;
    }
    if (tournamentNumOfActiveGames(chess->tournament_map, tournament_id) <= 0){
        return CHESS_NO_GAMES;
    }

    TournamentResult tournament_result = tournamentEnd(chess->tournament_map, tournament_id);
    return chessResultToTournamentResult(tournament_result);
}

/**
 * chessCalculateAveragePlayTime: the function returns the average playing time for a particular player
 *
 * @param chess - a chess system that contains the player. Must be non-NULL.
 * @param player_id - player ID. Must be positive.
 * @param chess_result - this variable will contain the returned error code.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not exist in the system.
 *     CHESS_SUCCESS - if average playing time was returned successfully.
 */
double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result){
    if (!chess){
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }
    if (!playerIdIsValid(player_id)){
        *chess_result =  CHESS_INVALID_ID;
        return 0;
    }
    if (!playerExists(chess->player_map, player_id)){
        *chess_result =  CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    
    double average_play_time = 0;
    PlayerResult player_result = playerCalculateAveragePlayTime(chess->player_map, player_id, &average_play_time);
    
   
    *chess_result = chessResultToPlayerResult(player_result);
    return average_play_time;
}

/**
 * chessSavePlayersLevels: prints the rating of all players in the system as
 * explained in the *.pdf
 *
 * @param chess - a chess system. Must be non-NULL.
 * @param file - an open, writable output stream, to which the ratings are printed.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_SAVE_FAILURE - if an error occurred while saving.
 *     CHESS_SUCCESS - if the ratings was printed successfully.
 */
ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    
    Map player_map_copy = playerMapCopy(chess->player_map);
    double max_level = 0;
    PlayerId player_id = playerGetMaxLevelAndId(player_map_copy, &max_level, true);
    
    while (playerIdIsValid(player_id)){
        if (max_level > 0){
            fprintf(file, "%d %.2f", player_id, max_level);
        }
        player_id = playerGetMaxLevelAndId(player_map_copy, &max_level, true);
    }
    playerDestroyMap(player_map_copy);
    return CHESS_SUCCESS;
}

/**
 * chessSaveTournamentStatistics: prints to the file the statistics for each tournament that ended as
 * explained in the *.pdf
 *
 * @param chess - a chess system. Must be non-NULL.
 * @param path_file - the file path which within it the tournament statistics will be saved.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_NO_TOURNAMENTS_ENDED - if there are no tournaments ended in the system.
 *     CHESS_SAVE_FAILURE - if an error occurred while saving.
 *     CHESS_SUCCESS - if the ratings was printed successfully.
 */
ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file){
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    
    TournamentResult result = tournamentSaveStatistics(chess->tournament_map, path_file);

    return chessResultToTournamentResult(result);
}
