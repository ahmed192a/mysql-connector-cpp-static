#include "mysqlconncpp-static/include/mysqlx/xdevapi.h"
#include <iostream>
#include <thread>
#include <vector>

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
}

int main(){
// Connect to the MySQL server
    mysqlx::Session session("localhost", 33060, "root", "root");

    // create a database 
    session.sql("CREATE DATABASE IF NOT EXISTS addressbook").execute();
    mysqlx::Schema db = session.getSchema("addressbook");

    // drop the table if exists
    session.sql("DROP TABLE IF EXISTS addressbook.addresses").execute();

    // create a table with three columns id , name and address
    session.sql("CREATE TABLE IF NOT EXISTS addressbook.addresses (id INT NOT NULL AUTO_INCREMENT, name VARCHAR(45) NULL, address VARCHAR(45) NULL, PRIMARY KEY (id))").execute();
    mysqlx::Table myTable = db.getTable("addresses");

    // Insert some data into table  Asynchronous API
    myTable.insert("name", "address").values("John Doe", "123 Main Street").execute();
    myTable.insert("name", "address").values("Jane Doe", "124 Main Street").execute();
    myTable.insert("name", "address").values("Joe Doakes", "125 Main Street").execute();

    // close the session
    session.close();


    Client cli("root:root@localhost/addressbook", ClientOption::POOL_MAX_SIZE, 4, ClientOption::POOL_MAX_IDLE_TIME, 0, ClientOption::POOL_QUEUE_TIMEOUT, 0);
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++){
        threads.push_back(std::thread(test, cli.getSession()));
    }

    for (auto& th : threads){
        th.join();
    }

    cli.close();

    return 0;
}