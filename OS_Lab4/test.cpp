#define BOOST_TEST_MODULE IPCMessagingTest
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <string>

#define MAX_MESSAGE_LENGTH 20

BOOST_AUTO_TEST_CASE(test_message_write_read) {
    std::string test_file = "test_messages.bin";
    std::ofstream file(test_file.c_str(), std::ios::binary | std::ios::trunc);
    BOOST_REQUIRE(file);

    char empty[MAX_MESSAGE_LENGTH] = { 0 };
    for (int i = 0; i < 5; ++i) {
        file.write(empty, MAX_MESSAGE_LENGTH);
    }
    file.close();

    std::fstream rw_file(test_file.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    BOOST_REQUIRE(rw_file);

    std::string message = "TestMsg";
    rw_file.seekp(0);
    rw_file.write(message.c_str(), message.size());
    rw_file.close();

    std::ifstream read_file(test_file.c_str(), std::ios::binary);
    BOOST_REQUIRE(read_file);

    char buffer[MAX_MESSAGE_LENGTH] = { 0 };
    read_file.read(buffer, MAX_MESSAGE_LENGTH);
    read_file.close();

    BOOST_CHECK_EQUAL(std::string(buffer), message);

    remove(test_file.c_str());
}
