#include <stdio.h>
#include "sqlite3.h"
#include <iostream>

const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";

const char* SETTINGS = "PRAGMA page_size = 4096; "
											 "PRAGMA cache_size = 200; "
												"PRAGMA journal_mode = WAL";

int main(int argc, char **argv){

	sqlite3 *db = 0;
	char *err = 0;

	if( sqlite3_open("my_cosy_database.dblite", &db) )
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
	else if (sqlite3_exec(db, SETTINGS, 0, 0, &err))
	{
		sqlite3_exec(db, SQL, 0, 0, &err);
		fprintf(stderr, "Ошибка SQL: %sn", err);
		sqlite3_free(err);
	}
	sqlite3_close(db);
	return 0;
}
