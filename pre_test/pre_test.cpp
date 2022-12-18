//存储用 块状链表
#ifndef LINKLIST_H
#define LINKLIST_H

#include <fstream>
#include<iostream>

//LinkList类 模板
//someType 插入元素类型
template<class index,class someType>
class LinkList {
    static constexpr int blockSize = 1024;
public:
    LinkList(const std::string &file_name) {

        r_w_LinkList.open(file_name);

//        r_w_LinkList.open("1", std::fstream::trunc);

        if (!r_w_LinkList.good()) {
            //create headNode

            r_w_LinkList.open(file_name, std::ios::out);
            r_w_LinkList.close();
            r_w_LinkList.open(file_name);

//            r_w_LinkList.open("1", std::ios::out);
//            r_w_LinkList.close();
//            r_w_LinkList.open("1");

            headNode1.next = 0;
            r_w_LinkList.seekp(0);//将指针定位到文件开头
            head = r_w_LinkList.tellp();
            //从文件开头写HeadNode
            r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
        }
        r_w_LinkList.seekg(head);
        r_w_LinkList.read(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
    }

    ~LinkList() = default;

    void printList() {
        BlockNode blockNode;
        long iter = headNode1.next;//第一个节点
        while (iter != 0) {
            std::cout << "####\n";
            blockNode = ReadNode(iter);
            for (int i = 0; i < blockNode.NodeHead.size; ++i)
                blockNode.Array[i].print();
            std::cout << '\n';
            iter = blockNode.NodeHead.next;
        }
    }

    void Insert(someType ele) {
        bool break_flag = false;//是否break
        bool insert_flag = false;//是否已find
        BlockNode blockNode;
        Head head1, head2;
        long iter = headNode1.next;
        //第一次插元素
        if (headNode1.next == 0) {
            r_w_LinkList.seekp(0, std::ios::end);//the end of the file
            iter = r_w_LinkList.tellp();
            blockNode.NodeHead.pre = head;
            headNode1.next = iter;
            blockNode.Insert(ele);//不可能裂块
            r_w_LinkList.seekp(head);
            r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
            WriteNode(blockNode, iter);
        }
            //之后插元素
        else {
            head2 = ReadHead(iter);
            if (head2.max > ele) {
                blockNode = ReadNode(iter);
                blockNode.Insert(ele);
                insert_flag = true;
                if (blockNode.NodeHead.size == blockSize)break_flag = true;
                if (break_flag) {
                    BreakNode(iter, blockNode);
                }
                WriteNode(blockNode, iter);
            }
            if (!insert_flag) {
                iter = head2.next;
                while (iter != 0) {//向后遍历
                    head1 = head2;
                    head2 = ReadHead(iter);
                    if (ele >= head1.min && head2.min >= ele) {
                        if (head1.max > ele) {
                            iter = head2.pre;
                            blockNode = ReadNode(iter);
                            blockNode.Insert(ele);//插入当前块的array
                            if (blockNode.NodeHead.size == blockSize)break_flag = true;
                            if (break_flag) {
                                BreakNode(iter, blockNode);
                            }
                            WriteNode(blockNode, iter);
                        }
                        if (ele > head1.max) {
                            blockNode = ReadNode(iter);
                            blockNode.Insert(ele);//插入后一块的array
                            if (blockNode.NodeHead.size == blockSize)break_flag = true;
                            if (break_flag) {
                                BreakNode(iter, blockNode);
                            }
                            WriteNode(blockNode, iter);
                        }
                        break;
                    }
                    iter = head2.next;
                }
                if (iter == 0) {
                    //曾对尾插做过一些愚蠢的多余特判 导致会在same index顺序插入时不断加新块！！！！
                        if (head1.next != 0)iter = head1.next;
                        else iter = headNode1.next;
                        blockNode = ReadNode(iter);
                        blockNode.Insert(ele);//插入后一块的array
                        if (blockNode.NodeHead.size == blockSize)break_flag = true;
                        if (break_flag) BreakNode(iter, blockNode);
                        WriteNode(blockNode, iter);
                }
            }
        }
    }

    //Delete
    bool Delete(const someType &ele) {
        long iter = headNode1.next;
        BlockNode blockNode;
        Head head1;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (ele >= head1.min && head1.max >= ele)break;
            iter = head1.next;
        }
        if (iter == 0) return false;
        blockNode = ReadNode(iter);
        bool b = blockNode.Delete(ele);
        if (!b) return false;
        if (blockNode.NodeHead.size < blockSize / 2) {
            Head preHead, nextHead;
            BlockNode preNode, nextNode;
            if (blockNode.NodeHead.pre != 0) {
                preHead = ReadHead(blockNode.NodeHead.pre);
            }
            if (blockNode.NodeHead.next != 0) {
                nextHead = ReadHead(blockNode.NodeHead.next);
            }
            if (blockNode.NodeHead.size == 0) {
                if (blockNode.NodeHead.pre == 0) {
                    headNode1.next = blockNode.NodeHead.next;
                    r_w_LinkList.seekp(head);
                    r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
                    if (blockNode.NodeHead.next!=0){
                        nextHead.pre=head;
                        WriteHead(nextHead,blockNode.NodeHead.next);
                    }
                }else{
                    preHead.next = blockNode.NodeHead.next;
                    WriteHead(preHead, blockNode.NodeHead.pre);
                    if (blockNode.NodeHead.next != 0) {
                        nextHead.pre = blockNode.NodeHead.pre;
                        WriteHead(nextHead, blockNode.NodeHead.next);
                    }
                }
                return true;
            }
            if (preHead.size > blockSize / 2) {
                preNode = ReadNode(blockNode.NodeHead.pre);
                blockNode.Insert(preNode.NodeHead.max);
                preNode.Delete(preNode.NodeHead.max);
                WriteNode(preNode, blockNode.NodeHead.pre);
                WriteNode(blockNode, iter);
                return true;
            }
            if (nextHead.size > blockSize / 2) {
                nextNode = ReadNode(blockNode.NodeHead.next);
                blockNode.Insert(nextNode.NodeHead.min);
                nextNode.Delete(nextNode.NodeHead.min);
                WriteNode(nextNode, blockNode.NodeHead.next);
                WriteNode(blockNode, iter);
                return true;
            }
            if (blockNode.NodeHead.next != 0) {
                nextNode = ReadNode(blockNode.NodeHead.next);
                preNode = ReadNode(blockNode.NodeHead.pre);
                CombineNode(preNode, blockNode, nextNode);
                return true;
            }
        }
        WriteNode(blockNode, iter);
        return true;
    }

    void FindPrint(const index &index1){
        long iter = headNode1.next;
        Head head1;
        while (iter != 0) {
            head1 = ReadHead(iter);
            if (index1 >= head1.min.GetIndex() && head1.max.GetIndex() >= index1)break;
            iter = head1.next;
        }
        if(iter==0){
            std::cout<<"null";
            return ;
        }
        BlockNode blockNode= ReadNode(iter);
        int i=0;
        while (!(index1==blockNode.Array[i].GetIndex())){
            ++i;
            if(i==blockNode.NodeHead.size) {
                std::cout<<"null";
                return;//没有元素
            }
        }
        while(true){
            std::cout<<blockNode.Array[i].GetVal()<<" ";
            ++i;
            if(i==blockNode.NodeHead.size){//结束一个块
                blockNode= ReadNode(blockNode.NodeHead.next);
                i=0;
            }
            if(!(index1==blockNode.Array[i].GetIndex())) return;//找完
        }
    }

private:

    //块链节点头部
    //前驱后继 元素数 上下界
    struct Head {
        long pre = 0;
        long next = 0;
        int size = 0;
        someType max;
        someType min;
    };

    //头节点
    struct HeadNode {
        long next = 0;
    };

    //数据块
    struct BlockNode {
        Head NodeHead;
        someType Array[blockSize];

        //Insert
        void Insert(const someType &ele) {
            int i = 0;
            while (ele > Array[i] && i < NodeHead.size) {
                ++i;
            }
            if (i < NodeHead.size && ele == Array[i]) return;
            int move = NodeHead.size;
            while (move != i) {
                Array[move] = Array[move - 1];
                --move;
            }
            Array[i] = ele;
            ++NodeHead.size;
            if (i == 0) NodeHead.min = ele;
            if (i == NodeHead.size - 1) NodeHead.max = ele;
        }

        //Delete
        bool Delete(const someType&ele){
            int i=0;
            while(i < NodeHead.size) {
                if(Array[i]==ele) break;
                ++i;
            }
            if(i==NodeHead.size) return false;
            --NodeHead.size;
            for (; i < NodeHead.size; ++i) {
                Array[i] = Array[i + 1];
            }
            someType eleNull;
            Array[NodeHead.size] = eleNull;
            NodeHead.min = Array[0];
            NodeHead.max = Array[NodeHead.size - 1];
            return true;
        }
    };

    HeadNode headNode1;
    long head;//头节点位置

    //LinkList读写的文件流对象 在LinkList构造时和相关文件关联
    std::fstream r_w_LinkList;

    //Read iter指向块链节点
    BlockNode ReadNode(long iter) {
        BlockNode blockNode;
        r_w_LinkList.seekg(iter);
        r_w_LinkList.read(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
       return blockNode;
    }

    //Read iter指向块链的头部
    Head ReadHead(long iter) {
        Head nodeHead;
        r_w_LinkList.seekg(iter);
        r_w_LinkList.read(reinterpret_cast<char *> (&nodeHead), sizeof(Head));
        return nodeHead;
    }

    //Write 块链信息
    void WriteNode(BlockNode blockNode, const long &iter) {
        r_w_LinkList.seekp(iter);
        r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    }

    //Write 块链头部信息
    void WriteHead(Head nodeHead, const long &iter) {
        r_w_LinkList.seekp(iter);
        r_w_LinkList.write(reinterpret_cast<char *> (&nodeHead), sizeof(Head));
    }

    //BreakNode
    void BreakNode(const long &iter, BlockNode &blockNode) {
        //获取新空间
        r_w_LinkList.seekp(0, std::ios::end);//the end of the file
        long newIter = r_w_LinkList.tellp();
        BlockNode newBlock;
        someType ele;
        for (int i = blockSize / 2; i < blockSize; ++i) {
            newBlock.Array[i - blockSize / 2] = blockNode.Array[i];
            blockNode.Array[i] = ele;
        }
        blockNode.NodeHead.size = newBlock.NodeHead.size = blockSize / 2;
        newBlock.NodeHead.max = blockNode.NodeHead.max;
        blockNode.NodeHead.max = blockNode.Array[blockSize / 2 - 1];
        newBlock.NodeHead.min = newBlock.Array[0];
        newBlock.NodeHead.next = blockNode.NodeHead.next;
        newBlock.NodeHead.pre = iter;
        blockNode.NodeHead.next = newIter;
        if (newBlock.NodeHead.next != 0) {
            Head head1 = ReadHead(newBlock.NodeHead.next);
            head1.pre = newIter;
            WriteHead(head1, newBlock.NodeHead.next);
        }
        WriteNode(newBlock, newIter);
    }

    //CombineNode并入后一块
    void CombineNode(BlockNode &pre, BlockNode &blockNode, BlockNode &next) {
        for (int i = next.NodeHead.size; i >= 0; --i) {
            next.Array[blockNode.NodeHead.size + i] = next.Array[i];
        }
        for (int i = 0; i < blockNode.NodeHead.size; ++i) {
            next.Array[i] = blockNode.Array[i];
        }
        next.NodeHead.size += blockNode.NodeHead.size;
        next.NodeHead.min = blockNode.NodeHead.min;
        if (blockNode.NodeHead.pre != 0) {
            pre.NodeHead.next = blockNode.NodeHead.next;
            next.NodeHead.pre = blockNode.NodeHead.pre;
            WriteNode(pre, next.NodeHead.pre);
            WriteNode(next, pre.NodeHead.next);
        } else {
//            HeadNode headNode;
            headNode1.next = blockNode.NodeHead.next;
            next.NodeHead.pre = head;
            WriteNode(next, headNode1.next);
            r_w_LinkList.seekp(head);
            r_w_LinkList.write(reinterpret_cast<char *> (&headNode1), sizeof(HeadNode));
        }
    }
};

#endif

//元素类 对应someType
#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <cstring>

struct Index {
    char index[65];

    Index() {
        memset(index, 0, sizeof(index));
    }

    Index(char* id) {
       strcpy(index,id);
    }

    ~Index() = default;

    std::string ToString() const {
        std::string str = index;
        return str;
    }

    bool operator>(const Index &id) const {
        return strcmp(index,id.index)>0;
    }

    bool operator==(const Index &id) const {
        return !strcmp(index,id.index);
    }

    bool operator>=(const Index &id) const {
        return strcmp(index,id.index)>=0;
    }

    Index &operator=(const Index &index1) = default;

    friend std::ostream &operator<<(std::ostream &os, const Index &id) {
        std::string str = id.ToString();
        os << str;
        return os;
    }
};

struct Account {
    Index index;
    int value = 0;

    //GetIndex
    Index GetIndex() {
        return index;
    }
    //GetVal
    int GetVal() {
        return value;
    }

    void print() const {
        int i = 0;
        while (index.index[i] != '\0') {
            std::cout << index.index[i];
            ++i;
        }
        std::cout << " " << value << "\n";
    }

    bool operator>(const Account &id) const {
        if (index > id.index || (index == id.index && value > id.value)) return true;
        else return false;
    }

    bool operator==(const Account &account) const {
        if ( account.value == value&&account.index == index )return true;
        else return false;
    }

    bool operator>=(const Account &account) const {
        return !(account>*this);
    }

    Account &operator=(std::pair<Account, bool> account) {
        *this = account.first;
        return *this;
    }
};

class AccountManager {
public:

    void Print() {
        accountList.printList();
    }

    void Insert(char* index, int value) {
        Index index1(index);
        Account ele;
        ele.index = index1;
        ele.value = value;
        accountList.Insert(ele);
    }

    bool Delete(char* index, int value) {
        Index index1(index);
        Account ele;
        ele.index = index1;
        ele.value = value;
        bool delete_flag = accountList.Delete(ele);
        return delete_flag;
    }

    void Find(char* index){
        Index index1(index);
        accountList.FindPrint(index1);
    }

private:

    LinkList<Index,Account> accountList{"information"};

    std::fstream io_account_information;

};

#endif
using namespace std;

int main() {
//    freopen("/dev/null", "w", stdout);
//    freopen("my.out", "w", stdout);
    string cmd;
    char id[65];
    AccountManager *accountManager = new AccountManager;
    int value;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "insert") {
            cin >> id >> value;
            accountManager->Insert(id, value);
       }
        if (cmd == "delete") {
            cin >> id >> value;
            accountManager->Delete(id, value);
        }
        //index-based find
        if (cmd == "find") {
            cin >> id;
            accountManager->Find(id);
            std::cout << "\n";
        }
    }
//    accountManager->Print();
    delete accountManager;
    return 0;
}