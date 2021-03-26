#include <iostream>
#include <utility>
#include "sqlite3.h"
#include "../inc/connector.h"
#include "error_handler.h"

Connector::~Connector() {
	//CloseConnection();
}

std::shared_ptr<sqlite3> Connector::GetDB() {
	return _db;
}

void Connector::OpenDB() {
	sqlite3* tmp = nullptr;
	auto ret = sqlite3_open_v2(_name.data(), &tmp, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	if (ret != SQLITE_OK) {
		LOG(ret);
		return;
	}
	_db = std::shared_ptr<sqlite3>(tmp, [=](sqlite3* ptr) { sqlite3_close_v2(ptr); }); // this will close the connection eventually when no longer needed.
	_open = true;
	sqlite3_extended_result_codes(_db.get(), true);
	ApplyDBSettings("dbsettings.yml");
}

int Connector::ApplyDBSettings(const std::string& filename) {
	if (_open) {
		if (sqlite3_exec(GetDB().get(), _SETTINGS, 0, 0, 0)) {
			std::cerr << "Error DB settings loading" << sqlite3_errmsg(_db.get())
				<< std::endl;
			return -1;
		}
	}
	return 0;
}

int Connector::CloseConnection() {
	if (_open)
		return sqlite3_close(GetDB().get()) == SQLITE_OK ? 0 : -1;
	return -1;
}

int Connector::ExecuteManagingQuery(const std::string& query) {
	if (sqlite3_exec(GetDB().get(), query.c_str(), 0, 0, 0)) {
		std::cerr << "Error query execution" << sqlite3_errmsg(GetDB().get()) << std::endl;
		return -1;
	}
	return 0;
}


