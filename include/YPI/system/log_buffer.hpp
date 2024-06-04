#ifndef YPI_LOG_BUFF_HPP
#define YPI_LOG_BUFF_HPP

#include "../../lib_headers/sfml.hpp"
#include <streambuf>

namespace ypi {

    class LoggerStreambuf : public std::streambuf {
    public:
        LoggerStreambuf() = default;

    protected:
        virtual int_type overflow(int_type ch);

    private:
        std::string lineBuffer;
    };

}

#endif //YPI_LOG_BUFF_HPP