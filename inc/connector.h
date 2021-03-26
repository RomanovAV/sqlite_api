#pragma once
#include <utility>
//#include <yaml-cpp/yaml.h>
#include "sqlite3.h"
#include <memory>

enum class OpenFlags {
		READONLY = SQLITE_OPEN_READONLY,
		READWRITE = SQLITE_OPEN_READWRITE,
		CREATE = SQLITE_OPEN_CREATE,
		NOMUTEX = SQLITE_OPEN_NOMUTEX,
		FULLMUTEX = SQLITE_OPEN_FULLMUTEX,
		SHAREDCACHE = SQLITE_OPEN_SHAREDCACHE,
		PRIVATECACH = SQLITE_OPEN_PRIVATECACHE,
		URI = SQLITE_OPEN_URI
};

OpenFlags operator|(const OpenFlags& a, const OpenFlags& b);

class Connector {

public:
    explicit Connector(const std::string& name) : _name(std::move(name)) {};

    int ExecuteManagingQuery(const std::string &query);

    int CloseConnection();

    ~Connector();

    Connector(const Connector&) = delete;
		Connector(Connector&&) = delete;
    Connector& operator=(const Connector&) = delete;
    Connector& operator=(Connector&&) = delete;

		void OpenDB(const OpenFlags& flags);

		std::shared_ptr<sqlite3> GetDB();

private:
    const char* _SETTINGS = "PRAGMA page_size = 4096; "
                           "PRAGMA cache_size = 200; "
                           "PRAGMA journal_mode = WAL";

		const std::string _name;
    bool _open;
		std::shared_ptr<sqlite3> _db;

    int ApplyDBSettings(const std::string &filename);
};