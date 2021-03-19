#pragma once

#include <string>
#include <sstream>
#include "connector.h"
#include "sqlite3.h"
#include <vector>
#include "error_handler.h"


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

		virtual void Prepare(Connection conn);

		int ExecuteStep();

		void StatementReset();

		void Finalize();

		template <typename T>
		void Bind(const std::vector<T>& vals);

		template <typename T>
		void Bind(size_t id, T val);

		static const int row_limit = 4096;

		virtual sqlite3_stmt* GetStatement();

		virtual ~Query() = default;
		std::string cmd_;

private:
		bool finalized_;
		sqlite3_stmt *q_statement_;
		void ExecuteMany(int num);
};

class ReadQuery : public Query{
public:
		explicit ReadQuery(const std::string& q) : Query(q), statement_(nullptr){}

		template<typename T>
		std::vector<T> GetAllColumn(int col_num){
			std::vector<T> vals;
			for(int i  = 0; i < row_limit; ++i)  {
				int res = ExecuteStep();
				if (res == SQLITE_DONE || res == SQLITE_OK)
					break;
				if (res != SQLITE_ROW) {
					LOG(res);
					break;
				}
				vals.push_back(sqlite3_column_int(statement_, 0));
			}
			Finalize();
			return vals;
		}
		sqlite3_stmt * GetStatement() override;
		void Prepare(Connection conn) override;

private:
		template<typename T>
		T GetColumn(int col_num);
		std::string cmd_;
		sqlite3_stmt *statement_;
};

class WriteQuery : public Query{
public:
		WriteQuery(const std::string& q) : Query(q), statement_(nullptr) {}
		sqlite3_stmt * GetStatement() override;
		void Prepare(Connection conn) override;
private:
		std::string cmd_;
		sqlite3_stmt *statement_;
};