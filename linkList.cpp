#include "linkList.h"

//构造函数
template<class key, class someType>
LinkList<key, someType>::LinkList(const std::string &file_name, int typeSize) {
    //relate fstream with the file
    r_w_LinkList.open(file_name);

    //determine block_size
    if ((4096 / typeSize) % 2) {
        BlockNode::block_size = 4096 / typeSize - 1;
    } else {
        BlockNode::block_size = 4096 / typeSize;
    }

    //create headNode
    HeadNode headNode;
    headNode.pre = headNode.next = 0;//空：相对头的偏移量为0
    r_w_LinkList.seekp(0);//将指针定位到文件开头
    rear = head = r_w_LinkList.tellp();
    //从文件开头写HeadNode
    r_w_LinkList.write(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
}

//析构函数
template<class key, class someType>
LinkList<key, someType>::~LinkList() = default;

//insert an element into the list
template<class key, class someType>
void LinkList<key, someType>::Insert(key key1, const someType &ele) {
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
        if (key1 > blockNode1.min && blockNode2.min>key1) {
            if (blockNode1.max>key1) {
                break_flag = blockNode1.Insert(key1, ele);//插入当前块的array
                iter = blockNode2.pre;
                if (break_flag) BreakNode(iter, blockNode1);
                WriteNode(blockNode1, iter);
//                r_w_LinkList.seekp(iter);
//                r_w_LinkList.write(reinterpret_cast<char *> (&blockNode1), sizeof(BlockNode));
            } else {
                break_flag = blockNode2.Insert(key1, ele);//插入后一块的array
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

//erase an element from the list
template<class key, class someType>
void LinkList<key, someType>::Erase(key key1) {
    //find the ele
    long iter;//the pos of the node
    BlockNode blockNode = FindNode(key1, iter);
    bool combine_flag = blockNode.Erase(key1);//是否混合
    if (combine_flag) {
        int blockSize = blockNode.block_size;
        BlockNode preNode = ReadNode(blockNode.pre);
        BlockNode nextNode = ReadNode(blockNode.next);
        if (preNode.size > blockSize / 2) {
            //update the array
            someType borrowedEle = preNode.Find(preNode.max);
            blockNode.Insert(preNode.max, borrowedEle);
            preNode.Erase(preNode.max);
            return;
        }
        if (nextNode.size > blockSize / 2) {
            someType borrowedEle = nextNode.Array[0];
            blockNode.Insert(nextNode.min, borrowedEle);
            nextNode.Erase(nextNode.min);
            return;
        }
        //选择后一个并块
        CombineNode(iter, blockNode, nextNode);
    }
}

//find an element in the list
template<class key, class someType>
someType LinkList<key, someType>::FindKey(key key1) {
    long iter;
    BlockNode blockNode = FindNode(key1, iter);
    someType ele = blockNode.Find(key1);
    return ele;
}

template<class key, class someType>
typename LinkList<key, someType>::BlockNode LinkList<key, someType>::ReadNode(long iter) {
    BlockNode blockNode;
    r_w_LinkList.seekg(iter);//定位到下一个节点
    r_w_LinkList.read(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    return blockNode;
}

template<class key, class someType>
void LinkList<key, someType>::WriteNode(const LinkList::BlockNode &blockNode, const long &iter) {
    r_w_LinkList.seekp(iter);
    r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
}

template<class key, class someType>
void LinkList<key, someType>::BreakNode(const long &iter, BlockNode &blockNode) {
    long newIter = GetNode();
    BlockNode newNode;
    someType ele;//空
    //update the array
    for (int i = blockNode.block_size / 2; i < blockNode.block_size; ++i) {
        newNode.Array[i - blockNode.block_size / 2] = blockNode.Array[i];
        blockNode.Array[i] = ele;
    }
    //size
    blockNode.size = newNode.size = blockNode.block_size / 2;
    //index
    newNode.max = blockNode.max;
    blockNode.max = blockNode.Array[blockNode.block_size / 2].GetKey();
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

template<class key, class someType>
void LinkList<key, someType>::CombineNode(const long &iter, LinkList::BlockNode &blockNode1,
                                          LinkList::BlockNode &blockNode2) {
    int blockSize = blockNode1.block_size;
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

template<class key, class someType>
long LinkList<key, someType>::GetNode() {
    r_w_LinkList.seekp(0, std::ios::end);//the end of the file
    long iter = r_w_LinkList.tellp();
    BlockNode blockNode;
    r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    return iter;
}

template<class key, class someType>
typename LinkList<key, someType>::BlockNode LinkList<key, someType>::FindNode(key key1, long &iter) {
    iter = head;
    r_w_LinkList.seekg(iter);//HeadNode位置
    HeadNode headNode;
    r_w_LinkList.read(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
    iter = headNode.next;//第一个节点
    while (iter != 0) {
        BlockNode blockNode = ReadNode(iter);
        if (key1 >= blockNode.min &&  blockNode.max>=key1) return blockNode;
    }
}

//insert an element into the array
//change the BlockNode(in memory)
template<class key, class someType>
bool LinkList<key, someType>::BlockNode::Insert(key key1, const someType &ele) {
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
    if (size != block_size) {
        //update
        if (i == 0) {
            if (size == 1) {
                max = min = key1;
            } else {
                min = key1;
            }
        }
        return false;
    }
        //裂块
    else {
        return true;
    }
}

//find the element in the array in BlockNode(in memory)
template<class key, class someType>
someType LinkList<key, someType>::BlockNode::Find(key key1) {
    int i = 0;
    while (!(Array[i].GetKey == key1))++i;
    return Array[i];
}

//erase the element from the array in BlockNode(in memory)
template<class key, class someType>
bool LinkList<key, someType>::BlockNode::Erase(key key1) {
    int i = 0;
    while (!(Array[i].GetKey == key1))++i;
    --size;
    for (; i < size; ++i) {
        Array[i] = Array[i + 1];
    }
    min = Array[0].GetKey();
    max = Array[size - 1].GetKey();
    if (size < block_size / 2) {
        return true;
    } else {
        return false;
    }
}
