/*
 *  Author: Clément Thomas
 *  Create Time: 2023-07-07 02:58:14
 *  Modified by: Clément Thomas
 *  Modified time: 2023-07-07 03:12:10
 *  Description:
 */

#ifndef YPI_LOGGER_HPP
#define YPI_LOGGER_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <regex>
#include "../../lib_headers/sfml.hpp"

#include "log_stream.hpp"
#include "log_buffer.hpp"

namespace ypi {

    class logger {

        friend class log_stream;

    public:

        static void enableFileLogging() {
            if (!getInstance().m_logFileInitialized) {
                try {
                    getInstance().createLogFile();
                }
                catch (const std::exception& e) {
                    error() << "Error: could not create log file: "
                        << std::string(e.what()) << ". File logging will be disabled.";
                    return;
                }
            }

            getInstance().m_shouldLogToFile = true;
        }

        static void disableFileLogging() {
            getInstance().m_shouldLogToFile = false;
        }

        static void log(const std::string& message) {
            getInstance().logImpl("\033[37m\e[1m[LOG]\033[0m\e[0m " + message);
        }

        static void debug(const std::string& message) {
            getInstance().logImpl("\033[36m\e[1m[DEBUG]\033[0m\e[0m " + message);
        }


        static void warn(const std::string& message) {
            getInstance().logImpl("\033[33m\e[1m[WARN]\033[0m\e[0m " + message);
        }

        static void error(const std::string& message) {
            getInstance().logImpl("\033[31m\e[1m[ERROR]\033[0m\e[0m " + message);
        }

        static void success(const std::string& message) {
            getInstance().logImpl("\033[32m\e[1m[SUCCESS]\033[0m\e[0m " + message);
        }

        static log_stream log() {
            return log_stream(getInstance(), "\033[37m\e[1m[LOG]\033[0m\e[0m");
        }

        static log_stream debug() {
            return log_stream(getInstance(), "\033[36m\e[1m[DEBUG]\033[0m\e[0m");
        }

        static log_stream warn() {
            return log_stream(getInstance(), "\033[33m\e[1m[WARN]\033[0m\e[0m");
        }

        static log_stream error() {
            return log_stream(getInstance(), "\033[31m\e[1m[ERROR]\033[0m\e[0m");
        }

        static log_stream success() {
            return log_stream(getInstance(), "\033[32m\e[1m[SUCCESS]\033[0m\e[0m");
        }

        static void stopLogging() {
            getInstance().stopFlag = true;
            getInstance().logCondition.notify_all();
        }

    private:
        static logger& getInstance() {
            static logger instance;
            return instance;
        }

        bool m_shouldLogToFile = false; // Ajout d'un booléen pour activer ou désactiver l'écriture dans un fichier
        bool m_logFileInitialized = false; // Ajout d'un booléen pour vérifier si le fichier a été initialisé pour cette session

        LoggerStreambuf m_streambuf;

        std::mutex logMtx;
        std::deque<std::string> logQueue; // Modification de queue à deque
        std::thread loggerThread;
        std::atomic<bool> stopFlag;
        std::condition_variable logCondition;
        std::mutex conditionMtx;

        std::ofstream logFile;
        std::deque<std::string> fileLogQueue; // Modification de queue à deque
        std::thread fileLoggerThread;

        std::regex colorCodeRegex = std::regex("\033\\[[0-9;]*m"); // Utilisation de regex pour supprimer les codes de couleur

        logger() : stopFlag(false) {
            loggerThread = std::thread(&logger::logMessages, this);
            fileLoggerThread = std::thread(&logger::logToFile, this);

            // Redirigez sf::err vers loggerStreambuf
            sf::err().rdbuf(&m_streambuf);
        }

        ~logger() {
            {
                std::lock_guard<std::mutex> lock(logMtx);
                stopFlag = true;
            }
            logCondition.notify_all(); // Notifie tous les threads
            loggerThread.join();
            fileLoggerThread.join(); // Joignez le nouveau thread
            logFile.close(); // Ferme le fichier
        }

        void createLogFile() {
            if (!std::filesystem::exists("logs")) {
                std::filesystem::create_directory("logs");
            }

            // Obtenez l'heure et la date actuelles
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            struct tm* timeinfo = std::localtime(&now_c);

            // Générez un nom de fichier unique
            std::ostringstream filename;
            filename << "logs/" << std::put_time(timeinfo, "%Y-%m-%d-%H-%M-%S") << ".log";

            // Ouvrez le fichier
            logFile.open(filename.str(), std::ios::out);
            if (!logFile.is_open()) {
                throw std::runtime_error("Error: could not open log file");
            }

            m_logFileInitialized = true;
        }

        logger(const logger&) = delete;
        logger& operator=(const logger&) = delete;

        void logImpl(const std::string& message) {
            std::lock_guard<std::mutex> lock(logMtx);
            std::ostringstream logMessage;
            logMessage << logCurrentDate() << message; // Utilisation de ostringstream pour la concaténation de chaînes
            logQueue.push_back(logMessage.str()); // Utilisation de push_back avec deque
            logCondition.notify_all();
        }

        std::string logCurrentDate() {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            struct tm* timeinfo = std::localtime(&now_c);

            std::ostringstream date;
            date << std::put_time(timeinfo, "%c") << " ";

            return date.str();
        }

        void logToFile() {
            while (true) {
                std::string logEntry;
                {
                    std::unique_lock<std::mutex> lock(logMtx);
                    logCondition.wait(lock, [this] { return !fileLogQueue.empty() || stopFlag; });

                    if (stopFlag && fileLogQueue.empty()) {
                        break;
                    }

                    if (!fileLogQueue.empty()) {
                        logEntry = fileLogQueue.front();
                        fileLogQueue.pop_front(); // Utilisation de pop_front avec deque
                    }
                }
                if (!logEntry.empty()) {
                    // Remove color codes
                    logEntry = std::regex_replace(logEntry, colorCodeRegex, ""); // Réutilisation de l'expression régulière
                    logFile << logEntry << std::endl;
                    logFile.flush();
                }
            }
        }

        void logMessages() {
            while (true) {
                std::string logEntry;
                {
                    std::unique_lock<std::mutex> lock(logMtx);
                    logCondition.wait(lock, [this] { return !logQueue.empty() || stopFlag; });

                    if (stopFlag && logQueue.empty()) {
                        break;
                    }

                    if (!logQueue.empty()) {
                        logEntry = logQueue.front();
                        logQueue.pop_front(); // Utilisation de pop_front avec deque
                        if (m_shouldLogToFile)
                            fileLogQueue.push_back(logEntry); // Utilisation de push_back avec deque
                    }
                }
                if (!logEntry.empty()) {
                    std::cout << logEntry << std::endl;
                }
            }
        }
    };
}

#endif //YPI_LOGGER_HPP