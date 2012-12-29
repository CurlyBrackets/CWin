FLAGS = -Wall -std=c++0x
ifneq ($(shell uname), Linux)
	LIBS = -lpdcurses
	DEFS = -DWIN32
else
	LIBS = -lncurses
endif

debug:
	g++ $(DEFS) $(FLAGS) main.cpp Cwin.cpp -o debug $(LIBS)

lib:
	g++ $(DEFS) $(FLAGS) -c CWin.cpp
	
clean:
	rm -rf *.o
