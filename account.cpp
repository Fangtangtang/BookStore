//
// Created by 房诗涵 on 2022/12/10.
//
#include "account.h"
#include "linkList.h"
#include <string>
#include <cstring>

//权限 符号常量 对应{0} {1} {3} {7}
enum Privilege{visitor,customer,clerk,host};

//默认构造：全空
ID::ID() {
    memset(userID,0, sizeof(userID));
}

//由char*初始化
ID::ID(char* id) {
    strcpy(userID,id);
}

bool ID::operator>(const ID &id) const {
    return strcmp(userID,id.userID)>0;
}

bool ID::operator==(const ID &id) const {
    return !strcmp(userID,id.userID);
}

bool ID::operator>=(const ID &id) const {
    return strcmp(userID,id.userID)>=0;
}

ID &ID::operator=(const ID &id) = default;

ID::~ID() = default;


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool account::operator>(const account &id) const {
    ID id1 = GetKey();
    ID id2 = id.GetKey();
    if (id1 > id2) return true;
    else return false;
}

ID account::GetKey() const {
    return userID;
}

AccountManager::AccountManager() {

}
void AccountManager::AddUser(std::string &UserID, std::string &Password, int Privilege, std::string &Username) {
    //create an account
    account userAccount;
    userAccount.privilege = Privilege;
    StringToChar(Username, userAccount.name);
    ID userID;
    StringToChar(UserID, userID.userID);
    userAccount.userID = userID;
    StringToChar(Password, userAccount.password);
    //insert
    accountList.Insert(userID,userAccount);
}

void AccountManager::DeleteUser(std::string &UserID) {
     accountList.Erase(UserID);
}


account AccountManager::FindAccount(const std::string &UserID) {
    return accountList.FindKey(UserID);
}
