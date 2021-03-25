#include <iostream>
#include <utility>
#include "sqlite3.h"
#include "../inc/connector.h"

Connector::~Connector() {
	CloseConnection();
}

sqlite3* Connector::Init() {
	sqlite3* tmp = nullptr;
	if (!_open) {
		if (sqlite3_open_v2(_name.c_str(), &tmp,
												SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
			std::cerr << "Error DB opening" << sqlite3_errmsg(db.get()) << std::endl;
		}
	}
	return tmp;
}

int Connector::OpenDB() {
	db = std::shared_ptr<sqlite3>(Init());
	ApplyDBSettings("dbsettings.yml");
	return 0;
}

int Connector::ApplyDBSettings(const std::string& filename) {
	if (_open) {
		if (sqlite3_exec(db.get(), _SETTINGS, 0, 0, 0)) {
			std::cerr << "Error DB settings loading" << sqlite3_errmsg(db.get())
				<< std::endl;
			return -1;
		}
	}
	return 0;
}

int Connector::CloseConnection() {
	if (_open)
		return sqlite3_close(db.get()) == SQLITE_OK ? 0 : -1;
	return -1;
}

int Connector::ExecuteManagingQuery(const std::string& query) {
	if (sqlite3_exec(db.get(), query.c_str(), 0, 0, 0)) {
		std::cerr << "Error query execution" << sqlite3_errmsg(db.get()) << std::endl;
		return -1;
	}
	return 0;
}

