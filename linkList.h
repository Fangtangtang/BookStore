//
// Created by 房诗涵 on 2022/12/10.
//
//对Array中的struct重载大比较运算符，可直接将插入元素和Array中元素比
//直接以ID等重载作为index
#ifndef BOOKSTORE_LINKLIST_H
#define BOOKSTORE_LINKLIST_H

#include <fstream>

template<class key, class someType>//Array 类型
class LinkList {

public:
    //构造函数
    //file_name:文件名
    //typeSize：someType占空间(byte)大小,由此决定block_size
    //1.将LinkList与对应的文件和someType大小关联
    //2.构造list头节点，写入文件

    LinkList(const std::string &file_name) {
        //relate fstream with the file
        r_w_LinkList.open(file_name);
        //create headNode
        HeadNode headNode;
        headNode.pre = headNode.next = 0;//空：相对头的偏移量为0
        r_w_LinkList.seekp(0);//将指针定位到文件开头
        rear = head = r_w_LinkList.tellp();
        //从文件开头写HeadNode
        r_w_LinkList.write(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
    }

    ~LinkList() = default;

    //插入元素
    void Insert(key key1, const someType &ele) {
        //find the node
        //insert into the array
        long iter = head;
        bool break_flag;
        r_w_LinkList.seekg(iter);//HeadNode位置
        HeadNode headNode;
        BlockNode blockNode1, blockNode2;
        r_w_LinkList.read(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
        iter = headNode.next;//第一个节点
        //in a node
        while (iter != 0) {
            blockNode1 = blockNode2;
            blockNode2 = ReadNode(iter);
            if (key1 > blockNode1.min && blockNode2.min > key1) {
                if (blockNode1.max > key1) {
                    blockNode1.Insert(key1, ele);//插入当前块的array
                    if(blockNode1.size ==block_size)break_flag= true;
                    iter = blockNode2.pre;
                    if (break_flag) BreakNode(iter, blockNode1);
                    WriteNode(blockNode1, iter);
//                r_w_LinkList.seekp(iter);
//                r_w_LinkList.write(reinterpret_cast<char *> (&blockNode1), sizeof(BlockNode));
                } else {
                    blockNode2.Insert(key1, ele);//插入后一块的array
                    if(blockNode2.size ==block_size)break_flag= true;
                    if (break_flag) BreakNode(iter, blockNode2);
                    WriteNode(blockNode2, iter);
//                r_w_LinkList.seekp(iter);
//                r_w_LinkList.write(reinterpret_cast<char *> (&blockNode2), sizeof(BlockNode));
                }
                break;
            }
            iter = blockNode2.next;
        }
        if (iter == 0) {
            //new node needed
            r_w_LinkList.seekp(0, std::ios::end);//the end of the file
            iter = r_w_LinkList.tellp();
            BlockNode blockNode;
            blockNode.pre = blockNode1.next;
            blockNode2.next = iter;
            rear = iter;
            blockNode.Insert(key1, ele);//不可能裂块
            WriteNode(blockNode, iter);
//        r_w_LinkList.seekp(iter);
//        r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
        }
    }

    //删去元素
    void Erase(key key1) {
        //find the ele
        long iter;//the pos of the node
        BlockNode blockNode = FindNode(key1, iter);
        blockNode.Erase(key1);//是否混合
        bool combine_flag ;
        if(blockNode.size<block_size/2)combine_flag= true;
        if (combine_flag) {
//            int blockSize = blockNode.block_size;
            BlockNode preNode = ReadNode(blockNode.pre);
            BlockNode nextNode = ReadNode(blockNode.next);
            if (preNode.size > block_size / 2) {
                //update the array
                someType borrowedEle = preNode.Find(preNode.max);
                blockNode.Insert(preNode.max, borrowedEle);
                preNode.Erase(preNode.max);
                return;
            }
            if (nextNode.size > block_size / 2) {
                someType borrowedEle = nextNode.Array[0];
                blockNode.Insert(nextNode.min, borrowedEle);
                nextNode.Erase(nextNode.min);
                return;
            }
            //选择后一个并块
            CombineNode(iter, blockNode, nextNode);
        }
    }

    //寻找元素
    someType FindKey(key key1) {
        long iter;
        BlockNode blockNode = FindNode(key1, iter);
        someType ele = blockNode.Find(key1);
        return ele;
    }

private:
    //a node in LinkList
    struct BlockNode {
        //链接
        long pre = 0;
        long next = 0;

        //块状链表基础参数
        int size = 0;//元素个数
        key max;//区间上界
        key min;//区间下界
//        const static int block_size=4096/sizeof(someType);

        //储存内容
        someType Array[4096 / sizeof(someType)];

        //返回找到的ele
        someType Find(key key1) {
            int i = 0;
            while (!(Array[i].GetKey() == key1))++i;
            return Array[i];
        }

        //向BlockNode的Array插入一个元素
        //需要裂块 返回true
        void Insert(key key1, const someType &ele) {
            //find the pos
            int i = 0, move = size;
            //已经有元素
            if (size) {
                while (ele > Array[i])++i;//重载someType比较运算符，以index为标准
                while (move != i) {
                    Array[move] = Array[move - 1];
                    --move;
                }
            }
            //insert the ele
            Array[i] = ele;
            ++size;
                //update
            min = Array[0].GetKey();
            max = Array[size - 1].GetKey();
        }


        //从BlockNode的Array删除一个元素
        void Erase(key key1) {
            int i = 0;
            while (!(Array[i].GetKey() == key1))++i;
            --size;
            for (; i < size; ++i) {
                Array[i] = Array[i + 1];
            }
            min = Array[0].GetKey();
            max = Array[size - 1].GetKey();

        }
    };

    //empty head
    struct HeadNode {
        long pre;
        long next;
    };

    long head, rear;
    int block_size = (4096 / sizeof(someType)) % 2 ? 4096 / sizeof(someType) - 1 : 4096 / sizeof(someType);

    //LinkList读写的文件流对象 在LinkList构造时和相关文件关联
    std::fstream r_w_LinkList;

    //返回iter指向节点
    BlockNode ReadNode(long iter) {
        BlockNode blockNode;
        r_w_LinkList.seekg(iter);//定位到下一个节点
        r_w_LinkList.read(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
        return blockNode;
    }

    //修改blockNode信息，覆盖原信息
    void WriteNode(BlockNode blockNode, const long &iter) {
        r_w_LinkList.seekp(iter);
        r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    }


    //返回index所在的node
    BlockNode FindNode(key key1, long &iter) {
        iter = head;
        r_w_LinkList.seekg(iter);//HeadNode位置
        HeadNode headNode;
        r_w_LinkList.read(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
        iter = headNode.next;//第一个节点
        BlockNode blockNode;
        while (iter != 0) {
            blockNode = ReadNode(iter);
            if (key1 >= blockNode.min && blockNode.max >= key1) break;
        }
        return blockNode;
    }


    //在文件末开一个新node
    long GetNode() {
        r_w_LinkList.seekp(0, std::ios::end);//the end of the file
        long iter = r_w_LinkList.tellp();
        BlockNode blockNode;
        r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
        return iter;
    }

    //裂块
    void BreakNode(const long &iter, BlockNode &blockNode) {
        long newIter = GetNode();
        BlockNode newNode;
        someType ele;//空
        //update the array
        for (int i = block_size / 2; i < block_size; ++i) {
            newNode.Array[i - block_size / 2] = blockNode.Array[i];
            blockNode.Array[i] = ele;
        }
        //size
        blockNode.size = newNode.size = block_size / 2;
        //index
        newNode.max = blockNode.max;
        blockNode.max = blockNode.Array[block_size / 2].GetKey();
        newNode.min = newNode.Array[0].GetKey();
        //link
        //特判rear
        newNode.next = blockNode.next;
        newNode.pre = iter;
        blockNode.next = newIter;
        if (iter == rear) {
            rear = newIter;
        } else {
            r_w_LinkList.seekg(newNode.next);
            BlockNode nextNode;
            r_w_LinkList.read(reinterpret_cast<char *> (&nextNode), sizeof(BlockNode));
            nextNode.pre = newIter;
            WriteNode(nextNode, newNode.next);
//        r_w_LinkList.seekp(newNode.next);
//        r_w_LinkList.write(reinterpret_cast<char *> (&nextNode), sizeof(BlockNode));
        }
        WriteNode(newNode, newIter);
//    r_w_LinkList.seekp(newIter);
//    r_w_LinkList.write(reinterpret_cast<char *> (&newNode), sizeof(BlockNode));
    }

    //并块
    void CombineNode(const long &iter, BlockNode &blockNode1, BlockNode &blockNode2) {
        int blockSize = block_size;
        //update the array
        for (int i = blockSize / 2; i < blockSize; ++i) {
            blockNode1.Array[i] = blockNode2.Array[i - blockSize / 2];
        }
        //size
        blockNode1.size = blockSize;
        blockNode1.max = blockNode2.max;
        //link
        //rear?
        if (blockNode1.next != rear) {
            blockNode1.next = blockNode2.next;
            BlockNode nextNode = ReadNode(blockNode2.next);
            nextNode.pre = blockNode2.pre;
            WriteNode(nextNode, blockNode2.next);
        } else {
            blockNode1.next = blockNode2.next;
            rear = blockNode2.pre;
        }
        WriteNode(blockNode1, blockNode2.pre);
    }

};

#endif //BOOKSTORE_LINKLIST_H
