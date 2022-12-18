#include <iostream>
#include <fstream>
#include "linkList.h"
#include "account.h"
#include <string>
int main() {
    AccountManager a;
    std::string cmd;
    std::string ID,name,password;//ID->index
    int pri;
    std::cin>>ID>>name>>password>>pri;
    a.AddUser(ID,password,pri,name);
    a.DeleteUser(ID);
    std::cout<<1;
    return 0;
}
