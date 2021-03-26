#include "query.h"
#include <string>
#include <vector>
#include <utility>
#include "sqlite3.h"
#include <sstream>
#include "error_handler.h"


Query::Query(const std::string& q) : cmd_(q), finalized_(false), _q_statement(nullptr, sqlite3_finalize) {}

sqlite3_stmt* Query::PrepareStmt(Connector& conn) {
	sqlite3_stmt* tmp = nullptr;
	sqlite3_prepare_v2(conn.GetDB().get(), Query::cmd_.c_str(), cmd_.size(), &tmp, 0);
	return tmp;
}

void Query::Prepare(Connector& conn) {
	_q_statement.reset(PrepareStmt(conn));
}

int Query::ExecuteStep() {
	if(!finalized_)
    return SQLiteError::CheckError(sqlite3_step,GetStatement());
	return SQLITE_ABORT;
}

void Query::ExecuteMany(int num) {
	if (!finalized_) {
		int err;
		for (int i = 0; i < num; ++i) {
			err = SQLiteError::CheckError(sqlite3_step, GetStatement());
			if (err != SQLITE_DONE && err != SQLITE_ROW)
				break;
			StatementReset();
		}
	}
}

void Query::ResetPrevious() {
	sqlite3_reset(GetStatement());
	sqlite3_clear_bindings(GetStatement());
}

void Query::ExecuteAll() {
	ResetPrevious();
	int res;
	while ((res = sqlite3_step(GetStatement())) == SQLITE_ROW) {}
	if (res != SQLITE_DONE) {
		LOG(res)
	}
}

void Query::StatementReset() {
	if(!finalized_)
		SQLiteError::CheckError(sqlite3_reset, GetStatement());
}

template<>
void Query::Bind<int>(const std::vector<int>& vals) {
	int i  = 0;
	for(const auto& val : vals) {
		sqlite3_bind_int(GetStatement(), i, val);
	}
}

sqlite3_stmt* Query::GetStatement() {
	return _q_statement.get();
}

template<>
void Query::Bind<double>(const std::vector<double>& vals) {
	int i  = 0;
	for(auto val : vals) {
		sqlite3_bind_double(GetStatement(), i, val);
	}
}

template<>
void Query::Bind<std::string>(const std::vector<std::string>& vals) {
	int i  = 0;
	for(const auto& val : vals) {
		int res = sqlite3_bind_text(GetStatement(), i, val.c_str(), -1, SQLITE_STATIC);
		if (res != SQLITE_OK)
			break;
	}
}

template<>
void Query::Bind<int>(size_t id, int val) {
	sqlite3_bind_int(GetStatement(), id, val);
}

template<>
void Query::Bind<double>(size_t id, double val) {
	sqlite3_bind_double(GetStatement(), id, val);
}

template<>
void Query::Bind<const std::string&>(size_t id, const std::string& val) {
	sqlite3_bind_text(GetStatement(), id, val.c_str(), -1, SQLITE_STATIC);
}

sqlite3_stmt* ReadQuery::GetStatement() {
	return _statement.get();
}

void ReadQuery::Prepare(Connector& conn) {
	_statement.reset(PrepareStmt(conn));
}


template <>
double ReadQuery::GetColumn(int col_num) {
	return sqlite3_column_double(GetStatement(), col_num);
}

template <>
int ReadQuery::GetColumn(int col_num) {
	return sqlite3_column_int(GetStatement(), col_num);
}

template <>
std::string ReadQuery::GetColumn(int col_num) {
	std::stringstream ss;
	size_t sz = sqlite3_column_bytes(GetStatement(), col_num);
	const unsigned char* text = sqlite3_column_text(GetStatement(), col_num);
	for(size_t i = 0; i < sz; ++i) {
		ss << text[i];
	}
	return ss.str();
}

sqlite3_stmt* WriteQuery::GetStatement() {
	return _statement.get();
}

void WriteQuery::Prepare(Connector& conn) {
	_statement.reset(PrepareStmt(conn));
}

