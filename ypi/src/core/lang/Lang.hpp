#pragma once

#include "ypi/lib_headers/common.hpp"
#include "parsing.hpp"
#include "Exception.hpp"
#include "ypi/lib_headers/sfml.hpp"
#include <locale>
#include <codecvt>

namespace exng
{

    class Lang
    {
        public:
            enum class Languages
            {
                English,
                French,
            };

            static void loadLanguage(Languages lang, const std::string &filename)
            {
                auto& inst = getInstance();

                std::ifstream file(filename);
                if (!file.is_open())
                    THROW("Could not open language file: " + filename);

                nlohmann::json json;
                file >> json;

                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

                for (auto& [key, value] : json.items()) {
                    std::string strValue = value.get<std::string>();
                    inst.mLangMap[lang][key] = converter.from_bytes(strValue);
                }
            }

            static void setCurrentLanguage(Languages lang)
            {
                getInstance().mCurrentLanguage = lang;
            }

            static void setCurrentLanguage(const std::string& code)
            {
                auto& inst = getInstance();

                for (auto& [lang, info] : inst.mLangInfo) {
                    if (info.code == code) {
                        inst.mCurrentLanguage = lang;
                        return;
                    }
                }
            }

            static std::wstring getString(const std::string &key)
            {
                auto& inst = getInstance();
                return inst.mLangMap[inst.mCurrentLanguage][key];
            }

            struct langInfo
            {
                std::string str;
                std::string code;
            };

            static langInfo getLangInfo(Languages lang)
            {
                auto& inst = getInstance();
                return inst.mLangInfo[lang];
            }

        private:
            Lang() = default;

            static Lang &getInstance()
            {
                static Lang instance;
                return instance;
            }

            std::unordered_map<Languages, langInfo> mLangInfo {
                { Languages::English, { "English", "en" } },
                { Languages::French, { "Français", "fr" } },
            };

            Languages mCurrentLanguage = Languages::English;
            std::map<Languages, std::map<std::string, std::wstring>> mLangMap;
    };

}