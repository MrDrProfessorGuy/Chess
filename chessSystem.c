#include "chessSystem.h"
#include "player_map.h"
#include "tournament_map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


struct chess_system_t {
    Map tournament_map;
    Map player_map;
};

/*************************************************************/
/********************* Static Functions *********************/
/***********************************************************/
/**
 * chessResultToTournamentResult - convert TournamentResult to ChessResult equivalent
 * @param result - TournamentResult
 * @return
 *  ChessResult equivalent
 */
static ChessResult chessResultToTournamentResult(TournamentResult result) {
    if (result == TOURNAMENT_OUT_OF_MEMORY) {
        return CHESS_OUT_OF_MEMORY;
    }
    else if (result == TOURNAMENT_NULL_ARGUMENT) {
        return CHESS_NULL_ARGUMENT;
    }
    else if (result == TOURNAMENT_INVALID_ID) {
        return CHESS_INVALID_ID;
    }
    else if (result == TOURNAMENT_INVALID_LOCATION) {
        return CHESS_INVALID_LOCATION;
    }
    else if (result == TOURNAMENT_INVALID_MAX_GAMES) {
        return CHESS_INVALID_MAX_GAMES;
    }
    else if (result == TOURNAMENT_TOURNAMENT_ALREADY_EXISTS) {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    else if (result == TOURNAMENT_TOURNAMENT_NOT_EXIST) {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    else if (result == TOURNAMENT_GAME_ALREADY_EXISTS) {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    else if (result == TOURNAMENT_INVALID_PLAY_TIME) {
        return CHESS_INVALID_PLAY_TIME;
    }
    else if (result == TOURNAMENT_EXCEEDED_GAMES) {
        return CHESS_EXCEEDED_GAMES;
    }
    else if (result == TOURNAMENT_PLAYER_NOT_EXIST) {
        return CHESS_PLAYER_NOT_EXIST;
    }
    else if (result == TOURNAMENT_TOURNAMENT_ENDED) {
        return CHESS_TOURNAMENT_ENDED;
    }
    else if (result == TOURNAMENT_NO_TOURNAMENTS_ENDED) {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    else if (result == TOURNAMENT_NO_GAMES) {
        return CHESS_NO_GAMES;
    }
    else if (result == TOURNAMENT_SAVE_FAILURE) {
        return CHESS_SAVE_FAILURE;
    }
    assert(result == TOURNAMENT_SUCCESS);
    return CHESS_SUCCESS;
}
/**
 * chessResultToPlayerResult - convert PlayerResult to ChessResult equivalent
 * @param result - PlayerResult
 * @return
 *  ChessResult equivalent
 */
static ChessResult chessResultToPlayerResult(PlayerResult result) {
    if (result == PLAYER_OUT_OF_MEMORY) {
        return CHESS_OUT_OF_MEMORY;
    }
    else if (result == PLAYER_NULL_ARGUMENT) {
        return CHESS_NULL_ARGUMENT;
    }
    else if (result == PLAYER_INVALID_ID) {
        return CHESS_INVALID_ID;
    }
    else if (result == PLAYER_INVALID_LOCATION) {
        return CHESS_INVALID_LOCATION;
    }
    else if (result == PLAYER_INVALID_MAX_GAMES) {
        return CHESS_INVALID_MAX_GAMES;
    }
    else if (result == PLAYER_TOURNAMENT_ALREADY_EXISTS) {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    else if (result == PLAYER_TOURNAMENT_NOT_EXIST) {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    else if (result == PLAYER_GAME_ALREADY_EXISTS) {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    else if (result == PLAYER_INVALID_PLAY_TIME) {
        return CHESS_INVALID_PLAY_TIME;
    }
    else if (result == PLAYER_EXCEEDED_GAMES) {
        return CHESS_EXCEEDED_GAMES;
    }
    else if (result == PLAYER_NOT_EXIST) {
        return CHESS_PLAYER_NOT_EXIST;
    }
    else if (result == PLAYER_TOURNAMENT_ENDED) {
        return CHESS_TOURNAMENT_ENDED;
    }
    else if (result == PLAYER_NO_TOURNAMENTS_ENDED) {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    else if (result == PLAYER_NO_GAMES) {
        return CHESS_NO_GAMES;
    }
    else if (result == PLAYER_SAVE_FAILURE) {
        return CHESS_SAVE_FAILURE;
    }
    assert(result == PLAYER_SUCCESS);
    return CHESS_SUCCESS;
}
/**
 * checkValidityForAddGame - check the validity of AddGame request
 * @param chess - the chess check it's validity
 * @param tournament_id - the id tournament to look for
 * @param first_player - the first players id
 * @param second_player - the second players id
 * @param play_time - play time
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
static ChessResult checkValidityForAddGame(ChessSystem chess, int tournament_id, int first_player,
    int second_player, int play_time) {
    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id) || !playerIdIsValid(first_player) ||
        !playerIdIsValid(second_player) || first_player == second_player) {
        return CHESS_INVALID_ID;
    }
    Map tournament_map = chess->tournament_map;
    assert(tournament_map);
    Map tournament_player_map = tournamentGetPlayerMap(tournament_map, tournament_id);
    Map tournament_game_map = tournamentGetGameMap(tournament_map, tournament_id);

    if (!tournament_game_map) {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (tournamentEnded(tournament_map, tournament_id)) {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (gameExists(tournament_game_map, first_player, second_player)) {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if (!playTimeIsValid(play_time)) {
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games = 0;
    tournamentGetMaxGamesPerPlayer(tournament_map, tournament_id, &max_games);
    if (playerExceededGames(tournament_player_map, first_player, max_games) ||
        playerExceededGames(tournament_player_map, second_player, max_games)) {

        return CHESS_EXCEEDED_GAMES;
    }

    return CHESS_SUCCESS;
}
/**
 * addAndUpdatePlayersData - add players if nesescary and update their statistics
 * @param player_map - the map player to add to and update
 * @param player1_id - the first player's id
 * @param player2_id - the secinde player's id
 * @param winner - the deafault winner
 * @param play_time - play time
 * @return
 *  CHESS_OUT_OF_MEMORY - if allocation failed
 *  CHESS_SUCCESS - otherwise
 */
static ChessResult addAndUpdatePlayersData(Map player_map, PlayerId player1_id,
    PlayerId player2_id, Winner winner, int play_time) {
    assert(player_map);
    assert(playerIdIsValid(player1_id) && playerIdIsValid(player2_id));

    if (playerAdd(player_map, player1_id) != PLAYER_SUCCESS) {
        return CHESS_OUT_OF_MEMORY;
    }
    if (playerAdd(player_map, player2_id) != PLAYER_SUCCESS) {
        playerRemove(player_map, player1_id);
        return CHESS_OUT_OF_MEMORY;
    }

    playerUpdateDuelResult(player_map, player1_id, player2_id, play_time, winner, ADD);
    return CHESS_SUCCESS;
}

/*************************************************************/
/********************* Public Functions *********************/
/***********************************************************/

ChessSystem chessCreate() {
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    if (!chess_system) {
        return NULL;
    }

    chess_system->player_map = playerCreateMap();
    if (!chess_system->player_map) {
        chessDestroy(chess_system);
        return NULL;
    }
    chess_system->tournament_map = tournamentCreate();
    if (!chess_system->tournament_map) {
        chessDestroy(chess_system);
        return NULL;
    }

    return chess_system;
}

void chessDestroy(ChessSystem chess) {
    if (!chess) {
        return;
    }
    playerDestroyMap(chess->player_map);
    tournamentDestroy(chess->tournament_map);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id,
    int max_games_per_player, const char* tournament_location) {

    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)) {
        return CHESS_INVALID_ID;
    }
    if (tournamentContains(chess->tournament_map, tournament_id)) {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if (!tournamentLocationIsValid(tournament_location)) {
        return CHESS_INVALID_LOCATION;
    }
    if (max_games_per_player <= 0) {
        return CHESS_INVALID_MAX_GAMES;
    }

    TournamentResult result = tournamentAdd(chess->tournament_map,
        tournament_id, max_games_per_player, tournament_location);

    return chessResultToTournamentResult(result);

}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
    int second_player, Winner winner, int play_time) {
    ChessResult validity_check = checkValidityForAddGame(chess, tournament_id, first_player, second_player,
        play_time);
    if (validity_check != CHESS_SUCCESS) {
        return validity_check;
    }
    //no checks from here since all the conditions were already checked
    Map tournament_map = chess->tournament_map;
    Map tournament_player_map = tournamentGetPlayerMap(tournament_map, tournament_id);
    Map tournament_game_map = tournamentGetGameMap(chess->tournament_map, tournament_id);


    GameResult result = gameAdd(tournament_game_map, play_time, winner, first_player, second_player);
    if (result != GAME_SUCCESS) {
        return CHESS_OUT_OF_MEMORY;
    }
    int new_players = 0;
    new_players += !playerExists(tournament_player_map, first_player);
    new_players += !playerExists(tournament_player_map, second_player);

    if (addAndUpdatePlayersData(chess->player_map, first_player, second_player, winner, play_time) != CHESS_SUCCESS) {
        gameRemove(tournament_game_map, first_player, second_player);
        return CHESS_OUT_OF_MEMORY;
    }
    if (addAndUpdatePlayersData(tournament_player_map, first_player, second_player, winner, play_time)
        != CHESS_SUCCESS) {
        playerUpdateDuelResult(chess->player_map, first_player, second_player, play_time, winner, UNDO);
        gameRemove(tournament_game_map, first_player, second_player);
        return CHESS_OUT_OF_MEMORY;
    }
    tournamentUpdateStatistics(tournament_map, tournament_id, play_time, new_players);
    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id) {
    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)) {
        return CHESS_INVALID_ID;
    }
    if (tournamentContains(chess->tournament_map, tournament_id) == false) {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Map tournament_player_map = tournamentGetPlayerMap(chess->tournament_map, tournament_id);
    if (!tournament_player_map) {
        return CHESS_OUT_OF_MEMORY;
    }
    assert(chess->player_map);
    playerMapUpdateStatistics(chess->player_map, tournament_player_map, false, UNDO);

    TournamentResult tournament_result = tournamentRemove(chess->tournament_map, tournament_id);
    return chessResultToTournamentResult(tournament_result);
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id) {
    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }
    if (!playerIdIsValid(player_id)) {
        return CHESS_INVALID_ID;
    }
    if (!playerExists(chess->player_map, player_id)) {
        return CHESS_PLAYER_NOT_EXIST;
    }

    ///iterate over all tournaments that have not ended
    /// and remove the player from the map and all games he played in
    tournamentPlayerRemove(chess->tournament_map, chess->player_map, player_id);
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id) {
    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(tournament_id)) {
        return CHESS_INVALID_ID;
    }
    if (!tournamentContains(chess->tournament_map, tournament_id)) {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (tournamentEnded(chess->tournament_map, tournament_id)) {
        return CHESS_TOURNAMENT_ENDED;
    }
    if (tournamentNumOfGames(chess->tournament_map, tournament_id) <= 0) {
        return CHESS_NO_GAMES;
    }

    TournamentResult tournament_result = tournamentEnd(chess->tournament_map, tournament_id);
    return chessResultToTournamentResult(tournament_result);
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result) {
    if (!chess) {
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }
    if (!playerIdIsValid(player_id)) {
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    if (!playerExists(chess->player_map, player_id)) {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }

    double average_play_time = 0;
    PlayerResult player_result = playerCalculateAveragePlayTime(chess->player_map, player_id, &average_play_time);


    *chess_result = chessResultToPlayerResult(player_result);
    return average_play_time;
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file) {
    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }

    Map player_map_copy = playerMapCopy(chess->player_map);
    double max_level = 0;
    PlayerId player_id = playerGetMaxLevelAndId(player_map_copy, &max_level, true);

    while (playerIdIsValid(player_id)) {
        //if (max_level > 0){
        fprintf(file, "%d %.2f\n", player_id, max_level);
        //}
        player_id = playerGetMaxLevelAndId(player_map_copy, &max_level, true);
    }
    playerDestroyMap(player_map_copy);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char* path_file) {
    if (!chess) {
        return CHESS_NULL_ARGUMENT;
    }

    TournamentResult result = tournamentSaveStatistics(chess->tournament_map, path_file);

    return chessResultToTournamentResult(result);
}
