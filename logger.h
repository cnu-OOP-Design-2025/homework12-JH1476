
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <string>

class Logger {
private:
    static std::unique_ptr<Logger> instance;
    static std::mutex init_mtx;
    static std::mutex write_mtx;
    std::ofstream logFile;

    Logger() {
        logFile.open("Test/output2.txt", std::ios::trunc);
        if(logFile.is_open()) {
            logFile << "[Init] Logger started." << std::endl;
        }
    }


public:

    ~Logger() {
        std::lock_guard<std::mutex> lock(init_mtx);
        if(logFile.is_open()) {
            logFile << "[Shutdown] Logger closed." << std::endl;
            logFile.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    static Logger* getInstance(const std::string& filename = "Test/output2.txt") {
        std::lock_guard<std::mutex> lock(init_mtx);
        if (instance == nullptr) {
            instance = std::unique_ptr<Logger>(new Logger());
        }
        return instance.get();
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(init_mtx);
        if(logFile.is_open()) {
            logFile << message << std::endl;
        }
    }

};

std::unique_ptr<Logger> Logger::instance;
std::mutex Logger::init_mtx;
std::mutex Logger::write_mtx;

