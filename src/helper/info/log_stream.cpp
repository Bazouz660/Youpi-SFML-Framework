/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** log_stream
*/

#include "log_stream.hpp"
#include "logger.hpp"

ypi::log_stream::log_stream(ypi::logger &logger, const std::string &level)
    : logger_(logger), level_(level)
{
}

ypi::log_stream::~log_stream()
{
    logger_.logImpl(level_ + " " + ss_.str());
}
