#include "gtest/gtest.h"
#include "query.h"
#include "connector.h"
#include <vector>

class TestConnection {
public:
			TestConnection() : con(Connector("../../test/test.sqlite")){
			con.OpenDB(OpenFlags::READWRITE | OpenFlags::CREATE);
			Query q("CREATE TABLE IF NOT EXISTS test(a,b,c);");
			q.Prepare(con);
			q.ExecuteStep();
		}
		Connector& GetConnection() {
		return con;
		}
		~TestConnection() {
			Query dq("DROP TABLE test;");
			dq.Prepare(con);
			dq.ExecuteStep();
		}
private:
		Connector con;
};

TEST(query_test, usual_usage) {
	TestConnection con;
	WriteQuery wq("INSERT INTO test VALUES(1,2,3);");
	wq.Prepare(con.GetConnection());
	for (int i = 0; i < 10; ++i) {
		wq.ExecuteStep();
		wq.StatementReset();
	}
	ReadQuery rq("SELECT * FROM test;");
	rq.Prepare(con.GetConnection());
	rq.ExecuteStep();
	std::vector<int> a = rq.GetAllColumn<int>(0);
	ASSERT_EQ(a.size(), 10);
	ASSERT_EQ(a.at(0), 1);
	std::vector<int> b = rq.GetAllColumn<int>(1);
	ASSERT_EQ(b.at(0), 2);
	std::vector<int> c = rq.GetAllColumn<int>(1);
	ASSERT_EQ(c.at(0), 2);
	rq.StatementReset();
}

TEST(query_test, empty_table) {
	TestConnection con;
	ReadQuery rq("SELECT * FROM test;");
	rq.Prepare(con.GetConnection());
	ASSERT_EQ(rq.ExecuteStep(), SQLITE_OK);
	ASSERT_EQ(rq.GetColumn<int>(1), 0);
	rq.StatementReset();
}

TEST(DISABLED_query_test, many_values) {
	TestConnection con;
	for (int i = 0; i < 4100; ++i) {
		std::ostringstream os;
		os << "INSERT INTO test VALUES("
			<< i << ',' << (i + 1)<< ',' << (i + 2) << ");";
		WriteQuery wq(os.str());
		wq.Prepare(con.GetConnection());
		wq.ExecuteStep();
	}
	ReadQuery rq("SELECT * FROM test;");
	rq.Prepare(con.GetConnection());
	rq.ExecuteStep();
	std::vector<int> a = rq.GetAllColumn<int>(0);
	ASSERT_EQ(a.size(), 4096);
	ASSERT_EQ(a.at(4095), 4096);
}

