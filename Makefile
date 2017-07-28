OBJS = genetic_algo.o individual.o
CC = clang++
DEBUG = -g
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)

algo : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o algo

genetic_algo.o : genetic_algo.cpp individual.h
	$(CC) $(CFLAGS) genetic_algo.cpp

individual.o : individual.h individual.cpp
	$(CC) $(CFLAGS) individual.cpp
