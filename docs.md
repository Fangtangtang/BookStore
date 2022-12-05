# 书店管理系统开发文档
作者：房诗涵

### 程序功能概述
一个书店管理系统，允许用命令行以合法指令交互。

系统包括：账户系统、图书系统、日志系统。

各个系统通过系统对应的合法指令操作。

不同指令用分隔为若干指令。

单个合法指令由ASCII 字符、空格（多个连续空格效果与一个空格等价；行首行末允许出现多余空格；如无特殊说明禁止省略或另增空格）构成。

指令包括：基础指令、帐户系统指令、图书系统指令、日志系统指令。

### 主体逻辑说明
在main函数中通过循环读入指令，main基本不进行具体指令执行，除基础指令quit和
exit。

解析读入指令（类似basic解释器的tokenscanner、parser等，具体的接口还没想好）

根据读入指令的类型由不同的类完成相应操作。

通过文件读写来实现数据的输入输出。

### 代码文件结构
主函数: `main.cpp`
类：
- `account.h` / `account.cpp`
- `loggingStatus.h` / `loggingStatus.cpp`
- `book.h` / `book.cpp`
- `log.h` / `log.cpp`
- `linkList.h` / `linkList.cpp`
文件：账户信息文件，登录状态文件，图书信息文件，日志文件

### 各个类的接口及成员说明
#### 账户
##### 类
基类：`account`
成员：
```c++
struct account {
private:
  int Privilege;
  char UserID [30], Username[30];
  char Password[30], CurrentPassword[30], NewPassword[30];
};
```

##### 指令
- 登录账户 `{0} su [UserID] ([Password])?`

  `void login(std::string &UserID, std::string &Password);`              

- 注销账户 `logout`
`void logout();`
-  注册账户  ` {0} register [UserID] [Password] [Username]`
`void register(std::string &UserID, std::string &Password,std::string & Username);`
- 修改密码  ` {1} passwd [UserID] ([CurrentPassword])? [NewPassword]`
`void changePassword(std::sting&UserID,std::string&CurrentPassword,std::strin&NewPassword);`
- 创建账户   `{3} useradd [UserID] [Password] [Privilege] [Username]`
`void addUser(std::string &UserID, std::string &Password,int Privilege ,std::string & Username); `
- 删除账户   `{7} delete [UserID]`      
`void deleteUser(std::string &UserID)`
`Private:`
- 查找帐户 `account find(std::string UserID); `

#### 登录状态
##### 类
```c++
class loggingStatus{
public：
void logIn(std::string );
void logout(std::string);
void select(std::string);
}
```
#### 图书
##### 类
```c++
class book{
public: 
char ISBN[20]; //合法字符集：除不可见字符以外 ASCII 字符
char BookName[60];// 合法字符集：除不可见字符以外 ASCII 字符
char Author[60];// 合法字符集：除不可见字符以外 ASCII 字符
char Keyword[60];// 合法字符集：除不可见字符以外 ASCII 字符 
//内容以 | 为分隔可以出现多段信息
int Quantity;// 数值不超过 2'147'483'647
double Price; //浮点数输入输出精度固定为小数点后两位
double TotalCost// 浮点数输入输出精度固定为小数点后两位
```
##### 指令
`public：`
- 购买图书：`buy [ISBN] [Quantity]`
`void buy(std::string &ISBN,int Quantity);`
- 选择图书：`{3} select [ISBN]`
`void select(std::string &ISBN);`
- 修改图书信息：`{3} modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+`
`void changeInformation(std::string &ISBN, std::string & BookName, std::string & Author,std::string Keyword,double Price);`
- 图书进货：`{3} import [Quantity] [TotalCost]`
`void inport(std::string &ISBN,int Quantity,int TotalCost)`

`private:`
`book find(std::string ISBN);`

#### 日志
##### 类
`log`
##### 指令
- 记录一条日志
```c++
struct record{
std::string UserID;
std::string command;
 }
private:
int count;
long long sum;
public:
//财务记录查询：
void showFinance(int count);
//生成日志：
void addlog(record)
```
#### 块状链表类
`linkList`
辅助信息存取,会用于图书信息等的维护）
```c++
struct blockNode{
blockNode *pre；
blockNode *next；
someType Array[block_size];
int size;
int max;
int min;
}
```

### 文件存储说明
### 其他补充说明




