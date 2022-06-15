#include <iostream>
#include <stdio.h>
#include <map>
#include <string.h>
#include "menu.h"
#include "tinyxml.h"
using namespace std;

/*
    增加菜单项：1.修改lcd_menu.xml，插入新增菜单名及对应业务函数名
               2.在func.cpp和menu.h中添加业务函数
               3.在create_func_table函数中添加业务函数名和函数对应关系
*/
Menu *root;
map<string, Menu *(*)(Menu *)> funcTable;
int run = 1;
Menu::Menu(char _name[], Menu *(*_func)(Menu *curr), Menu *_front, Menu *_parent)
{
    strcpy(name, _name);
    func = _func;
    is_selected = false;
    is_head = false;
    next = this;

    if (_front)
        front = _front;
    else
        front = this;

    child = NULL;
    parent = _parent;

    x = 0;
    y = 0;
}
Menu::~Menu()
{
    if (!next->is_head)
        delete next;
    if (child)
        delete child;
    fprintf(stdout, "free %s\n", name);
}

Menu *Menu::action(int act)
{
    Menu *ret;
    switch (act)
    {
    case CANCEL: // cancel
    case LEFT:   // left
        this->cancel_select();
        ret = parent;
        break;
    case OK: // ok
    case RIGHT:
        ret = func(this);
        break;
    case UP:  // up
    case ADD: // add
        this->cancel_select();
        ret = front;
        break;
    case DOWN: // down
    case SUB:  // sub
        this->cancel_select();
        ret = next;
        break;
    default:
        printf("key error\n");
        ret = this;
        break;
    }
    if (!ret)
        ret = root;
    ret->select();
    print_menu(root);
    return ret;
}

void Menu::select()
{
    is_selected = true;
}

void Menu::cancel_select()
{
    if (is_selected)
        is_selected = false;
}

Menu *default_func(Menu *curr)
{
    if (!curr)
    {
        fprintf(stdout, "curr is NULL\n");
        return root;
    }
    if (curr->child)
    {
        curr->child->select();
        return curr->child;
    }
    else
        return curr;
}

void Menu::XML2Menu(TiXmlElement *rootElem, Menu *parent, Menu *front)
{
    if (!rootElem->FirstChildElement())
        return;
    const char *attrName, *attrFunc;
    Menu *(*_func)(Menu * curr);
    char *str;
    TiXmlElement *elem;
    Menu *menu;
    for (elem = rootElem->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        attrName = elem->Attribute("name");
        attrFunc = elem->Attribute("func");
        if (funcTable.find(attrFunc) != funcTable.end())
        {
            menu = new Menu((char *)attrName, funcTable[attrFunc], front, parent);
        }
        else
        {
            fprintf(stdout, "xml file error, func %s not found\n", attrFunc);
            exit(-1);
        }

        if (elem == rootElem->FirstChildElement())
        {
            menu->is_head = true;
            if (parent != NULL)
            { //非root第一节点
                parent->child = menu;
                menu->x = parent->x + 1;
                menu->y = parent->y;
            }
            else
            { // root节点
                root = menu;
                // menu->x = 0;
                // menu->y = 0;
            }
        }
        else
        { //非头结点
            front->next = menu;
            menu->x = front->x;
            menu->y = front->y + 1;
        }
        front = menu;
        XML2Menu(elem, menu, NULL);
    }
    if (parent)
    {
        parent->child->front = menu;
        menu->next = parent->child;
    }
    else
    {
        root->front = menu;
        menu->next = root;
        root->select();
    }
}

void Menu::reset(Menu *curr)
{
    if (curr)
    {
        if (curr == root)
            curr->select();
        else
            curr->cancel_select();

        if (curr->child)
            reset(curr->child);
        if (!curr->next->is_head)
            reset(curr->next);
    }
}

void Menu::print_menu(Menu *curr)
{
    if (curr)
    {
        if (curr->is_selected)
            fprintf(stdout, "[%s(%d, %d)]\t", curr->name, curr->x, curr->y);
        else
            fprintf(stdout, "%s(%d, %d)\t", curr->name, curr->x, curr->y);
        if (!curr->next->is_head)
        {
            print_menu(curr->next);
        }
        else
        {
            fprintf(stdout, "\n---------------\n");
        }
        if (curr->is_selected)
        {
            print_menu(curr->child);
        }
    }
}

void kill_process(int signal)
{
    if (root)
    {
        delete root;
        root = NULL;
    }
    run = 0;
    fprintf(stdout, "killed by signal:%d\n", signal);
}

void create_func_table()
{
    class Insert{
    public:
        void operator()(string str, Menu *(*_func)(Menu *)){
            funcTable.insert(pair<string, Menu *(*)(Menu *)>(str, _func));
        }
    }insert;
    // Insert insert;
    insert("default", default_func);
    insert("linkRemote", linkRemote);
    insert("linkProtect", linkProtect);
    insert("linkPMU", linkPMU);
    insert("stateDevice", stateDevice);
    insert("dataYX", dataYX);
    insert("dataYC", dataYC);
    insert("dataYK", dataYK);
    insert("recordSOE", recordSOE);
    insert("recordYK", recordYK);
    insert("recordEvent", recordEvent);
    insert("stateSynchronize", stateSynchronize);
    insert("stateAlarm", stateAlarm);
    insert("stateMain", stateMain);
    insert("version", version);
    insert("setIPconfig", setIPconfig);
    insert("setRouting", setRouting);
    insert("setMAC", setMAC);
    insert("setPassword", setPassword);
    insert("setLCD", setLCD);
    insert("setChannel", setChannel);
    insert("setFuncExit", setFuncExit);
    insert("setClock", setClock);
    insert("testNetwork", testNetwork);
    insert("testSignalOut", testSignalOut);
    insert("testYK", testYK);
    insert("testNumberSet", testNumberSet);
    insert("clearInfo", clearInfo);
    insert("testLCD", testLCD);
    insert("testKey", testKey);
    insert("testLED", testLED);
    insert("testSerialPort232", testSerialPort232);
    insert("testSerialPort232to311", testSerialPort232to311);
    insert("testSerialPort485", testSerialPort485);
    insert("TestCodeB", TestCodeB);
    insert("convertUser", convertUser);
    insert("reboot", reboot);
    insert("poweroff", poweroff);
}

int main()
{
    Menu *curr;
    int key;
    signal(SIGTERM, kill_process);
    signal(SIGINT, kill_process);

    create_func_table();
    TiXmlDocument doc(filename);
    if (!doc.LoadFile(TIXML_DEFAULT_ENCODING))
    {
        fprintf(stdout, "load xml file %s error=%s\n", filename, doc.ErrorDesc());
        exit(1);
    }
    TiXmlElement *xmlRoot = doc.RootElement();

    Menu::XML2Menu(xmlRoot, NULL, NULL);

    Menu::print_menu(root);

    curr = root;
    while (run)
    {
        cin >> key;
        if (key == 111)
            break;
        printf("**********************\n");
        curr = curr->action(key);
        printf("**********************\n");
    }
    if (root)
        delete root;
    return 0;
}