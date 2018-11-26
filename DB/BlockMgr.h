#ifndef BLOCK_MGR_H
#define BLOCK_MGR_H
#include <vector>
#include "sql/Expr.h"
#include "Convertor.h"
#include "global.h"
using namespace std;
using namespace hsql;

class Block;

typedef struct file {
	int fileid;
	char fileid64[4];
	int16 blockNum;
	file() {}
	file(int _fileid, int _blocknum = 0) {
		fileid = _fileid;
		blockNum = _blocknum;
		strcpy(fileid64,Conv64::to_64(fileid,3).c_str());
	}
}file;

class BlockMgr
{
public:
	static BlockMgr* getInstance();
	static void release();
	~BlockMgr();
private:
	static BlockMgr* instance;
	BlockMgr() {}
	BlockMgr(const BlockMgr&) {}
	BlockMgr& operator=(const BlockMgr&)=delete;
public:
	bool isFileFull(const char* fileid);									//�ļ��Ƿ��Ѿ�����
	bool isAbleToInput(const char* fileid,const char* blockid, Expr content);	//һ�������Ƿ��ܷ���Expr
	bool isAbleToInput(Block* block, Expr content);					
	const char* allocBlock(const char* fileid,BlockType bt=table);				//��һ���ļ��ڷ����,�����ļ��żӿ��
	const char* allocFile();												//�´���һ���ļ�,�����ļ���
	Block* getBlock(const char* fileid, const char* blockid);					//����fileid��blockid���ҿ�,ע���ڴ�й¶���ں�����new���µ�block��ʹ�ú���Ҫ�Լ�delete
	file* getFile(const char* fileid);									//����fileid����file
private:
	vector<file> files;
	Block* b;		//���ڷ�ֹgetBlock�������ڴ�й©
};

#endif // !BLOCK_MGR_H