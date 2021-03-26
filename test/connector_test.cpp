#include "gtest/gtest.h"
#include "connector.h"

TEST(connector, construction) {
    Connector conn("test.db");
    conn.OpenDB();
    ASSERT_TRUE(conn.GetDB().operator bool());
}