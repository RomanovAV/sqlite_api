#pragma once
#include "sqlite3.h"
#include <iostream>

class SQLiteError{
public:
		template<typename T, typename U>
		static int CheckError(T func, U arg) {
			int err = func(arg);
			if (err != SQLITE_OK && err != SQLITE_DONE && err != SQLITE_ROW) {
				std::cerr << "An error " << err << " occured" << "\n";
				return SQLITE_ERROR;
			}
			if (err == SQLITE_ROW)
				return SQLITE_ROW;
			return SQLITE_OK;
		}
};


