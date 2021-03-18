#include <iostream>
#include <utility>
#include "../inc/connector.h"

Connection::Connection(std::string name) : db(nullptr), name(std::move(name)) {}

Connector::~Connector() {
	CloseConnection();
}

int Connector::OpenDB() {
	if (sqlite3_open_v2(connection_.name.c_str(), &connection_.db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
		std::cerr << "Error DB opening" << sqlite3_errmsg(connection_.db) << std::endl;
		return -1;
	}
	ApplyDBSettings("dbsettings.yml");
	return 0;
}

int Connector::ApplyDBSettings(const std::string& filename) {
	if (sqlite3_exec(connection_.db, SETTINGS, 0, 0, 0)) {
		std::cerr << "Error DB settings loading" << sqlite3_errmsg(connection_.db) << std::endl;
		return -1;
	}
	else return 0;
}

int Connector::CloseConnection() {
	return sqlite3_close(connection_.db) == SQLITE_OK ? 0 : -1;
}

int Connector::ExecuteManagingQuery(const std::string& query) {
	if (sqlite3_exec(connection_.db, query.c_str(), 0, 0, 0)) {
		std::cerr << "Error query execution" << sqlite3_errmsg(connection_.db) << std::endl;
		return -1;
	}
	return 0;
}

Connection Connector::GetConnection() {
    return connection_;
}


