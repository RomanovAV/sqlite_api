#pragma once
#include <string>
#include "connector.h"

class DB{
public:
		explicit DB(const std::string name);

		template<typename T>
		T SelectOne(const std::string& query) {
			
		}

		template<typename T>
		T SelectMany(const std::string& query) {

		}
private:
		Connector conn;
};