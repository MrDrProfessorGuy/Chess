gcc -std=c99 -o chessSystem -Wall -pedantic-errors -Werror -DNDEBUG *.c
    ./tests/chessSystemTestsExample.c -L. –lmap

output: chess.o


clean:
    rm *.o output





CC = gcc
OBJS1 = event_manager.o priority_queue.o date.o event.o member.o event_manager_example_tests.o
OBJS2 = priority_queue.o  pq_example_tests.o
EXEC1 = chess
DEBUG_FLAG = -g  -DNDEBUG
COMP_FLAG = -std=c99 -Wall -Werror

$(EXEC1) : $(OBJS1)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
$(EXEC2) : $(OBJS2)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

event_manager.o: event_manager.c priority_queue.h event_manager.h date.h \
 member.h event.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
priority_queue.o : priority_queue.c priority_queue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
date.o : date.c date.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
event.o : event.c event.h date.h member.h priority_queue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
member.o : member.c member.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(EXEC)s






















