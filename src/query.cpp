#include "query.h"
#include <string>
#include <utility>
#include "sqlite3.h"
#include <sstream>


Query::Query(std::string q) : cmd(std::move(q)), finalized(false), statement(nullptr)  {}

void Query::Prepare(Connection& conn) {
    sqlite3_prepare_v2(conn.db, cmd.c_str(), cmd.size(), &statement, 0);
}

int Query::ExecuteStep() {
	if(!finalized)
    return sqlite3_step(statement);
}

void Query::StatementReset() {
	if(!finalized)
    sqlite3_reset(statement);
}

void Query::Finalize() {
	if (sqlite3_finalize(statement) == SQLITE_OK) {
		finalized = true;
	}
}

template<>
void Query::Bind(const std::vector<int>& vals) {
	int i  = 0;
	for(auto val : vals) {
		sqlite3_bind_int(statement, i, val);
	}
}

template<>
void Query::Bind(const std::vector<double>& vals) {
	int i  = 0;
	for(auto val : vals) {
		sqlite3_bind_double(statement, i, val);
	}
}

template<>
void Query::Bind(const std::vector<std::string>& vals) {
	int i  = 0;
	for(auto val : vals) {
		int res = sqlite3_bind_text(statement, i, val.c_str(), -1, SQLITE_STATIC);
	}
}

template<>
void Query::Bind(size_t id, int val) {
	sqlite3_bind_int(statement, id, val);
}

template<>
void Query::Bind(size_t id, double val) {
	sqlite3_bind_double(statement, id, val);
}

template<>
void Query::Bind(size_t id, const std::string& val) {
	sqlite3_bind_text(statement, id, val.c_str(), -1, SQLITE_STATIC);
}

template <>
double ReadQuery::GetColumn(int col_num) {
	return sqlite3_column_double(statement, col_num);
}

template <>
int ReadQuery::GetColumn(int col_num) {
	return sqlite3_column_int(statement, col_num);
}

template <>
std::string ReadQuery::GetColumn(int col_num) {
	std::stringstream ss;
	size_t sz = sqlite3_column_bytes(statement, col_num);
	const unsigned char* text = sqlite3_column_text(statement, col_num);
	for(size_t i = 0; i < sz; ++i) {
		ss << text[i];
	}
	return ss.str();
}
