# 书店管理系统开发文档
作者：房诗涵

### 程序功能概述
一个书店管理系统，允许用命令行以合法指令交互。

系统包括：账户系统、图书系统、日志系统。
#### 交互方式

各个系统通过系统对应的合法指令操作。

输入数据以换行符或回车符为分隔为若干指令。

单个合法指令由ASCII 字符、空格（多个连续空格效果与一个空格等价；行首行末允许出现多余空格；如无特殊说明禁止省略或另增空格）构成。

指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配。

在用户输入一条指令后，如果合法则执行对应操作，如果有则输出操作结果；如果指令非法或操作失败则输出`Invalid\ `。仅有空格的指令合法且无输出内容。

除非有特殊说明，如果输入指令对应的输出内容非空，则结尾有 `\n ` 字符；输出内容为空则不输出任何字符。



合法指令要求：

- `[x]` 表示一串有特定限制的用户自定义字符串；
- `(a|b|c) `表示此处仅能出现`a`,`b`,`c`中其一；
-  `(x)?` 表示此处可以出现零次或一次 `x`；
- `(x)+` 表示此处可以出现一次或多次 `x`。



指令包括：

- 基础指令
`quit`正常退出系统
`exit` 正常退出系统
- 帐户系统指令
`su [UserID] ([Password])?`
`logout`
`register [UserID] [Password] [Username]`
`passwd [UserID] ([CurrentPassword])? [NewPassword]`
`useradd [UserID] [Password] [Privilege] [Username]`
`delete [UserID]`
- 图书系统指令
`show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?`
`buy [ISBN] [Quantity]`
`select [ISBN]`
`modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+`
`import [Quantity] [TotalCost]`
- 日志系统指令
`show finance ([Count])?`
`log`



#### 系统功能
##### 账户系统

书店管理系统服务于 店主（超级管理员），员工（管理员）以及顾客（用户）。


为满足其不同需求，故需帐户系统管理帐户及权限数据，并提供相应功能。
###### 相关说明

**权限**

帐户的权限等级有 {7}, {3} 和 {1}；未登录任何帐户的状态下当前帐户权限视为 {0}。

高权限的帐户可以执行需要低权限的指令，反之则不可。

- **店主{7}：**使用 `root` 帐户，可以访问书店系统所有功能，包括日志和帐户管理功能
- **员工{3}：**可以访问修改图书数据相关功能等
- **顾客{1}：**可以注册帐户，查询、购买图书
- **游客{0}：**可以注册帐户

**账户信息**
账户名、账户ID、权限等级、密码、登录状态

**账户操作**
- **登录账户：**使登录帐户变为已登录状态，当前帐户变为该帐户；密码错误则操作失败；如果当前帐户权限等级高于登录帐户则可以省略密码。
- **注销账户：**撤销最后一次成功执行的`su` 指令效果；如无已登录帐户则操作失败。
- **注册账户：**如果 `[UserID] `与已注册帐户重复则操作失败。
- **修改密码：**修改指定帐户的密码；帐户不存在则操作失败；` [CurrentPassword]`错误则操作失败；如果当前帐户权限等级为 {7} 则可以省略` [CurrentPassword]`。
- **创建账户：**如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败；如果 `[UserID]`与已注册帐户重复则操作失败。
- **删除账户：**删除指定帐户；如果待删除帐户不存在则操作失败；如果待删除帐户已登录则操作失败。

**其他**
系统支持嵌套登录，即允许多个帐户同时处于登录状态

允许同一帐户同时多次登录

输入的指令视为最后登录的帐户操作

退出系统视为登出所有已登录帐户



##### 图书系统
图书系统提供图书信息、库存信息和图书交易财务记录的相关服务及数据存储功能。

###### 相关说明

**图书信息**
ISBN、BookName、Author、Keyword、Quantity、Price、TotalCast

**图书操作**
- **检索图书：**以 `[ISBN]`字典升序依次输出满足要求的图书信息；无满足要求的图书时输出空行；无附加参数时，所有图书均满足要求；附加参数内容为空、`[Keyword] `中出现多个关键词则操作失败。每个图书信息输出格式为`[ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[库存数量]\n`

- **购买图书：**购买指定数量的指定图书，以浮点数输出购买图书所需的总金额。没有符合条件的图书、购买数量为非正整数则操作失败。

- **选择图书：**以当前帐户选中指定图书（帐户登出后无需保存选中图书情况）。没有符合条件的图书则创建仅拥有 [ISBN] 信息的新图书；退出系统视为取消选中图书。

- **修改图书信息：**以指令中的信息更新选中图书的信息。如未选中图书、附加参数内容为空、`[keyword]` 包含重复信息段则操作失败；有重复附加参数为非法指令；不允许将 ISBN 改为原有的 ISBN。

- **图书进货：**以指定交易总额购入指定数量的选中图书。如未选中图书、购入数量为非正整数、交易总额为非正数则操作失败。

  

##### 日志系统

日志系统负责记录书店管理系统运行过程中的各种数据，提供各类日志信息查询服务

###### 相关说明

**日志信息**
Count：交易笔数

**日志操作**
- **财务记录查询：**输出最后完成的指定笔数交易总额。无` Count` 参数时，输出所有交易之总额；不存在交易时认为收入支出均为 `0.00`;`Count` 为 0 时输出空行；`Count` 大于历史交易总笔数时操作失败。格式为` + [收入] - [支出]\n`

- **生成日志：**生成日志记录。内容包括系统操作者行为、每一笔交易情况等。



### 主体逻辑说明

在main函数中通过循环读入指令，main基本不进行具体指令执行，除基础指令quit和
exit。

解析读入指令（类似basic解释器的tokenscanner、parser等，具体的接口还没想好）

根据读入指令的类型由不同的类完成相应操作。

通过文件读写来实现数据的输入输出。

### 代码文件结构
主函数: `main.cpp`

类：

- 账户类：`account.h` / `account.cpp`

- 登录状态类：`loggingStatus.h` / `loggingStatus.cpp`

- 图书类：`book.h` / `book.cpp`

- 日志类：`log.h` / `log.cpp`

- 块状链表类：`linkList.h` / `linkList.cpp`

- 输入扫描类：`tokenscanner.h`/`tokenscanner.cpp`

- 命令解析类：`parser.h`/`paeser.cpp`

文件：
- 账户信息相关文件
- 图书信息文件
- 日志文件：财务日志文件、操作日志文件

### 各个类的接口及成员说明

#### 账户

**`Account`类**
成员：
```c++
//单个账户
//作为块状链表accontList的BlockNode的元素
struct Account {
  int privilege;
  char ID [30], name[30];
  char password[30];
};
class AccountManager {

}
```

##### 指令
```c++
public:
//登录账户 {0} su [UserID] ([Password])?
  void Login(std::string &UserID, std::string &Password);         
//注销账户 logout
  void Logout();
//注册账户 {0} register [UserID] [Password] [Username]
  void Register(std::string &UserID,std::string &Password,std::string & Username);
//修改密码 {1} passwd [UserID] ([CurrentPassword])? [NewPassword]
  void ChangePassword(std::sting&UserID,std::string&CurrentPassword,std::strin&NewPassword);
//创建帐户 {3} useradd [UserID] [Password] [Privilege] [Username]
  void AddUser(std::string &UserID, std::string &Password,int Privilege ,std::string & Username);
//删除账户 {7} delete [UserID] 
  void DeleteUser(std::string &UserID)

private:
//查找帐户 
  account find(std::string UserID);
```
#### 登录状态
##### 类
```c++
class loggingStatus{
public：
  void LogIn(std::string );
  void Logout(std::string);
  void Select(std::string);
}
```
#### 图书
##### 类
```c++
class book{
public: 
  char ISBN[20]; //合法字符集：除不可见字符以外 ASCII 字符
  char name[60];// 合法字符集：除不可见字符以外 ASCII 字符
  char author[60];// 合法字符集：除不可见字符以外 ASCII 字符
  char keyword[60];// 合法字符集：除不可见字符以外 ASCII 字符 
//内容以 | 为分隔可以出现多段信息
  int quantity;// 数值不超过 2'147'483'647
  double price; //浮点数输入输出精度固定为小数点后两位
  double totalCost// 浮点数输入输出精度固定为小数点后两位
```
##### 指令
```c++
public:
//购买图书 buy [ISBN] [Quantity]
  void Buy(std::string &ISBN,int Quantity);
//选择图书 {3} select [ISBN]
  void Select(std::string &ISBN);
//修改图书信息 {3} modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
  void ChangeInformation(std::string &ISBN, std::string & BookName, std::string & Author,std::string Keyword,double Price);
//图书进货 {3} import [Quantity] [TotalCost]
  void Inport(std::string &ISBN,int Quantity,int TotalCost)

private:
  book Find(std::string ISBN);
```
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

public:
//财务记录查询：
  void ShowFinance(int count);
//生成日志：
  void Addlog(record)

private:
  int count;
  long long sum;
```
#### 块状链表
**`LinkList`类**
说明：辅助信息存取,用于账户信息、图书信息的维护。
```c++
struct BlockNode{
  //链接
  blockNode *pre=nullptr;
  blockNode *next=nullptr;
  //储存内容
  someType Array[block_size];
  //块状链表基础参数
  int size;
  int max;
  int min;
  
  //向BlockNode的Array插入一个元素
  void insert(int index);
}

class LinkList{  
public:
  LinkList();

  ~LinkList();
  
  //插入元素
  void Insert(int index);
  //删去元素
  void Erase(int index);
  //寻找元素
  someType FindIndex(int index);
}
private:
  BlockNode *head,*rear;
```

### 文件存储说明
### 其他补充说明




