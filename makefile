all:
	g++ main.cpp source.cpp -o debug -lncurses
clean:
	rm -rf *.o
