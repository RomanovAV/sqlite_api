#pragma once

#include <string>
#include <sstream>
#include "connector.h"
#include "sqlite3.h"
#include <vector>
#include "error_handler.h"
#include <memory>


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

		virtual void Prepare(Connector& conn);

		int ExecuteStep();

		void StatementReset();

		void Finalize();

		template <typename T>
		void Bind(const std::vector<T>& vals);

		template <typename T>
		void Bind(size_t id, T val);

		static const int row_limit = 4096;

		virtual sqlite3_stmt * GetStatement();
		virtual sqlite3_stmt* PrepareStmt(Connector& conn);
		virtual ~Query() = default;

		std::string cmd_;

private:

		bool finalized_;
		std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> _q_statement;
		void ExecuteMany(int num);
};

class ReadQuery : public Query{
public:
		explicit ReadQuery(const std::string& q) : Query(q), _statement(nullptr, sqlite3_finalize){}


		template<typename T>
		std::vector<T> GetAllColumn(int col_num){
			return GetColumn<T>(col_num, row_limit);
		}

		template<typename T>
		std::vector<T> GetColumn(int col_num, int row_count){
			std::vector<T> vals;
			for(int i  = 0; i < row_count && i < row_limit; ++i)  {
				int res = ExecuteStep();
				if (res == SQLITE_ROW || res == SQLITE_OK) {
					vals.push_back(GetColumn<T>(col_num));
					if (res == SQLITE_OK)
						break;
				}
				else {
					LOG(res);
					break;
				}
			}
			return vals;
		}

		template<typename T>
		T GetColumn(int col_num);

		sqlite3_stmt * GetStatement() override;

		void Prepare(Connector& conn) override;

private:
		std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> _statement;
};

class WriteQuery : public Query {
public:
		explicit WriteQuery(const std::string& q) : Query(q), _statement(nullptr, sqlite3_finalize){}

		sqlite3_stmt * GetStatement() override;

		void Prepare(Connector& conn) override;

private:
		std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>  _statement;
};

class ServiceQuery : public Query {

};