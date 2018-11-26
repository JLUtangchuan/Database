#ifndef BPLUS_NODE_H
#define BPLUS_NODE_H
#include "global.h"

enum NODE_TYPE { INTERNAL, LEAF };        // ������ͣ��ڽ�㡢Ҷ�ӽ��
enum SIBLING_DIRECTION { LEFT, RIGHT };   // �ֵܽ�㷽�����ֵܽ�㡢���ֵܽ��

// ������
class Node {
public:
	Node();
	virtual ~Node();

	NODE_TYPE getType() const { return m_Type; }
	void setType(NODE_TYPE type) { m_Type = type; }
	int getKeyNum() const { return m_KeyNum; }
	void setKeyNum(int n) { m_KeyNum = n; }
	KeyType getKeyValue(int i) const { return m_KeyValues[i]; }
	void setKeyValue(int i, KeyType key) { m_KeyValues[i] = key; }
	int getKeyIndex(KeyType key)const;  // �ҵ���ֵ�ڽ���д洢���±�
										// ���麯��������ӿ�
	virtual void removeKey(int keyIndex, int childIndex) = 0;  // �ӽ�����Ƴ���ֵ
	virtual void split(Node* parentNode, int childIndex) = 0; // ���ѽ��
	virtual void mergeChild(Node* parentNode, Node* childNode, int keyIndex) = 0;  // �ϲ����
	virtual void clear() = 0; // ��ս�㣬ͬʱ����ս�����������������
	virtual void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d) = 0; // ���ֵܽ���н�һ����ֵ
	virtual int getChildIndex(KeyType key, int keyIndex)const = 0;  // ���ݼ�ֵ��ȡ���ӽ��ָ���±�
protected:
	NODE_TYPE m_Type;
	int m_KeyNum;
	KeyType m_KeyValues[MAXNUM_KEY];
};

// �ڽ��
class InternalNode : public Node {
public:
	InternalNode();
	virtual ~InternalNode();

	Node* getChild(int i) const { return m_Childs[i]; }
	void setChild(int i, Node* child) { m_Childs[i] = child; }
	void insert(int keyIndex, int childIndex, KeyType key, Node* childNode);
	virtual void split(Node* parentNode, int childIndex);
	virtual void mergeChild(Node* parentNode, Node* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	Node* m_Childs[MAXNUM_CHILD];
};

// Ҷ�ӽ��
class LeafNode : public Node {
public:
	LeafNode();
	virtual ~LeafNode();

	LeafNode* getLeftSibling() const { return m_LeftSibling; }
	void setLeftSibling(LeafNode* node) { m_LeftSibling = node; }
	LeafNode* getRightSibling() const { return m_RightSibling; }
	void setRightSibling(LeafNode* node) { m_RightSibling = node; }
	DataType getData(int i) const { return m_Datas[i]; }
	void setData(int i, const DataType& data) { m_Datas[i] = data; }
	void insert(KeyType key, const DataType& data);
	virtual void split(Node* parentNode, int childIndex);
	virtual void mergeChild(Node* parentNode, Node* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(Node* destNode, Node* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	LeafNode* m_LeftSibling;
	LeafNode* m_RightSibling;
	DataType m_Datas[MAXNUM_LEAF];
};
#endif