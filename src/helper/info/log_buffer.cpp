#include "log_buffer.hpp"
#include "logger.hpp"
#include <streambuf>

namespace ypi {

    std::streambuf::int_type LoggerStreambuf::overflow(std::streambuf::int_type ch)
    {
        if (ch != EOF) {
            // Ajoute le caractère au tampon de ligne
            lineBuffer += ch;

            // Si le caractère est une nouvelle ligne ou une fin de fichier, envoie la ligne tamponnée à votre logger
            if (ch == '\n') {
                lineBuffer.pop_back();
                ypi::logger::error(lineBuffer);
                lineBuffer.clear();
            }
        } else {
            ypi::logger::error(lineBuffer);
            lineBuffer.clear();
        }
        return ch;
    }

};