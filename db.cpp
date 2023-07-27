#include <thread>
#include <chrono>
#include "mysqlconncpp-static/include/mysqlx/xdevapi.h"
#include <iostream>
#include <mutex>

class DatabaseAdapter {
public:
    DatabaseAdapter(const std::string& host, unsigned int port, const std::string& user, const std::string& password, const std::string& schema)
        : host_(host), port_(port), user_(user), password_(password), schema_(schema), running_(false),
        session_(mysqlx::Session(host, port, user, password))
    {
        // check if the schema exists
        if (!session_.getSchema(schema_).existsInDatabase()) {
            // create the schema if it does not exist
            session_.sql("CREATE DATABASE " + schema_).execute();
            // create data table with two cols id auto increment and d1 varchar(45)
            session_.sql("CREATE TABLE " + schema_ + ".data (id INT NOT NULL AUTO_INCREMENT, d1 VARCHAR(45) NULL, PRIMARY KEY (id))").execute();
        }
        session_.startTransaction();
        running_ = true;
        thread_ = std::thread(&DatabaseAdapter::transactionManager, this);
    }

    ~DatabaseAdapter() {
        if (running_) {
            running_ = false;
            thread_.join();
        }
    }

    void insertData(const std::string& data) {
        // Lock the mutex to prevent concurrent access to the session_
        std::lock_guard<std::mutex> lock(mutex_);

        // Perform database insert operation within the current transaction
        try {
            mysqlx::Table table = session_.getSchema(schema_).getTable("data");
            table.insert("d1").values(data).execute();
        } catch (const std::exception& e) {
            // Handle any exceptions or errors
            // For simplicity, print the error message here
            std::cerr << "Error while inserting data: " << e.what() << std::endl;
        }
    }
    // select *  data from table
    void selectData() {
        // Lock the mutex to prevent concurrent access to the session_
        std::lock_guard<std::mutex> lock(mutex_);

        // Perform database insert operation within the current transaction
        try {
            mysqlx::Table table = session_.getSchema(schema_).getTable("data");
            mysqlx::RowResult res = table.select("*").execute();
            std::cout << "id\t" << "d1" << std::endl;
            while (mysqlx::Row row = res.fetchOne()) {
                std::cout << row[0] << "\t" << row[1] << std::endl;
            }
        } catch (const std::exception& e) {
            // Handle any exceptions or errors
            // For simplicity, print the error message here
            std::cerr << "Error while selecting data: " << e.what() << std::endl;
        }
    }


private:
    void transactionManager() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            try {
                // Lock the mutex before committing and starting a new transaction
                std::lock_guard<std::mutex> lock(mutex_);
                std::cout << "Committing and starting a new transaction" << std::endl;

                session_.commit();
                session_.startTransaction();
            } catch (const std::exception& e) {
                // Handle any exceptions or errors
                // For simplicity, print the error message here
                std::cerr << "Error while managing transactions: " << e.what() << std::endl;

                // Rollback and start a new transaction even in case of an error
                session_.rollback();
                session_.startTransaction();
            }
        }
    }

private:
    std::string host_;
    unsigned int port_;
    std::string user_;
    std::string password_;
    std::string schema_;
    mysqlx::Session session_;
    std::thread thread_;
    bool running_;
    std::mutex mutex_; // Mutex to protect the session_
};

int main() {
    // Initialize the database adapter with your connection details
    DatabaseAdapter dbAdapter("localhost", 33060, "root", "root", "test_d");

    while (true) {
        std::string data;
        std::cout << "Enter data to insert into the database: ";
        std::getline(std::cin, data);

        if (data == "exit") {
            break;
        }

        // Insert the data into the database
        dbAdapter.insertData(data);
    }

    // ...
    
    // The destructor of DatabaseAdapter will join the demon thread and clean up resources.
    return 0;
}
