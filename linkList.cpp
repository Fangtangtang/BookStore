#include "linkList.h"

//构造函数
template<class someType>
LinkList<someType>::LinkList(const std::string &file_name, int typeSize) {
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
template<class someType>
LinkList<someType>::~LinkList() = default;

//insert an element into the list
template<class someType>
void LinkList<someType>::Insert(int index, const someType &ele) {
    //find the node
    //insert into the array
    long iter = head;
    bool break_flag = false;
    r_w_LinkList.seekg(iter);//HeadNode位置
    HeadNode headNode;
    BlockNode blockNode1, blockNode2;
    r_w_LinkList.read(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
    iter = headNode.next;//第一个节点
    //in a node
    while (iter != 0) {
        blockNode1 = blockNode2;
        blockNode2 = ReadNode(iter);
        if (index > blockNode1.min && index < blockNode2.min) {
            if (index < blockNode1.max) {
                break_flag = blockNode1.Insert(index, ele);//插入当前块的array
                iter = blockNode2.pre;
                if (break_flag) BreakNode(iter, blockNode1);
                WriteNode(blockNode1,iter);
//                r_w_LinkList.seekp(iter);
//                r_w_LinkList.write(reinterpret_cast<char *> (&blockNode1), sizeof(BlockNode));
            } else {
                break_flag = blockNode2.Insert(index, ele);//插入后一块的array
                if (break_flag) BreakNode(iter, blockNode2);
                WriteNode(blockNode2,iter);
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
        blockNode.Insert(index, ele);//不可能裂块
        WriteNode(blockNode,iter);
//        r_w_LinkList.seekp(iter);
//        r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    }
}

//erase an element from the list
template<class someType>
void LinkList<someType>::Erase(int index) {
    //find the ele
    long iter;//the pos of the node
    BlockNode blockNode = FindNode(index, iter);
    bool combine_flag = blockNode.Erase(index);//是否混合
    if(combine_flag){
        int blockSize=blockNode.block_size;
        BlockNode preNode= ReadNode(blockNode.pre);
        BlockNode nextNode= ReadNode(blockNode.next);
        if(preNode.size>blockSize/2){
            //update the array
            someType borrowedEle=preNode.Find(preNode.max);
            blockNode.Insert(preNode.max,borrowedEle);
            preNode.Erase(preNode.max);
            return;
        }
        if(nextNode.size>blockSize/2){
            someType borrowedEle=nextNode.Array[0];
            blockNode.Insert(nextNode.min,borrowedEle);
            nextNode.Erase(nextNode.min);
            return;
        }
        //选择后一个并块
        CombineNode(iter,blockNode,nextNode);
    }
}

//find an element in the list
template<class someType>
someType LinkList<someType>::FindIndex(int index) {
    long iter;
    BlockNode blockNode = FindNode(index, iter);
    someType ele = blockNode.Find(index);
    return ele;
}

template<class someType>
typename LinkList<someType>::BlockNode LinkList<someType>::ReadNode(long iter) {
    BlockNode blockNode;
    r_w_LinkList.seekg(iter);//定位到下一个节点
    r_w_LinkList.read(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    return blockNode;
}

template<class someType>
void LinkList<someType>::WriteNode(const LinkList::BlockNode &blockNode, const long &iter) {
    r_w_LinkList.seekp(iter);
    r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
}
template<class someType>
void LinkList<someType>::BreakNode(const long &iter, BlockNode &blockNode) {
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
    blockNode.max = blockNode.Array[blockNode.block_size / 2].GetIndex();
    newNode.min = newNode.Array[0].GetIndex();
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
        WriteNode(nextNode,newNode.next);
//        r_w_LinkList.seekp(newNode.next);
//        r_w_LinkList.write(reinterpret_cast<char *> (&nextNode), sizeof(BlockNode));
    }
    WriteNode(newNode,newIter);
//    r_w_LinkList.seekp(newIter);
//    r_w_LinkList.write(reinterpret_cast<char *> (&newNode), sizeof(BlockNode));
}

template<class someType>
void
LinkList<someType>::CombineNode(const long &iter, LinkList::BlockNode &blockNode1, LinkList::BlockNode &blockNode2) {
    int blockSize=blockNode1.block_size;
    //update the array
    for(int i=blockSize/2;i<blockSize;++i){
        blockNode1.Array[i]=blockNode2.Array[i-blockSize/2];
    }
    //size
    blockNode1.size=blockSize;
    blockNode1.max=blockNode2.max;
    //link
    //rear?
    if(blockNode1.next!=rear){
        blockNode1.next=blockNode2.next;
        BlockNode nextNode=ReadNode(blockNode2.next);
        nextNode.pre=blockNode2.pre;
        WriteNode(nextNode,blockNode2.next);
    }else {
        blockNode1.next=blockNode2.next;
        rear=blockNode2.pre;
    }
    WriteNode(blockNode1,blockNode2.pre);
}

template<class someType>
long LinkList<someType>::GetNode() {
    r_w_LinkList.seekp(0, std::ios::end);//the end of the file
    long iter = r_w_LinkList.tellp();
    BlockNode blockNode;
    r_w_LinkList.write(reinterpret_cast<char *> (&blockNode), sizeof(BlockNode));
    return iter;
}

template<class someType>
typename LinkList<someType>::BlockNode LinkList<someType>::FindNode(int index, long &iter) {
    iter = head;
    r_w_LinkList.seekg(iter);//HeadNode位置
    HeadNode headNode;
    r_w_LinkList.read(reinterpret_cast<char *> (&headNode), sizeof(HeadNode));
    iter = headNode.next;//第一个节点
    while (iter != 0) {
        BlockNode blockNode = ReadNode(iter);
        if (index >= blockNode.min && index <= blockNode.max) return blockNode;
    }
}

//insert an element into the array
//change the BlockNode(in memory)
template<class someType>
bool LinkList<someType>::BlockNode::Insert(int index, const someType &ele) {
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
                max = min = index;
            } else {
                min = index;
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
template<class someType>
someType LinkList<someType>::BlockNode::Find(int index) {
    int i = 0;
    while (Array[i].GetIndex != index)++i;
    return Array[i];
}

//erase the element from the array in BlockNode(in memory)
template<class someType>
bool LinkList<someType>::BlockNode::Erase(int index) {
    int i = 0;
    while (Array[i].GetIndex != index)++i;
    --size;
    for (; i < size; ++i) {
        Array[i] = Array[i + 1];
    }
    min=Array[0].GetIndex();
    max=Array[size-1].GetIndex();
    if (size < block_size / 2) {
        return true;
    } else {
        return false;
    }
}
