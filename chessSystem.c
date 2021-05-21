#include "chessSystem.h"
#include "map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


/**
 *
 *              ################ NOTE! ################
 * the new update says that max_games_per_player is per tournament and not
 * counted by the total games a player has played, therefor I moved the player map
 * to the Tournament Data (from the main Chess_System Struct)
 */

#define PLAYER_SEPARATOR '#'

/**************************************************************/
/********************* Type definitions *********************/
typedef int* PlayerId;// Key = player_id
typedef struct player_data* PlayerData;

typedef char* GameId; /// key = "player1_id"+ '#' +"player2_id"
typedef struct game_data* GameData;

typedef int* TournamentId;// keys = 1,2,3,....
typedef struct tournament_data* TournamentData;


typedef const char* Location;

/***************************************************************/
/********************* Helper Functions *********************/
static char* playersIdToGameId(int player1_id, int player2_id);
static int countDigits(int num);

/********************* Player functions *********************/
static void freePlayerData(PlayerData data);
static void frePlayerId(PlayerId id);
static PlayerData copyPlayerData(PlayerData data);
static PlayerId copyPlayerId(PlayerId id);
static int comparePlayerId(PlayerId id1, PlayerId id2);

static bool playerIdIsValid(PlayerId id);
static PlayerId createPlayerId(int player1_id);
static PlayerData createPlayerData();
static ChessResult addPlayer(Map player_map, PlayerId player_id){
    if (!player_map){
        return CHESS_NULL_ARGUMENT;
    }
    if (!playerIdIsValid(player_id)){
        return CHESS_INVALID_ID;
    }
    PlayerId id = createPlayerId(*player_id);
    if (!id){
        return CHESS_OUT_OF_MEMORY;
    }
    PlayerData data = createPlayerData();
    if (!data){
        frePlayerId(id);
        return CHESS_OUT_OF_MEMORY;
    }
    
    if (mapPut(player_map, id, data) != MAP_SUCCESS){
        frePlayerId(id);
        freePlayerData(data);
        return CHESS_OUT_OF_MEMORY;
    }
    return CHESS_SUCCESS;
}



/********************* Tournament functions *********************/
static void freeTournamentData(TournamentData data);
static void freTournamentId(TournamentId id);
static TournamentData copyTournamentData(TournamentData data);
static TournamentId copyTournamentId(TournamentId id);
static int compareTournamentId(TournamentId* id1, TournamentId* id2);

static TournamentId createTournamentId();
static TournamentData createTournamentData();
static bool tournamentIdIsValid(TournamentId id);

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
static bool validPlayTime(int play_time);


struct chess_system_t{
    Map tournament_map;
    ///player map moved to Tournament Data
};

struct player_data{
    int num_of_games; // used to calculate averages when needed
    int num_of_wins; // used to calculate Level
    int num_of_loses; // used to calculate Level
    int num_of_draws; // used to calculate Level
    int total_play_time; // used to calculate average playtime
};

struct tournament_data{
    Location location;
    int max_games_per_player;
    bool has_ended;
    Map game_map;
    Map player_map;
    /// num_of_players removed since player map already has it
    int longest_game_time; // printed in tournament statistics
    long total_game_time; // used for tournament statistics
    PlayerId winner;
};

struct game_data{
    int play_time;
    Winner winner;
    /// REMOVED: PlayerId player1_id;since the id contains it
    /// REMOVED: PlayerId player2_id; since the id contains it
};

/*************************************************************/
/********************* Static Functions *********************/

/**
 * count_digits:
 *
 * @param
 *
 * @return
 */
static int countDigits(int num){
    int count = 0;
    do{
        num = num / 10;
        count++;
    }while (num != 0);
    return count;
}

/**
 * id_to_str:
 *
 * @param
 *
 * @return
 */
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


/********************* Player functions *********************/
/**
 * tournamentIdIsValid:
 *
 * @param
 *
 * @return
 */
static bool playerIdIsValid(PlayerId id){
    if (*id > 0){
        return true;
    }
    return false;
}

/********************* Tournament functions *********************/
/**
 * tournamentIdIsValid:
 *
 * @param
 *
 * @return
 */
static bool tournamentIdIsValid(TournamentId id){
    if (*id > 0){
        return true;
    }
    return false;
}

/********************* Game functions *********************/
/**
 * tournamentIdIsValid:
 *
 * @param
 *
 * @return
 */
static bool gameExists(Map game_map, PlayerId player_id1, PlayerId player_id2){
    ChessResult chess_result = CHESS_SUCCESS;
    bool result = false;
    GameId game_id = createGameId(player_id1, player_id2, &chess_result);
    if (!game_id || !game_map){
        return false;
    }
    if (mapContains(game_map, game_id)){
        result = true;
    }
    freeGameId(game_id);
    return result;
}
/**
 * createGameData:
 *
 * @param play_time
 * @param winner
 *
 * @return
 * NULL - if failed
 */
static GameData createGameData(int play_time, Winner winner, ChessResult* result){
    if (!validPlayTime(play_time)){
        *result = CHESS_INVALID_PLAY_TIME;
        return NULL;
    }
    GameData gameData = malloc(sizeof(*gameData));
    if (!gameData){
        *result = CHESS_OUT_OF_MEMORY;
        return NULL;
    }
    gameData->play_time = play_time;
    gameData->winner = winner;
    return gameData;
}
static bool validPlayTime(int play_time){
    if (play_time > 0){
        return true;
    }
    return false;
}
static ChessResult addGame(Map game_map, int play_time, Winner winner,
                           PlayerId player1_id, PlayerId player2_id){
    ChessResult result = CHESS_SUCCESS;
    if (!game_map){
        return CHESS_NULL_ARGUMENT;
    }
    GameData game_data = createGameData(play_time, winner, &result);
    if (!game_data){
        assert(result != CHESS_SUCCESS);
        return result;
    }
    GameId game_id = createGameId(player1_id, player2_id, &result);
    if (!game_id){
        assert(result != CHESS_SUCCESS);
        freeGameData(game_data);
        return result;
    }
    if (mapPut(game_map, game_id, game_data) != MAP_SUCCESS){
        freeGameData(game_data);
        freeGameId(game_id);
        return CHESS_OUT_OF_MEMORY;
    }
    return CHESS_SUCCESS;
}

static GameId createGameId(PlayerId player1_id, PlayerId player2_id, ChessResult* result){
    
    if (!playerIdIsValid(player1_id) || !playerIdIsValid(player2_id)){
        *result = CHESS_INVALID_ID;
    }
    
    GameId game_id = playersIdToGameId(*player1_id, *player2_id);
    if (!game_id){
        *result = CHESS_OUT_OF_MEMORY;
        return NULL;
    }
    /// format as "player1_id"+ '#' +"player2_id"
    
    return game_id;
}






/*************************************************************/
/********************* Public Functions *********************/

/**
 * chessCreate: create an empty chess system.
 *
 * @return A new chess system in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
ChessSystem chessCreate(){
    printf("");
}

/**
 * chessDestroy: free a chess system, and all its contents, from
 * memory.
 *
 * @param chess - the chess system to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void chessDestroy(ChessSystem chess){
    printf("");
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
    printf("");
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
/// todo
ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time){
    
    if (!chess){
        return CHESS_NULL_ARGUMENT;
    }
    if (!tournamentIdIsValid(&tournament_id) || !playerIdIsValid(&first_player) ||
        !playerIdIsValid(&second_player) || !comparePlayerId(&first_player, &second_player)){
        return CHESS_INVALID_ID;
    }
    if (!mapContains(chess->tournament_map, &tournament_id)){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    // get tournament data and check if it has ended
    TournamentData tournament_data = mapGet(chess->tournament_map, &tournament_id);
    if (tournament_data->has_ended){
        return CHESS_TOURNAMENT_ENDED;
    }
    // get game map and check if it's valid
    Map game_map = tournament_data->game_map;
    assert(game_map);
    
    // check if the game already exists
    if (gameExists(game_map, &first_player, &second_player)){
        return CHESS_GAME_ALREADY_EXISTS;
    }
    // check if play_time is valid
    if (!validPlayTime(play_time)){
        return CHESS_INVALID_PLAY_TIME;
    }
    
    Map player_map = tournament_data->player_map;
    assert(player_map); // player map should be valid
    
    PlayerData player1 = mapGet(player_map, &first_player);
    if (player1 == NULL && addPlayer(player_map, &first_player) != CHESS_SUCCESS){// player doesn't exist ==> add him to the map
        return CHESS_OUT_OF_MEMORY;
    }
    PlayerData player2 = mapGet(player_map, &second_player);
    if (player2 == NULL && addPlayer(player_map, &second_player) != CHESS_SUCCESS){// player doesn't exist ==> add him to the map
        return CHESS_OUT_OF_MEMORY;
    }
    
    if (player1->num_of_games >= tournament_data->max_games_per_player ||
        player2->num_of_games >= tournament_data->max_games_per_player){
        return CHESS_EXCEEDED_GAMES;
    }
    
    return addGame(game_map, play_time, winner, &first_player, &second_player);
}

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
ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id);

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
/// todo
ChessResult chessRemovePlayer(ChessSystem chess, int player_id);

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
ChessResult chessEndTournament (ChessSystem chess, int tournament_id);

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
/// todo
double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result);

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
 /// todo
ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file);

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
/// todo
ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file);
