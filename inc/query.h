#pragma once

#include <string>
#include <sstream>
#include "connector.h"
#include "sqlite3.h"

class BlobResponse {

};

class Query {
public:
    Query(std::string q);

private:
    void Prepare(Connection& db);
    int ExecuteStep();
    void StatementReset();
    template <typename T>
    T GetColumn (int col_num) {
    }

    std::string cmd;
    sqlite3_stmt *statement;
};

