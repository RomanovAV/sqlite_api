#include "gtest/gtest.h"
#include "connector.h"

TEST(connector, construction) {
    Connector conn("test.db");
    ASSERT_EQ(conn.OpenDB(), 0);
}