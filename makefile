all:
	g++ main.cpp source.cpp -o debug -lncurses
win:
	g++ main.cpp source.cpp -o debug -lpdcurses

clean:
	rm -rf *.o
