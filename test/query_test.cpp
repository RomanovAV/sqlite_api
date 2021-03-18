#include "gtest/gtest.h"
#include "query.h"
#include "connector.h"

TEST(query_test, usual_usage) {
	Query q("CREATE TABLE IF NOT EXISTS test(a,b,c);");
	Connector con("../../test/test.sqlite");
	ASSERT_EQ(con.OpenDB(), SQLITE_OK);
	q.Prepare(con.GetConnection());
	q.ExecuteStep();
	q.Finalize();
	WriteQuery wq("INSERT INTO test VALUES(1,2,3);");
	wq.Prepare(con.GetConnection());
	wq.ExecuteStep();
	wq.Finalize();
	ReadQuery rq("SELECT * FROM test;");
	rq.Prepare(con.GetConnection());
	rq.ExecuteStep();
	ASSERT_EQ(rq.GetColumn<int>(1), 1);
	wq.Finalize();
}