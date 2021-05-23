#include "tournament_map.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


typedef TournamentId* TournamentKey;
typedef struct tournament_data* TournamentData;

struct tournament_data{
    Location location;
    int max_games_per_player;
    bool has_ended;
    Map game_map;
    PlayerId winner_id;
};


/***************************************************************/
/********************* Tournament functions *********************/
static void freeTournamentData(MapDataElement data);
static void freeTournamentKey(MapKeyElement key);
static TournamentData copyTournamentData(TournamentData data);
static TournamentKey copyTournamentKey(TournamentKey key);
static int compareTournamentKey(TournamentKey id1, TournamentKey id2);

static TournamentKey createTournamentKey();
static TournamentData createTournamentData();




bool tournamentIdIsValid(TournamentId id){
    if (id > 0){
        return true;
    }
    return false;
}






