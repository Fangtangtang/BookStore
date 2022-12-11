//
// Created by 房诗涵 on 2022/12/10.
//
//重载key > >= == =
//someType >
//函数 GetKey
#ifndef BOOKSTORE_ACCOUNT_H
#define BOOKSTORE_ACCOUNT_H

#include "linkList.h"
#include <string>
//key:ID
struct ID {
    char userID[31];

    ID();

    ID(std::string id);

    ~ID();

    bool operator>(const ID &id) const;

    bool operator==(const ID &id) const;

    bool operator>=(const ID &id) const;

    ID &operator=(const ID &id);

};

struct account {
    int privilege;
    ID userID;
    char name[31];
    char password[31];

    bool operator>(const account &id) const;

    //获取key
    ID GetKey() const;

    //修改密码
    void ChangePassword(std::string &CurrentPassword,std::string &NewPassword);

};

class AccountManager{
public:
    //登录账户 {0} register [UserID] [Password] [Username]
    void Register(std::string &UserID,std::string &Password,std::string & Username);

    //创建帐户 {3} useradd [UserID] [Password] [Privilege] [Username]
    void AddUser(std::string &UserID, std::string &Password,int Privilege ,std::string & Username);

    //删除账户 {7} delete [UserID]
    void DeleteUser(std::string &UserID);

    //修改密码 {1} passwd [UserID] ([CurrentPassword])? [NewPassword]
    void ChangePassword(std::string&UserID,std::string&CurrentPassword,std::string&NewPassword);

private:
    //储存账户信息的块状链表
    //类中用（）会和声明函数歧义
    LinkList<ID,account> accountList{"account_information",sizeof(account)};

    std::fstream io_account_information;

//    std::fstream io_account_index;

    //查找帐户
    account FindAccount(const std::string &UserID);

    //转格式
    void StringToChar(std::string str,char* ch);
};
#endif //BOOKSTORE_ACCOUNT_H
