#include <iostream>
#include <utility>
#include "../inc/connector.h"

int Connector::OpenDB() {
	if (sqlite3_open(connection.name.c_str(), &connection.db)) {
		std::cerr << "Error DB opening" << sqlite3_errmsg(connection.db) << std::endl;
		return -1;
	}
	ApplyDBSettings("dbsettings.yml");
	return 0;
}

int Connector::ApplyDBSettings(const std::string& filename) {
	if (sqlite3_exec(connection.db, SETTINGS, 0, 0, 0)) {
		std::cerr << "Error DB settings loading" << sqlite3_errmsg(connection.db) << std::endl;
		return -1;
	}
	else return 0;
}

Connector::~Connector() {
	CloseConnection();
}

int Connector::CloseConnection() {
	return sqlite3_close(connection.db) == SQLITE_OK ? 0 : -1;
}

int Connector::ExecuteManagingQuery(const std::string& query) {
	if (sqlite3_exec(connection.db, query.c_str(), 0, 0, 0)) {
		std::cerr << "Error query execution" << sqlite3_errmsg(connection.db) << std::endl;
		return -1;
	}
	return 0;
}

Connection Connector::GetConnection() {
    return connection;
}

Connection::Connection(std::string name) : db(nullptr), name(std::move(name)) {}
