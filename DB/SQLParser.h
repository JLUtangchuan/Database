#ifndef SQLPARSER_SQLPARSER_H
#define SQLPARSER_SQLPARSER_H

#include "SQLParserResult.h"
#include "sql/statements.h"

namespace hsql {

  class SQLParser {
   public:
	// ����sql��䣬���ת���ɹ�����true��ʧ�ܷ���false����������sql�����Ч�������Ч����result->isValid();
    static bool parse(const std::string& sql, SQLParserResult* result);
    // ����Ϊ����
    static bool tokenize(const std::string& sql, std::vector<int16_t>* tokens);

   private:
    SQLParser();
  };

} // namespace hsql


#endif