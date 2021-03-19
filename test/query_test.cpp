#include "gtest/gtest.h"
#include "query.h"
#include "connector.h"
#include <vector>

TEST(query_test, usual_usage) {
	Query q("CREATE TABLE IF NOT EXISTS test(a,b,c);");
	Connector con("../../test/test.sqlite");
	ASSERT_EQ(con.OpenDB(), SQLITE_OK);
	q.Prepare(con.GetConnection());
	q.ExecuteStep();
	q.Finalize();
	for (int i = 0; i < 10; ++i) {
		WriteQuery wq("INSERT INTO test VALUES(1,2,3);");
		wq.Prepare(con.GetConnection());
		wq.ExecuteStep();
		wq.Finalize();
	}
	ReadQuery rq("SELECT * FROM test;");
	rq.Prepare(con.GetConnection());
	rq.ExecuteStep();
	std::vector<int> a = rq.GetAllColumn<int>(0);
	ASSERT_EQ(a.size(), 10);
	ASSERT_EQ(a.at(0), 1);
	rq.StatementReset();
	Query dq("DROP TABLE test;");
	dq.Prepare(con.GetConnection());
	dq.ExecuteStep();
	dq.Finalize();
}