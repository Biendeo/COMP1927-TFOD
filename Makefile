CC = gcc
CFLAGS = -Wall -Werror -g -std=gnu99
BINS = testGameView testHunterView testDracView dracula hunter
COMMON = GameView.o Places.o Map.o Set.o Queue.o
LIBS = 

all : $(BINS)

testGameView : testGameView.o $(COMMON)
testHunterView : testHunterView.o HunterView.o $(COMMON)
testDracView : testDracView.o DracView.o $(COMMON)
dracula : dracPlayer.o dracula.o DracView.o $(COMMON) $(LIBS) 
hunter : huntPlayer.o hunter.o HunterView.o $(COMMON) $(LIBS)

Places.o : Places.c Places.h
Map.o : Map.c Map.h
Queue.o : Queue.c Queue.h
Set.o : Set.c Set.h
GameView.o : GameView.c GameView.h
HunterView.o : HunterView.c HunterView.h
DracView.o : DracView.c DracView.h
testGameView.o : testGameView.c
testHunterView.o : testHunterView.c
testDracView.o : testDracView.c
dracPlayer.o : player.c
	$(CC) $(CFLAGS) -D I_AM_DRACULA -c -o dracPlayer.o player.c
huntPlayer.o : player.c
	$(CC) $(CFLAGS) -c -o huntPlayer.o player.c
dracula.o : dracula.c dracula.h
hunter.o : hunter.c hunter.h

clean :
	rm -f $(BINS) *.o core

