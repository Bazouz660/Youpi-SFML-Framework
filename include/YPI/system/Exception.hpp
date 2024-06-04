/*
 * @ Author: Basile Trebus--Hamann
 * @ Create Time: 2023-07-31 19:02:54
 * @ Modified by: Basile Trebus--Hamann
 * @ Modified time: 2023-07-31 19:12:18
 * @ Description:
 */

#pragma once

#include <string>
#include <exception>

#define THROW(message) throw ypi::Exception(message, "file: " + std::string(__FILE__) + ", line: " + std::to_string(__LINE__))

namespace ypi {

    class Exception : public std::exception {
    public:
        Exception(const std::string& message, const std::string& where = "unknown")
            : m_message(message), m_where(where)
        {
        }

        const char* what() const noexcept override { return m_message.c_str(); }
        const char* where() const noexcept { return m_where.c_str(); }

    private:
        std::string m_message;
        std::string m_where;
    };

} // namespace ypi