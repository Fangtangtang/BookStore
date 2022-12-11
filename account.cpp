//
// Created by 房诗涵 on 2022/12/10.
//
#include "account.h"

#include <utility>

ID::ID() {
    userID[0] = '\0';//empty string
}

ID::ID(std::string id) {
    for (int i = 0; i < id.size(); ++i)
        userID[i] = id[i];
    userID[id.size()] = '\0';
}

bool ID::operator>(const ID &id) const {
    for (int i = 0;; ++i) {
        if (userID[i] == '\0')return false;
        if (id.userID[i] == '\0')return true;
        if (userID[i] > id.userID[i])return true;
    }
}

bool ID::operator==(const ID &id) const {
    int i = 0;
    while (userID[i] != '\0') {
        if (userID[i] != id.userID[i])return false;
        ++i;
    }
    return true;
}

bool ID::operator>=(const ID &id) const {
    if ((*this > id) || (*this == id))return true;
    else return false;
}

ID &ID::operator=(const ID &id) = default;

ID::~ID() = default;

bool account::operator>(const account &id) const {
    ID id1 = GetKey();
    ID id2 = id.GetKey();
    if (id1 > id2) return true;
    else return false;
}

ID account::GetKey() const {
    return userID;
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

void AccountManager::StringToChar(std::string str, char *ch) {
    int i;
    for (i = 0; i < str.size(); ++i) {
        ch[i] = str[i];
    }
    ch[i] = '\0';
}