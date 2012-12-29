#ifndef __CWIN_H__
#define __CWIN_H__
#ifdef WIN32
    #include <PDCurses/curses.h>
#else
    #include <ncurses.h>
#endif
#include <string>
#include <vector>

#ifdef WIN32
    #define COLOUR_BLACK       0x0
    #define COLOUR_DARK_BLUE   0x1
    #define COLOUR_DARK_GREEN  0x2
    #define COLOUR_TURQUOISE   0x3
    #define COLOUR_DARK_RED    0x4
    #define COLOUR_PURPLE      0x5
    #define COLOUR_DARK_YELLOW 0x6
    #define COLOUR_LIGHT_GREY  0x7
    #define COLOUR_DARK_GREY   0x8
    #define COLOUR_BLUE        0x9
    #define COLOUR_GREEN       0xA
    #define COLOUR_CYAN        0xB
    #define COLOUR_RED         0xC
    #define COLOUR_MAGENTA     0xD
    #define COLOUR_YELLOW      0xE
    #define COLOUR_WHITE       0xF
#else
    #define COLOUR_BLACK    0x0
    #define COLOUR_BLUE     0x1
    #define COLOUR_GREEN    0x2
    #define COLOUR_CYAN     0x3
    #define COLOUR_RED      0x4
    #define COLOUR_MAGENTA  0x5
    #define COLOUR_YELLOW   0x6
    #define COLOUR_WHITE    0x7
#endif


class CWinStorage;

class CWin{
    public:
        CWin(unsigned int sx, unsigned int sy, unsigned int dx, unsigned int dy, bool scrolling = false);
        ~CWin();

        void mv(unsigned int x, unsigned int y);
        void print(int ch, unsigned int x = -1, unsigned int y = -1);
        void print(std::string in, int attr = A_NORMAL, unsigned int x = -1, unsigned int y = -1);
        void refresh();
        void clear();
        void test_border();
        void horiz_line(unsigned int width, unsigned int x=-1, unsigned int y=-1);
        int createColor(int FG, int BG);
        void attrOn(chtype attr);
        void attrOff(chtype attr);

        unsigned int lastCol() const;//should this be private?
        void setCol(unsigned int newCol, bool raw = false);
        WINDOW* win() const;
        static void start();
        static void end();

        unsigned int xMax() const;
        unsigned int yMax() const;
        unsigned int xPos() const;
        unsigned int yPos() const;

        int sscroll(int n);
        void saveState();
        void restoreState();

        std::string input(unsigned int x, unsigned int y, unsigned int dx);

        /*
            constants
        */
        static int key_enter();
        static int key_backspace();
        static int key_end();
        static int window_x();
        static int window_y();
    private:
        void defarg(unsigned int& x, unsigned int& y);

        static int colorInc;
        unsigned int _cX, _cY, _mX, _mY, sX, sY;
        int _lastCol;
        bool _scrolling;
        WINDOW* _win;
        CWinStorage* dataStore;

        static int _key_enter, _wx, _wy, _kbspc, _kend;
};

class CWinStorage{
    public:
        CWinStorage(CWin* window);
        int getChr(int x, int y) const;
        int getCol(int x, int y) const;
        int getCx() const;
        int getCy() const;
    private:
        class Node{
            public:
                Node(int chr, int col);
                int getChr() const;
                int getCol() const;
            private:
                int col, chr;
        };
        unsigned int width, height, cx, cy;
        std::vector<Node> nodes;
};


#endif // __CWIN_H__

