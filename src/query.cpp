#include "query.h"
#include <string>
#include <utility>
#include "sqlite3.h"

Query::Query(std::string  q) : cmd(std::move(q)) {}

void Query::Prepare(Connection& conn) {
    sqlite3_prepare_v2(conn.db, cmd.c_str(), cmd.size(), &statement, 0);
}

int Query::ExecuteStep() {
    return sqlite3_step(statement);
}

void Query::StatementReset() {
    sqlite3_reset(statement);
}


