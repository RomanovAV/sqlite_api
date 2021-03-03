#pragma once
#include <utility>
//#include <yaml-cpp/yaml.h>
#include "sqlite3.h"

class Connector {

public:
	explicit Connector(std::string name) : db_name(std::move(name)) {};
	int OpenDB();
	int ExecuteQuery(const std::string& query);
	~Connector();

private:
		const char* SETTINGS = "PRAGMA page_size = 4096; "
											 "PRAGMA cache_size = 200; "
											 "PRAGMA journal_mode = WAL";
		sqlite3 *db = 0;
		const std::string db_name;
		int ApplyDBSettings(const std::string& filename);
		void CloseConnection();
};