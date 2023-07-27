#include "mysqlconncpp-static/include/mysqlx/xdevapi.h"
#include "mysqlx/devapi/settings.h"
#include <iostream>
#include <thread>
#include <vector>
#include "lib/test.h"

using namespace mysqlx;


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

    // open Client Pooll with size 4  and Max Idle Time infinite and Queue Timeout infinite
    Client cli("root:root@localhost/addressbook", ClientOption::POOL_MAX_SIZE, 4, ClientOption::POOL_MAX_IDLE_TIME, 2000, ClientOption::POOL_QUEUE_TIMEOUT, 50000);
    std::vector<std::thread> threads;
    for (int i = 0; i < 20; i++){
        threads.push_back(std::thread(test, cli.getSession()));
    }

    for (auto& th : threads){
        th.join();
    }
    std::cout << "finished" << std::endl;
    // wait for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    cli.close();

    return 0;
}