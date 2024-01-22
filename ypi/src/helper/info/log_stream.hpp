/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** log_stream
*/

#ifndef EXNG_LOG_STREAM_HPP
#define EXNG_LOG_STREAM_HPP

#include <iostream>
#include <string>
#include <sstream>

namespace exng {

    class logger;

    class log_stream {
    public:
        log_stream(logger& logger, const std::string& level);
        ~log_stream();

        template <typename T>
        log_stream& operator<<(const T& msg) {
            ss_ << msg;
            return *this;
        }

    private:
        logger& logger_;
        std::string level_;
        std::stringstream ss_;
    };
}

#endif //EXNG_LOG_STREAM_HPP