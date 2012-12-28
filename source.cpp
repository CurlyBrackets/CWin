#include <Curly/CWin.hpp>
#include <iostream>

int CWin::colorInc = 1;

CWin::CWin(unsigned int sx, unsigned int sy, unsigned int dx, unsigned int dy, bool scrolling):
_cX(0),_cY(0),_mX(dx),_mY(dy), sX(sx), sY(sy), _lastCol(-1),_scrolling(scrolling), _win(NULL), dataStore(NULL){
    _win = newwin(dy, dx, sy, sx);
    scrollok(_win, scrolling);
}

CWin::~CWin(){
    delwin(_win);
    if(dataStore)
        delete dataStore;
}

void CWin::mv(unsigned int x, unsigned int y){
    wmove(_win, y, x);
    _cX = x;
    _cY = y;
}

void CWin::print(int ch, unsigned int x, unsigned int y){
    defarg(x,y);
    mvwaddch(_win, y, x, ch);
    _cX++;
    if(xPos() >= xMax()){
        _cX = 0;
        _cY++;
        if(_cY >= yMax()){
            _cY = yMax()-1;
            _cX = 0;
            //this->scroll(1);
        }
    }
}

void CWin::print(std::string in, int attr, unsigned int x, unsigned int y){
    defarg(x,y);
    mv(x,y);
    attrOn(attr);
    for(unsigned int chinc=0;chinc<in.length();chinc++)
        print(in[chinc]);
    attrOff(attr);
}

void CWin::refresh(){
    wrefresh(_win);
}

void CWin::clear(){
    this->mv(0,0);
    for(unsigned int y=0;y<yMax();y++){
        for(unsigned int x=0;x<xMax();x++){
            //print(ACS_BLOCK);
            print(' ');
        }
    }
    this->refresh();
}

void CWin::border(){
    print('.', 0,0);
    print('.', xMax()-1,0);
    print('\'', 0,yMax()-1);
    print('\'', xMax()-1,yMax()-1);
    for(unsigned int x=1;x<xMax()-1;x++){
        print('-',x,0);
        print('-',x,yMax()-1);
    }
    for(unsigned int y=1;y<yMax()-1;y++){
        print('|',0,y);
        print('|',xMax()-1,y);
    }
}

void CWin::hline(unsigned int width, unsigned int x, unsigned int y){

}

int CWin::createColor(int FG, int BG){
    init_pair(colorInc++,FG,BG);
    return colorInc-1;
}

void CWin::attrOn(chtype attr){
    wattron(_win, attr);
}

void CWin::attrOff(chtype attr){
    wattroff(_win, attr);
}

unsigned int CWin::lastCol() const{
    return _lastCol;
}

void CWin::setCol(unsigned int newCol, bool raw){
    if(!raw)
        newCol = COLOR_PAIR(newCol);
    if(newCol != lastCol()){
        attrOff(lastCol());
        attrOn(newCol);
        _lastCol = newCol;
    }
}

WINDOW* CWin::win() const{
    return _win;
}

void CWin::start(){
    initscr();
    start_color();
    noecho();
    raw();
    keypad(stdscr, TRUE);
}

void CWin::end(){
    endwin();
}

unsigned int CWin::xMax() const{
    return _mX;
}
unsigned int CWin::yMax() const{
    return _mY;
}
unsigned int CWin::xPos() const{
    return _cX;
}
unsigned int CWin::yPos() const{
    return _cY;
}

void CWin::defarg(unsigned int& x, unsigned int& y){
    if(x == (unsigned int)-1)
        x = xPos();
    if(y == (unsigned int)-1)
        y = yPos();
}

int CWin::scroll(int n){
    return wscrl(_win, n);
}

void CWin::saveState(){
    if(dataStore)
        delete dataStore;
    dataStore = new CWinStorage(this);
}

void CWin::restoreState(){
    if(dataStore){
        scrollok(_win, false);
        for(unsigned int y=0;y<yMax();y++){
            for(unsigned int x=0;x<xMax();x++){
                setCol(dataStore->getCol(x,y), true);
                print(dataStore->getChr(x,y), x, y);
            }
        }
        mv(dataStore->getCx(), dataStore->getCy());
        redrawwin(win());
        scrollok(_win, _scrolling);
    }
}

std::string CWin::input(unsigned int x, unsigned int y,unsigned int dx){
    std::string buffer;
    int ch;
    mv(x,y);
    do{
        ch = getch();
        if(ch > 0xF && ch < 0x7F){
            //check for dx
            print(ch);
            this->refresh();
            buffer += ch;
            move(y, sX+x+buffer.length());
        }
        else if(ch == 8){
            //clear and reprint buffer
            buffer = buffer.substr(0, buffer.length()-1);
            mv(x,y);
            for(unsigned int i=0;i<dx;i++)
                print(' ');
            print(buffer, A_NORMAL, x, y);
            this->refresh();
            move(y, sX+x+buffer.length());

        }
    }while(ch != 13);
    return buffer;
}

CWinStorage::CWinStorage(CWin* window):
width(window->xMax()), height(window->yMax()), cx(window->xPos()), cy(window->yPos()){
    for(unsigned int y=0;y<window->yMax();y++){
        for(unsigned int x=0;x<window->xMax();x++){
            nodes.push_back(CWinStorage::Node(mvwinch(window->win(), y, x)&A_CHARTEXT, mvwinch(window->win(), y, x)&A_COLOR));
        }
    }
}

int CWinStorage::getChr(int x, int y) const{
    return nodes[width*y+x].getChr();
}

int CWinStorage::getCol(int x, int y) const{
    return nodes[width*y+x].getCol();
}

int CWinStorage::getCx() const{
    return cx;
}
int CWinStorage::getCy() const{
    return cy;
}

CWinStorage::Node::Node(int chr, int col):
col(col),chr(chr){}

int CWinStorage::Node::getChr() const{
    return chr;
}
int CWinStorage::Node::getCol() const{
    return col;
}
