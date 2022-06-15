#ifndef MENU_INCLUDED
#define MENU_INCLUDED
#include "tinyxml.h"
static const char *filename = "../lcd_menu.xml";
const int CANCEL = 0;
const int OK = 1;
const int ADD = 48;
const int SUB = 49;
const int LEFT = 32;
const int RIGHT = 33;
const int UP = 16;
const int DOWN = 17;

class Menu
{
public:
    char name[32];
    Menu *(*func)(Menu *curr);
    bool is_selected;
    bool is_head;
    int x;
    int y;
    Menu *next, *front;
    Menu *child, *parent;

    Menu(char _name[], Menu *(*_func)(Menu *curr), Menu *_front, Menu *_parent);
    ~Menu();

    Menu *action(int act);
    void select();
    void cancel_select();

    void reset(Menu *curr);
    static void print_menu(Menu *curr);
    static void XML2Menu(TiXmlElement *rootElem, Menu *parent, Menu *front);
};

void print_menu(Menu *curr);
void create_func_table();

Menu *default_func(Menu *curr);

Menu *linkRemote(Menu *curr);
Menu *linkProtect(Menu *curr);
Menu *linkPMU(Menu *curr);
Menu *stateDevice(Menu *curr);

Menu *dataYX(Menu *curr);
Menu *dataYC(Menu *curr);
Menu *dataYK(Menu *curr);

Menu *recordSOE(Menu *curr);
Menu *recordYK(Menu *curr);
Menu *recordEvent(Menu *curr);

Menu *stateSynchronize(Menu *curr);
Menu *stateAlarm(Menu *curr);
Menu *stateMain(Menu *curr);
Menu *stateMain(Menu *curr);
Menu *version(Menu *curr);

Menu *setIPconfig(Menu *curr);
Menu *setRouting(Menu *curr);
Menu *setMAC(Menu *curr);

Menu *setPassword(Menu *curr);
Menu *setLCD(Menu *curr);
Menu *setChannel(Menu *curr);
Menu *setFuncExit(Menu *curr);
Menu *setClock(Menu *curr);

Menu *testNetwork(Menu *curr);
Menu *testSignalOut(Menu *curr);
Menu *testYK(Menu *curr);
Menu *testNumberSet(Menu *curr);
Menu *clearInfo(Menu *curr);

Menu *testLCD(Menu *curr);
Menu *testKey(Menu *curr);
Menu *testLED(Menu *curr);
Menu *testSerialPort232(Menu *curr);
Menu *testSerialPort232to311(Menu *curr);
Menu *testSerialPort485(Menu *curr);
Menu *TestCodeB(Menu *curr);
Menu *convertUser(Menu *curr);
Menu *reboot(Menu *curr);
Menu *poweroff(Menu *curr);
#endif