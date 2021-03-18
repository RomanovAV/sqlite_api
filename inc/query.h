#pragma once

#include <string>
#include <sstream>
#include "connector.h"
#include "sqlite3.h"
#include <vector>

enum class QueryState {
		START,
		PREPARED,
		STEP_DONE,
		RESET,
		FINALIZED
};

class Query {
public:
		Query(const std::string& q);

		void Prepare(Connection db);

		void ExecuteStep();

		void StatementReset();

		void Finalize();

		template <typename T>
		void Bind(const std::vector<T>& vals);

		template <typename T>
		void Bind(size_t id, T val);

private:
		std::string cmd_;
		bool finalized_;
		sqlite3_stmt *statement_;

		void ExecuteMany(int num);
};

class ReadQuery : public Query{
public:
		ReadQuery(const std::string& q) : Query(q), statement_(nullptr){}

		template<typename T>
		T GetColumn(int col_num);

private:
		sqlite3_stmt *statement_;
};

class WriteQuery : public Query{
public:
		WriteQuery(const std::string& q) : Query(q), statement_(nullptr) {}
private:
		sqlite3_stmt *statement_;
};