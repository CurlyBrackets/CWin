FLAGS = -Wall -std=c++0x
ifeq ($(shell uname), windows32)
	LIBS = -lpdcurses
	DEFS = -DWIN32
else
	ifneq ($(shell uname), Linux)
		FLAGS += -IC:/cygwin/usr/include/ncurses
	endif
	LIBS = -lncurses
endif

debug:
	g++ $(DEFS) $(FLAGS) main.cpp Cwin.cpp -o debug $(LIBS)

lib:
	g++ $(DEFS) $(FLAGS) -c CWin.cpp
	
clean:
	rm -rf *.o
