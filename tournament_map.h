

#ifndef CHESS_TOURNAMENT_H
#define CHESS_TOURNAMENT_H

#include "chess_types.h"






/***************************************************************/
/********************* Tournament functions *********************/
static void freeTournamentData(TournamentData data);
static void freTournamentId(TournamentId id);
static TournamentData copyTournamentData(TournamentData data);
static TournamentId copyTournamentId(TournamentId id);
static int compareTournamentId(TournamentId* id1, TournamentId* id2);

static TournamentId createTournamentId();
static TournamentData createTournamentData();
static bool tournamentIdIsValid(TournamentId id);










#endif //CHESS_TOURNAMENT_H
