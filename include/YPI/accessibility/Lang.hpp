#pragma once

#include "../../lib_headers/common.hpp"
#include "../../lib_headers/json.hpp"
#include "../system/parsing.hpp"
#include "../system/Exception.hpp"
#include "../../lib_headers/sfml.hpp"
#include "../system/logger.hpp"
#include <locale>
#include <codecvt>

namespace ypi
{

    class Lang
    {
    public:
        struct LangInfo
        {
            std::string name;
            std::string code;
        };

        static void loadLanguage(const LangInfo& lang, const std::string &filepath)
        {
            auto& inst = getInstance();

            std::ifstream file(filepath);
            if (!file.is_open())
                THROW("Could not open language file: " + filepath);

            nlohmann::json json;
            file >> json;

            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

            for (auto& [key, value] : json.items()) {
                std::string strValue = value.get<std::string>();
                inst.mLangMap[lang.code][key] = converter.from_bytes(strValue);
            }

            inst.mLangInfo[lang.code] = lang;
        }

        static void setCurrentLanguage(const std::string& code)
        {
            auto& inst = getInstance();

            if (inst.mLangInfo.find(code) == inst.mLangInfo.end())
                THROW("Language not loaded: " + code);

            inst.mCurrentLanguage = code;
        }

        static std::wstring getWString(const std::string &key)
        {
            auto& inst = getInstance();
            if (inst.mLangMap[inst.mCurrentLanguage].find(key) == inst.mLangMap[inst.mCurrentLanguage].end()) {
                logger::error() << "Lang key not found: " << key;
                return L"";
            }
            return inst.mLangMap[inst.mCurrentLanguage][key];
        }

        static std::string getString(const std::string &key)
        {
            auto& inst = getInstance();
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            if (inst.mLangMap[inst.mCurrentLanguage].find(key) == inst.mLangMap[inst.mCurrentLanguage].end()) {
                logger::error() << "Lang key not found: " << key;
                return "";
            }
            return converter.to_bytes(inst.mLangMap[inst.mCurrentLanguage][key]);
        }

        static LangInfo getLangInfo(const std::string& code)
        {
            auto& inst = getInstance();
            return inst.mLangInfo[code];
        }

        static std::vector<LangInfo> getLangs()
        {
            auto& inst = getInstance();
            std::vector<LangInfo> langs;
            for (auto& [code, info] : inst.mLangInfo)
                langs.push_back(info);
            return langs;
        }

    private:
        Lang() = default;

        static Lang &getInstance()
        {
            static Lang instance;
            return instance;
        }

        std::string mCurrentLanguage = "en";
        std::map<std::string, std::map<std::string, std::wstring>> mLangMap;
        std::map<std::string, LangInfo> mLangInfo;
    };

}