#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include "BPlus_node.h"
#include <vector>
using namespace std;

enum COMPARE_OPERATOR { LT, LE, EQ, BE, BT, BETWEEN }; // �Ƚϲ�������<��<=��=��>=��>��<>
const int INVALID_INDEX = -1;

struct SelectResult
{
	int keyIndex;
	LeafNode* targetNode;
};

class BPlusTree {
public:
	BPlusTree();
	~BPlusTree();
	bool insert(KeyType key, const DataType& data);
	bool remove(KeyType key);
	bool update(KeyType oldKey, KeyType newKey);
	// ��ֵ��ѯ��compareOperator������LT(<)��LE(<=)��EQ(=)��BE(>=)��BT(>)
	vector<DataType> select(KeyType compareKey, int compareOpeartor);
	// ��Χ��ѯ��BETWEEN
	vector<DataType> select(KeyType smallKey, KeyType largeKey);
	bool search(KeyType key); // �����Ƿ����
	vector<DataType> getAllValues();	//��������ֵ
	vector<pair<KeyType,DataType> > getAllKVPairs();	//�������м�ֵ��
	void clear();             // ���
	void print()const;        // ��ӡ���ؼ���
	void printData()const;    // ��ӡ����
private:
	void r_insert(Node* parentNode, KeyType key, const DataType& data);	//����ǰ׺r_��ʾ�ݹ麯��
	void r_remove(Node* parentNode, KeyType key);
	void printInConcavo(Node *pNode, int count)const;
	bool r_search(Node *pNode, KeyType key)const;
	void changeKey(Node *pNode, KeyType oldKey, KeyType newKey);
	void search(KeyType key, SelectResult& result);
	void r_search(Node* pNode, KeyType key, SelectResult& result);
	void remove(KeyType key, DataType& dataValue);
	void r_remove(Node* parentNode, KeyType key, DataType& dataValue);
private:
	Node* pRoot;
	LeafNode* pDataHead;
	KeyType MaxKey;  // B+���е�����
};

#endif
