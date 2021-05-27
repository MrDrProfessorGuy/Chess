
CC = gcc
EXEC1 = chess
OBJS1 = chessSystemTestsExample.o game_map.o player_map.o tournament_map.o chessSystem.o
OBJS2 = chess.o
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors 

$(EXEC1) : $(OBJS2)
	$(CC) $(OBJS2) $(COMP_FLAG) -L. -lmap -o $@
$(OBJS2) : $(OBJS1)
	-ld -r -o $(OBJS2) $(OBJS1)


chessSystemTestsExample.o : tests/chessSystemTestsExample.c chessSystem.h test_utilities.h
	gcc $(COMP_FLAG) -c -o chessSystemTestsExample.o tests/chessSystemTestsExample.c
chessSystem.o : chessSystem.c chessSystem.h player_map.h game_map.h tournament_map.h
	gcc $(COMP_FLAG) -c -o chessSystem.o chessSystem.c
game_map.o : game_map.c game_map.h map.h
	gcc $(COMP_FLAG) -c -o game_map.o game_map.c
player_map.o : player_map.c player_map.h map.h
	gcc $(COMP_FLAG) -c -o player_map.o player_map.c
tournament_map.o : tournament_map.c tournament_map.h map.h player_map.h game_map.h
	gcc $(COMP_FLAG) -c -o tournament_map.o tournament_map.c


clean:
	rm -f $(OBJS1) $(OBJS2) $(EXEC)



















