/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** log_stream
*/

#include "log_stream.hpp"
#include "logger.hpp"

exng::log_stream::log_stream(exng::logger &logger, const std::string &level)
    : logger_(logger), level_(level)
{
}

exng::log_stream::~log_stream()
{
    logger_.logImpl(level_ + " " + ss_.str());
}
