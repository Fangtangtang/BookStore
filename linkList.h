//
// Created by 房诗涵 on 2022/12/10.
//
//对Array中的struct重载大比较运算符，可直接将插入元素和Array中元素比
//重载> != =
//函数 GetIndex
#ifndef BOOKSTORE_LINKLIST_H
#define BOOKSTORE_LINKLIST_H

#include <fstream>

template<class someType>//Array 类型
class LinkList {

public:
    //构造函数
    //file_name:文件名
    //typeSize：someType占空间(byte)大小,由此决定block_size
    //1.将LinkList与对应的文件和someType大小关联
    //2.构造list头节点，写入文件
    LinkList(const std::string &file_name, int typeSize);

    ~LinkList();

    //插入元素
    void Insert(int index, const someType &ele);

    //删去元素
    void Erase(int index);

    //寻找元素
    someType FindIndex(int index);

private:
    //a node in LinkList
    struct BlockNode {
        //链接
        long pre = 0;
        long next = 0;

        //块状链表基础参数
        int size = 0;//元素个数
        int max = 0;//区间上界
        int min = 0;//区间下界
        static int block_size;

        //储存内容
        someType Array[block_size];

        //返回找到的ele
        someType Find(int index);

        //向BlockNode的Array插入一个元素
        //需要裂块 返回true
        bool Insert(int index, const someType &ele);

        //从BlockNode的Array删除一个元素
        bool Erase(int index);
    };

    //empty head
    struct HeadNode {
        long pre;
        long next;
    };

    long head, rear;

    //LinkList读写的文件流对象 在LinkList构造时和相关文件关联
    std::fstream r_w_LinkList;

    //返回iter指向节点
    BlockNode ReadNode(long iter);

    //修改blockNode信息，覆盖原信息
    void WriteNode(const BlockNode &blockNode, const long &iter);

    //返回index所在的node
    BlockNode FindNode(int index, long &iter);

    //在文件末开一个新node
    long GetNode();

    //裂块
    void BreakNode(const long &iter, BlockNode &blockNode);

    //并块
    void CombineNode(const long &iter, BlockNode &blockNode1,BlockNode &blockNode2);
};

#endif //BOOKSTORE_LINKLIST_H
