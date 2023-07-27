#include "../mysqlconncpp-static/include/mysqlx/xdevapi.h"
#include <iostream>
#include <thread>
#include <vector>
#include "test.h"


using namespace mysqlx;

void test(Session session){
    Schema db = session.getSchema("addressbook");
    Table myTable = db.getTable("addresses");
    RowResult myResult = myTable.select("name", "address").execute();
    std::cout << "Printing all rows from the table ..." << std::endl;
    auto rows = myResult.fetchAll();
    for (auto row : rows) {
        std::cout << row[0] << ", " << row[1] << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
