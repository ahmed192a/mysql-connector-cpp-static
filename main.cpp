#include "mysqlconncpp-static/include/mysqlx/xdevapi.h"
#include <iostream>

int main() {
  try {
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


    // Select all rows from the table
    mysqlx::RowResult myResult = myTable.select("name", "address").execute();

    // Fetch all rows from the result set
    std::cout << "Printing all rows from the table ..." << std::endl;

    auto rows = myResult.fetchAll();
    for (auto row : rows) {
      std::cout << row[0] << ", " << row[1] << std::endl;
    }



    mysqlx::Client  cli ("root:root@localhost/addressbook", mysqlx::ClientOption::POOL_MAX_SIZE, 10, mysqlx::ClientOption::POOL_MAX_IDLE_TIME, 0, mysqlx::ClientOption::POOL_QUEUE_TIMEOUT, 0);
    mysqlx::Session session1 = cli.getSession();
    mysqlx::Schema db2 = session1.getSchema("addressbook");
    mysqlx::Table myTable2 = db2.getTable("addresses");

    // get the number of rows in the table
    std::cout << "in my case Number of rows in the table: " << myTable2.count() << std::endl;

    // drop the table
    session.sql("DROP TABLE IF EXISTS addressbook.addresses").execute();

    // close the session
    session.close();
    session1.close();

    // close the connection
    cli.close();
    

  } catch (const mysqlx::Error &err) {
    std::cout << "ERROR: " << err << std::endl;
    return 1;
  }


  return 0;
}
