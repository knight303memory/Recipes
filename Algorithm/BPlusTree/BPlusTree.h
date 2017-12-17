// LearnAlgorithm.cpp : 定义控制台应用程序的入口点。
// 

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <math.h>

#define ROOTPID 1
using namespace std;


class None_Leaf_Node;
struct Node
{
	int pid;
	
	None_Leaf_Node *fatherNode;
	virtual ~Node() {}

	virtual int getNodeEntrySize() { return 0; }
	
};

struct None_Leaf_Node : public Node
{
	list<Node *> nextLevelNodes;
	list<int> keys;
	int getNodeEntrySize()
	{
		return keys.size();
	}

};

struct Leaf_Node : public Node
{
	map<int, int> keyValues;
	Leaf_Node *nextLeafNode;
	int getNodeEntrySize()
	{
		return keyValues.size();
	}
};

struct FileHeader
{
	int pageSize;
	int rootId;
	int depth;
};

class BPlusTree
{

public:
	BPlusTree();

	~BPlusTree();

	//第一个子树的所有关键字都小于父亲的第一个关键字
	//注意分裂
	bool insert(int key, int rid)
	{
		//刚开始插入的几次 直接在叶子节点插入
		if (typeid(*m_RootNode) == typeid(Leaf_Node))
		{
			//向叶子节点插入
			auto itr =  dynamic_cast<Leaf_Node *>( m_RootNode) ->keyValues.insert(pair<int, int>(key, rid));
			if (itr.second)
			{
				cout << "插入成功" << endl;
			}
			else
			{
				cout << "插入失败" << endl;
				return false;
			}
		
			//调整叶子节点
			adjustAfterInsert(m_RootNode, 1);
			return true;
		}
		else
		{
			//先查找然后插入 
			Leaf_Node *lNode = findInsertPoint(key);
			if (lNode->fatherNode == nullptr)
			{
				return false;
			}

			auto itr = lNode->keyValues.insert(pair<int, int>(key, rid));
			if (itr.second)
			{
				cout << "插入成功" << endl;
			}
			else
			{
				cout << "插入失败" << endl;
				return false;
			}

			adjustAfterInsert(lNode, 1);
			return true;
		}
		
	}


	Leaf_Node *findInsertPoint(int key) 
	{
		
		Node *curNode = m_RootNode;

		int depth = 1;
		//根节点是叶子节点,直接返回这个节点, 实际开发可以用一个标识标识叶子节点  非叶子节点 
		if (typeid(*curNode) == typeid(Leaf_Node))
		{
			return static_cast<Leaf_Node *>(curNode);
		}

		//根节点是非叶子节点 找到那个叶子节点
		
		while (depth <= m_Depth)
		{
			bool isLower = false;
		
		
				auto nextLevelNodesItr = static_cast<None_Leaf_Node *>(curNode)->nextLevelNodes.begin();

				for (const int &tempKey : static_cast<None_Leaf_Node *>(curNode)->keys)
				{
					if (key < tempKey)
					{
						isLower = true;
						curNode = *nextLevelNodesItr;
						break;
					}

					nextLevelNodesItr++;

				}
				if (isLower == false)
				{
					curNode = *nextLevelNodesItr;
				}
			

			depth++;
		}

		//处理叶子节点  depth == m_Depth
		return static_cast<Leaf_Node *>(curNode);
		
	}


private:

	int getEntryNum()// (PageSize - 4)/8
	{
		return (m_PageSize - 4) / 8;
	}

	int getPhysialOffset(int pid) //get offset in binary file
	{
		return 12 + ((pid - 1) * m_PageSize);
	}

	int getPageSize()
	{
		return m_PageSize;
	}

	int getDepth()
	{
		return m_Depth;
	}

	void adjustAfterInsert(Node *node, uint32_t depth)
	{
		if (node->getNodeEntrySize() <= m_MaxEntryNum)
		{
			return;
		}
		
		if (depth > m_Depth)
		{
			m_Depth = depth;
		}

		//判断是非叶子节点还是叶子节点 
		if (typeid(*node) == typeid(None_Leaf_Node))
		{
			None_Leaf_Node *curNode = static_cast<None_Leaf_Node *>(node);
			
			None_Leaf_Node *newNextNode = new None_Leaf_Node;
			newNextNode->fatherNode = curNode->fatherNode;



			//需要移动索引和子节点  
			auto childItr =  curNode->nextLevelNodes.begin();
			advance(childItr, 1);

			int count = 0;
			list<Node *>::iterator delChildItr;
			list<int>::iterator delKeyItr = curNode->keys.begin();
			for (const auto &curKey : curNode->keys)
			{
				if (count > m_MinEntryNum)
				{
					if (newNextNode->keys.empty())
					{
						delChildItr = childItr;
						
					}
					//加入到新节点
					newNextNode->keys.push_back(curKey);
					newNextNode->nextLevelNodes.push_back(*childItr);
				}
				else
				{
					delKeyItr++;
				}
				count++;
				childItr++;
			}

			//删除原来的  
			curNode->keys.erase(delKeyItr, curNode->keys.end());
			curNode->nextLevelNodes.erase(delChildItr, curNode->nextLevelNodes.end());

			//调整父节点  父节点为空  当前是根节点
			if (curNode->fatherNode == nullptr)
			{
				None_Leaf_Node *newRootNode = new None_Leaf_Node;
				newRootNode->pid = 1;

				newRootNode->keys.push_back(*delKeyItr);
				newRootNode->nextLevelNodes.push_back(curNode);
				newRootNode->nextLevelNodes.push_back(newNextNode);

				//修改根节点
				m_RootNode = newRootNode;
				return;
				
			}
			else  
			{		
				auto itr = lower_bound(curNode->fatherNode->keys.begin(), curNode->fatherNode->keys.end(), *delKeyItr);

				//需要保证key 和  子节点的一致性  用set方便  
				curNode->fatherNode->keys.insert(itr, *delKeyItr);

				auto itr2 = curNode->fatherNode->nextLevelNodes.begin();


				for (; itr2 != curNode->fatherNode->nextLevelNodes.end(); itr2++)
				{
					if (static_cast<Leaf_Node *>(*itr2)->keyValues.begin()->first == *delKeyItr)
					{
						itr2++;
						break;
					}
				}

				curNode->fatherNode->nextLevelNodes.insert(itr2, newNextNode);
				adjustAfterInsert(curNode->fatherNode, ++depth);
			}
		}
		else//叶子节点 
		{
			Leaf_Node *curNode = static_cast<Leaf_Node *>(node);
			Leaf_Node *nextLeafNode = new Leaf_Node();

			None_Leaf_Node *fatherNode = curNode->fatherNode;

			nextLeafNode->fatherNode = fatherNode;
			nextLeafNode->nextLeafNode = curNode->nextLeafNode;
			
			curNode->nextLeafNode = nextLeafNode;

			
			//分裂叶子节点
			int count = 1;
			int insertKey;
			for (const auto & itr : curNode->keyValues)
			{
				if (count > m_MinEntryNum)
				{		
					if (nextLeafNode->keyValues.empty())
					{
						insertKey = itr.first;
					}
					nextLeafNode->keyValues.insert(itr);
				}
				count++;
				
			}

			//删除原来的过半的  不可能有重复
			auto eraseItr = curNode->keyValues.lower_bound(insertKey);
			curNode->keyValues.erase(eraseItr, curNode->keyValues.end());
			

			int toInsertKey = nextLeafNode->keyValues.begin()->first;
			//修改父节点
			if (fatherNode == nullptr)
			{
				None_Leaf_Node *newRootNode = new None_Leaf_Node;
			

				newRootNode->keys.push_back(toInsertKey);
				newRootNode->nextLevelNodes.push_back(curNode);
				newRootNode->nextLevelNodes.push_back(nextLeafNode);
				
				curNode->fatherNode = newRootNode;
				nextLeafNode->fatherNode = newRootNode;

				//修改根节点
				m_RootNode = newRootNode;
			}
			else
			{
				//插入关键字和子节点
				//先找到插入点
				auto itr = lower_bound(fatherNode->keys.begin(), fatherNode->keys.end(), toInsertKey);
				//需要保证key 和  子节点的一致性  用set方便  
				fatherNode->keys.insert(itr, toInsertKey);


				//插入子节点， 用set  重载node运算符 更方便一些 
				auto itr2 = fatherNode->nextLevelNodes.begin();
				for (;itr2 != fatherNode->nextLevelNodes.end(); itr2++)
				{
					if (static_cast<Leaf_Node *>(*itr2)->keyValues.begin()->first == toInsertKey)
					{
						itr2++;
						break;
					}
				}

				fatherNode->nextLevelNodes.insert(itr2, nextLeafNode);
				adjustAfterInsert(fatherNode, ++depth);
			}			
		}
	}
public:
	Node *m_RootNode; //根节点

	int m_PageSize;
	int m_MaxEntryNum;//each non-leaf-node has maxEntryNum  IndexEntries
	int m_MinEntryNum;
	int m_Depth;// disinguish a node is leaf or not by the depth
	
	void freeNode(None_Leaf_Node *node)
	{
		for (auto child : node->nextLevelNodes)
		{
			freeNode(static_cast<None_Leaf_Node *>(child));
		}
		delete(node);
	}
};

BPlusTree::BPlusTree() : m_Depth(1), m_PageSize(36), m_RootNode(new Leaf_Node)
{
	m_MaxEntryNum = getEntryNum();
	m_MinEntryNum = ceil(1.0 * m_MaxEntryNum / 2);
	m_RootNode->fatherNode = nullptr;
}


BPlusTree::~BPlusTree()
{	
	//释放空间节点空间
	
	if (typeid(*m_RootNode) == typeid(Leaf_Node))
	{
		delete(m_RootNode);
	}
	else
	{
		freeNode(static_cast<None_Leaf_Node *>(m_RootNode));
	}

}

//打印节点
void print(Node *node)
{
	if (typeid(*node) == typeid(None_Leaf_Node))
	{
		for (auto &node1 : static_cast<None_Leaf_Node *>(node)->nextLevelNodes)
		{
			print(node1);
		
		}
		cout << "非叶子节点" << endl;
		for (auto k : static_cast<None_Leaf_Node *>(node)->keys)
		{
			cout << k << " * ";
		}
		cout << endl;
		for (auto &node1 : static_cast<None_Leaf_Node *>(node)->nextLevelNodes)
		{
			if (typeid(*node1) == typeid(None_Leaf_Node))
			{
				cout << "非叶子节点" << endl;
				for (auto k : static_cast<None_Leaf_Node *>(node1)->keys)
				{
					cout << k << " * ";
				}
			}
			if (typeid(*node1) == typeid(Leaf_Node))
			{
				cout << "叶子节点" << endl;
				auto k = static_cast<Leaf_Node *>(node1)->keyValues;
				for (auto kk : k)
				{
					cout << kk.first << " , ";
				}
			}

			cout << endl;
		}
		return;
	 }
	else //根节点是叶子节点 
	{
		
// 			auto k = static_cast<Leaf_Node *>(node)->keyValues;
// 			for (auto kk : k)
// 			{
// 				cout << kk.first << " , ";
// 			}
// 
// 			cout << endl;
		
	}
}

int main()
{

	//Test  Insert
	
	BPlusTree bTree;
	bTree.insert(1, 2);
	bTree.insert(2, 2);
	bTree.insert(3, 2);
	bTree.insert(4, 2);
	bTree.insert(5, 2);
	bTree.insert(6, 2);
	bTree.insert(7, 2);
	bTree.insert(8, 2);
	bTree.insert(9, 2);
	bTree.insert(10, 2);

	print(bTree.m_RootNode);


	getchar();
    return 0;
}

