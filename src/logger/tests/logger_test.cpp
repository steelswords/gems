#include "../logger.hpp"
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <functional>
#include <map>

using namespace gems;

gems::Logger getLogger()
{
    Logger logger("TESTLOG", [](const char *message) {
            std::cout << message << std::endl;
            });
    return logger;
}

// Function to capture stdout
std::string captureStdout(std::function<void()> func) {
    std::streambuf* original = std::cout.rdbuf();
    std::ostringstream capture;
    std::cout.rdbuf(capture.rdbuf());

    func();

    std::cout.rdbuf(original);
    return capture.str();
}

// Function to capture stderr
std::string captureStderr(std::function<void()> func) {
    std::streambuf* original = std::cerr.rdbuf();
    std::ostringstream capture;
    std::cerr.rdbuf(capture.rdbuf());

    func();

    std::cerr.rdbuf(original);
    return capture.str();
}

void BasicDebugLog()
{
    Logger logger = getLogger();
    std::string message = "Something lorem ipsum";
    std::string output = captureStdout(
            [&]()
            {
                logger.debug(message.c_str());
            }
        );
    // Gotcha: Logger outputs a newline at the end, so we add that.
    message += "\n";
    std::cout << "Expected to see this on stdout: \"" << message << "\", got this: \"" << output << "\"" << std::endl;
    assert(output == message);
}

void printUsage(const char* argv0, std::map<std::string, std::function<void()>> &tests)
{
    std::cerr << "\n\nUsage: " << argv0 << " <testName>\n"
        << "testName can be one of the following:\n\n";
    for (const auto &pair : tests)
    {
        std::cerr << "  * " << pair.first << "\n";
    }
    std::cerr << std::endl;
}

int main(int argc, char *argv[])
{
    std::map<std::string, std::function<void()> > tests {
        { "BasicDebugLog", []() { BasicDebugLog(); } }
    };

    if (argc != 2)
    {
        printUsage(argv[0], tests);
        return -1;
    }

    // Extract test to run
    std::stringstream transform;
    std::string testToRun;
    transform << argv[1];
    transform >> testToRun;

    auto it = tests.find(testToRun);
    if (it == tests.end())
    {
        std::cerr << "Unknown test called: \"" << testToRun << "\"" << std::endl;
        return -2;
    }
    else
    {
        it->second();
    }
    std::cout << "Finished running tests." << std::endl;
    return 0;
}
