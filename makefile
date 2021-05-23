
CC = gcc
EXEC1 = chess
OBJS1 = chess.o


DEBUG_FLAG = -g  -DNDEBUG
COMP_FLAG = -std=c99 -pedantic-errors

$(EXEC1) : $(OBJS1)
	$(CC) $(DEBUG_FLAG) $(OBJS1) -o $@

chess.o : game_map.c game_map.h player_map.c player_map.h tournament_map.c tournament_map.h\
map.h libmap.a chessSystem.c chessSystem.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c ./tests/chessSystemTestsExample.c

clean:
	rm -f $(OBJS1) $(EXEC)






















