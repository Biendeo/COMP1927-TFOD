CC = gcc
CFLAGS = -Wall -Werror -g
BINS = testGameView testHunterView testDracView

all : $(BINS)

testGameView : testGameView.o Places.o Map.o Queue.o GameView.o Set.o
testHunterView : testHunterView.o HunterView.o Places.o Map.o Queue.o GameView.o Set.o
testDracView : testDracView.o DracView.o Places.o Map.o Queue.o GameView.o Set.o

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


clean :
	rm -f $(BINS) *.o core

