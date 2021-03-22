#pragma once
#include <utility>
//#include <yaml-cpp/yaml.h>
#include "sqlite3.h"

class Connection {
public:
    Connection(std::string name);
    sqlite3* db;
    const std::string name;
};

class Connector {

public:
    explicit Connector(std::string name) : connection_(std::move(name)) {};

    int OpenDB();

    int ExecuteManagingQuery(const std::string &query);

    int CloseConnection();

    Connection& GetConnection();

    ~Connector();

private:
    const char *SETTINGS = "PRAGMA page_size = 4096; "
                           "PRAGMA cache_size = 200; "
                           "PRAGMA journal_mode = WAL";
    Connection connection_;
    bool open;

    int ApplyDBSettings(const std::string &filename);


};