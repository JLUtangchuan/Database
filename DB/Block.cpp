#include "Block.h"
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include "Convertor.h"
using namespace std;

Block::Block(BlockType _type, const char * _fileid, const char * _blockid)
{
	memset(buffer, 0, sizeof(buffer));
	blockType = _type;
	strcpy(fileid, _fileid);
	strcpy(blockid, _blockid);
	metaEnd = 0;
	bodyBegin = BLOCK_SIZE;
	updateBuffer();
}

Block::Block(const char * _fileid, const char * _blockid)
{
	memset(buffer, 0, sizeof(buffer));
	strcpy(fileid, _fileid);
	strcpy(blockid, _blockid);
	metaEnd = 0;
	bodyBegin = BLOCK_SIZE;
	updateBuffer();
}

Block::~Block()
{
}

bool Block::isAbleToInput(Expr content)
{
	if (pctfree < PCT_FREE_THRESHOLD||pctused>PCT_USED_THRESHOLD)
		return false;
	
	return true;
}

bool Block::put(Expr content)
{
	return false;
}

Expr Block::get(char * rowid)
{
	return get(atoi(Conv64::to_10(rowid).c_str()));
}

const char * Block::generateRowID(Expr content)
{
	stringstream ss;
	ss << fileid << blockid<< Conv64::to_64(recordnum);
	return ss.str().c_str();
}

bool Block::writeToFile()
{
	ofstream nodeFile(getFileName(), ios::binary | ios::out);
	if (!nodeFile)
		return false;
	nodeFile.seekp(atoi(Conv64::to_10(blockid).c_str()));
	nodeFile.write(buffer, BLOCK_SIZE);
	nodeFile.close();
	return true;
}

bool Block::readFromFile()
{
	ifstream nodeFile(getFileName(), ios::binary | ios::in);
	if (!nodeFile)
		return false;
	nodeFile.seekg(atoi(Conv64::to_10(blockid).c_str()));
	nodeFile.read(buffer, BLOCK_SIZE);
	nodeFile.close();
	return true;
}

void Block::writeToBuffer(int begin, char * text)
{
	strcpy(buffer + begin, text);
}

int Block::getMetaEnd()
{
	return metaEnd;
}

void Block::setMetaEnd(int _metaEnd)
{
	metaEnd = _metaEnd;
}

int Block::getBodyBegin()
{
	return bodyBegin;
}

void Block::setBodyBgein(int _bodyBegin)
{
	bodyBegin = _bodyBegin;
}

void Block::updateBuffer()
{
	metaEnd = 0;
	memcpy(buffer + metaEnd, databaseObjectID, sizeof(databaseObjectID));
	metaEnd += sizeof(databaseObjectID) - 1;
	memcpy(buffer + metaEnd, fileid, sizeof(fileid));
	metaEnd += sizeof(fileid) - 1;
	memcpy(buffer + metaEnd, blockid, sizeof(fileid));
	metaEnd += sizeof(blockid);
	memcpy(buffer + metaEnd, blockid, sizeof(fileid));
	metaEnd += sizeof(blockid);
	memcpy(buffer + metaEnd, &pctfree, sizeof(pctfree));
	metaEnd += sizeof(pctfree);
	memcpy(buffer + metaEnd, &pctused, sizeof(fileid));
	metaEnd += sizeof(pctused);
	memcpy(buffer + metaEnd, &recordnum, sizeof(recordnum));
	metaEnd += sizeof(recordnum);
	for (int i = 0; i < recordnum; i++) {
		memcpy(buffer + metaEnd, &recordpos[i], sizeof(recordpos[i]));
		metaEnd += sizeof(recordpos[i]);
	}
	for (int i = 0; i < recordnum; i++) {
		memcpy(buffer + metaEnd, &dataType[i], sizeof(dataType[i]));
		metaEnd += sizeof(dataType[i]);
	}
}

void Block::updateVar()
{
	long location = 0;
	char buff[19];
	memcpy(buff, buffer + location, sizeof(buff));
	location += sizeof(buff);
	string s(buff);
	strcpy(databaseObjectID, s.substr(0, 6).c_str());
	strcpy(fileid, s.substr(6, 3).c_str());
	strcpy(blockid, s.substr(9, 6).c_str());

	memcpy(blockid, buffer + location, sizeof(fileid));
	location += sizeof(blockid);
	memcpy(&pctfree, buffer + location, sizeof(pctfree));
	location += sizeof(pctfree);
	memcpy(&pctused, buffer + location, sizeof(fileid));
	location += sizeof(pctused);
	memcpy(&recordnum, buffer + location, sizeof(recordnum));
	location += sizeof(recordnum);

	arrayLen = 0;
	for (int i = 0; i < recordnum; i++) {
		memcpy(&recordpos[i], buffer + location, sizeof(recordpos[i]));
		location += sizeof(recordpos[i]);
	}
	for (int i = 0; i < recordnum; i++) {
		memcpy(&dataType[i], buffer + location, sizeof(dataType[i]));
		location += sizeof(dataType[i]);
	}
}

string Block::getFileName()
{
	stringstream ss;
	switch (blockType)
	{
	case index:
		ss << "./data/idx_" << Conv64::to_10(fileid, 3) << ".db";
		break;
	case table:
		ss << "./data/tbl_" << Conv64::to_10(fileid, 3) << ".db";
		break;
	case dictionary:
		ss << "./data/dict_" << Conv64::to_10(fileid, 3) << ".db";
		break;
	default:
		break;
	}
	
	return ss.str();
}

Expr Block::get(int16 idx)
{
	return Expr(kExprLiteralNull);
}

