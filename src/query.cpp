#include "query.h"
#include <string>
#include <utility>
#include "sqlite3.h"
#include <sstream>
#include "error_handler.h"


Query::Query(const std::string& q) : cmd_(q), finalized_(false), statement_(nullptr)  {}

void Query::Prepare(Connection conn) {
    sqlite3_prepare_v2(conn.db, cmd_.c_str(), cmd_.size(), &statement_, 0);
}

void Query::ExecuteStep() {
	if(!finalized_)
    SQLiteError::CheckError(sqlite3_step, statement_);
}

void Query::ExecuteMany(int num) {
	if(!finalized_) {
		int err;
		for (int i = 0; i < num; ++i) {
			err = SQLiteError::CheckError(sqlite3_step, statement_);
			if (err)
				break;
			StatementReset();
		}
	}
}

void Query::StatementReset() {
	if(!finalized_)
		SQLiteError::CheckError(sqlite3_reset, statement_);
}

void Query::Finalize() {
	if (SQLiteError::CheckError(sqlite3_finalize, statement_) == SQLITE_OK) {
		finalized_ = true;
	}
}

template<>
void Query::Bind(const std::vector<int>& vals) {
	int i  = 0;
	for(auto val : vals) {
		sqlite3_bind_int(statement_, i, val);
	}
}

template<>
void Query::Bind(const std::vector<double>& vals) {
	int i  = 0;
	for(auto val : vals) {
		sqlite3_bind_double(statement_, i, val);
	}
}

template<>
void Query::Bind(const std::vector<std::string>& vals) {
	int i  = 0;
	for(auto val : vals) {
		int res = sqlite3_bind_text(statement_, i, val.c_str(), -1, SQLITE_STATIC);
		if (res != SQLITE_OK)
			break;
	}
}

template<>
void Query::Bind(size_t id, int val) {
	sqlite3_bind_int(statement_, id, val);
}

template<>
void Query::Bind(size_t id, double val) {
	sqlite3_bind_double(statement_, id, val);
}

template<>
void Query::Bind(size_t id, const std::string& val) {
	sqlite3_bind_text(statement_, id, val.c_str(), -1, SQLITE_STATIC);
}

template <>
double ReadQuery::GetColumn<double>(int col_num) {
	return sqlite3_column_double(statement_, col_num);
}

template <>
int ReadQuery::GetColumn<int>(int col_num) {
	return sqlite3_column_int(statement_, col_num);
}

template <>
std::string ReadQuery::GetColumn<std::string>(int col_num) {
	std::stringstream ss;
	size_t sz = sqlite3_column_bytes(statement_, col_num);
	const unsigned char* text = sqlite3_column_text(statement_, col_num);
	for(size_t i = 0; i < sz; ++i) {
		ss << text[i];
	}
	return ss.str();
}


