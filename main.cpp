#include <iostream>
#include "sqlite3.h"
#include "inc/connector.h"

const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";

int main(int argc, char **argv){
	Connector conn("sb_unit.db");
	conn.OpenDB();
	conn.ExecuteQuery(SQL);
	return 0;
}
