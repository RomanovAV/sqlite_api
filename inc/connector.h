#pragma once
#include <utility>
//#include <yaml-cpp/yaml.h>
#include "sqlite3.h"
#include <memory>

class Connector {

public:
    explicit Connector(const std::string& name) : _name(std::move(name)) {};

    int OpenDB();

    int ExecuteManagingQuery(const std::string &query);

    int CloseConnection();

    ~Connector();

    Connector(const Connector&) = delete;
		Connector(Connector&&) = delete;
    Connector& operator=(const Connector&) = delete;
    Connector& operator=(Connector&&) = delete;

    std::shared_ptr<sqlite3> db;

private:
    const char* _SETTINGS = "PRAGMA page_size = 4096; "
                           "PRAGMA cache_size = 200; "
                           "PRAGMA journal_mode = WAL";

		const std::string _name;
    bool _open;

    int ApplyDBSettings(const std::string &filename);
		sqlite3* Init();
};