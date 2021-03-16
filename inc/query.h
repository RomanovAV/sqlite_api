#pragma once

#include <string>
#include <sstream>
#include "connector.h"
#include "sqlite3.h"
#include <vector>

class Query {
public:
		Query(std::string q);

		void Prepare(Connection& db);

		int ExecuteStep();

		void StatementReset();

		void Finalize();

		template <typename T>
		void Bind(const std::vector<T>& vals);

		template <typename T>
		void Bind(size_t id, T val);

private:
		std::string cmd;
		bool finalized;
		sqlite3_stmt *statement;
};

class ReadQuery : public Query{
public:
		ReadQuery(std::string q);

		template<typename T>
		T GetColumn(int col_num) {
		}

private:
		sqlite3_stmt *statement;
};

template <typename T>
class WriteQuery : public Query{
public:
		WriteQuery(std::string q);
private:
		sqlite3_stmt *statement;
};