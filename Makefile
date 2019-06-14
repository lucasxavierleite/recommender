all:
	gcc aux.c filme.c grafo.c recommender.c -g -o recommender

run:
	./recommender filmes.txt

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./main

