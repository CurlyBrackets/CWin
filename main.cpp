#include "CWin.hpp"
#include <iostream>
#include <cstdlib>

class Node{
    public:
        Node(unsigned int colour):
        _colour(colour),_link(NULL){}
        ~Node(){
            if(next() && !next()->first())
                delete next();
        }
        void link(Node* node){
            _link = node;
        }
        Node* next() const{
            return _link;
        }

        unsigned int colour() const{
            return _colour;
        }
        void push(Node* node){
            if(next()){
                if(next()->first()){
                    Node* temp = next();
                    link(node);
                    node->link(temp);
                }
                else
                    next()->push(node);

            }
            else if(first()){
                node->link(this);
                link(node);
            }
        }

        bool first() const{
            return _first;
        }
        void first(bool value){
            _first = value;
        }

    private:
        unsigned int _colour;
        bool _first;
        Node* _link;
};

int main(){
    CWin::start();
    CWin* test = new CWin(1,1,10,10);
    refresh();
    Node* first = new Node(test->createColor(0x0, 0x7)), *node = first;
    first->first(true);
    first->push(new Node(test->createColor(0xE, 0x0)));
    first->push(new Node(test->createColor(0xD, 0x0)));
    first->push(new Node(test->createColor(0xC, 0x0)));
    first->push(new Node(test->createColor(0xB, 0x0)));
    first->push(new Node(test->createColor(0xA, 0x0)));
    first->push(new Node(test->createColor(0x9, 0x0)));
    first->push(new Node(test->createColor(0x8, 0x0)));
    first->push(new Node(test->createColor(0x7, 0x0)));
    first->push(new Node(test->createColor(0x6, 0x0)));
    first->push(new Node(test->createColor(0x5, 0x0)));
    first->push(new Node(test->createColor(0x4, 0x0)));
    first->push(new Node(test->createColor(0x3, 0x0)));
    first->push(new Node(test->createColor(0x2, 0x0)));
    first->push(new Node(test->createColor(0x1, 0x0)));

    int chr = 32;
    for(unsigned int y=0;y<test->yMax();y++){
        for(unsigned int x=0;x<test->xMax();x++){
            test->setCol(node->colour());
            node = node->next();
            test->print(chr++);
            if(chr > 127){
                chr = 32;
            }
        }
    }

    test->refresh();
    test->saveState();
    getch();
    CWin* derp = new CWin(0,0,80,25);
    refresh();
    derp->setCol(first->colour());
    derp->clear();
    derp->refresh();
    getch();
    delete derp;
    refresh();
	#ifdef WIN32
	    	system("cls");
	#else
		system("clear");
	#endif
    test->restoreState();
    redrawwin(test->win());
    test->print('a',0,0);
    test->refresh();
    getch();
    delete test;
    delete first;

    CWin* win = new CWin(0,0,80,25);
    refresh();
    win->print("Something: ");
    win->refresh();
    win->input(0, 1, 20);

    delete win;

	int maxy, maxx;
	
	getmaxyx(stdscr, maxy, maxx);

    CWin::end();

	std::cout << maxy << "\t" << maxx << std::endl;

    return 0;
}
