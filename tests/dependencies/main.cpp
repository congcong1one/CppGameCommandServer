#include <mysql/jdbc.h>
#include <sw/redis++/redis++.h>
#include "probe.pb.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

int main(int argc, char** argv) {
    try {
        auto* driver = sql::mysql::get_mysql_driver_instance();
        if (driver == nullptr) {
            throw std::runtime_error("MySQL driver could not be loaded");
        }

        dependency_test::Probe original;
        original.set_text("dependency round-trip");
        original.set_sequence(42);
        std::string encoded;
        dependency_test::Probe decoded;
        if (!original.SerializeToString(&encoded) || !decoded.ParseFromString(encoded)
            || decoded.text() != original.text() || decoded.sequence() != 42) {
            throw std::runtime_error("Protobuf round-trip failed");
        }

        sw::redis::ConnectionOptions redis_options;
        redis_options.host = "127.0.0.1";
        redis_options.port = 6379;
        redis_options.connect_timeout = std::chrono::seconds(2);
        redis_options.socket_timeout = std::chrono::seconds(2);
        sw::redis::Redis redis(redis_options);

        if (argc == 2 && std::string(argv[1]) == "--local-services") {
            sql::ConnectOptionsMap options;
            options["hostName"] = sql::SQLString("unix:///var/run/mysqld/mysqld.sock");
            options["userName"] = sql::SQLString("root");
            options["password"] = sql::SQLString("");
            options["OPT_CONNECT_TIMEOUT"] = 2;
            std::unique_ptr<sql::Connection> connection(driver->connect(options));
            std::unique_ptr<sql::Statement> statement(connection->createStatement());
            std::unique_ptr<sql::ResultSet> result(statement->executeQuery("SELECT 1"));
            if (!result->next() || result->getInt(1) != 1 || redis.ping() != "PONG") {
                throw std::runtime_error("Local service check failed");
            }
            std::cout << "MySQL SELECT 1 and Redis PING passed\n";
        } else if (argc != 1) {
            throw std::runtime_error("Usage: dependency_smoke [--local-services]");
        }
        std::cout << "MySQL connector, redis++ and generated Protobuf passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
