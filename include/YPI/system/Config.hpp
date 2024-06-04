#pragma once

#include "../../lib_headers/common.hpp"
#include "../../lib_headers/json.hpp"
#include "../../lib_headers/sfml.hpp"
#include "parsing.hpp"
#include "logger.hpp"
#include "Exception.hpp"
#include <variant>
#include <tuple>
#include <any>


namespace ypi
{
    class Config
    {
        public:

            Config() = default;
            ~Config() = default;

            void save(const std::string& path)
            {
                std::ofstream file(path);
                if (!file.is_open()) {
                    logger::error("Failed to create config file: " + path);
                    return;
                }

                file << m_settings.dump(4);
            }

            void load(const std::string& path)
            {
                std::ifstream file(path);
                if (!file.is_open()) {
                    m_settings = m_defaultSettings;
                    save(path);
                    return;
                }

                file >> m_settings;

                // check if all default settings are present
                for (auto& [key, value] : m_defaultSettings.items()) {
                    if (!m_settings.contains(key)) {
                        ypi::logger::warn() << "Setting not found: " << key << " (using default value)";
                        m_settings[key] = value;
                    }
                }

                // check if all settings are valid
                for (auto size = 0; size < m_settings.size(); size++) {
                    auto& [key, value] = *std::next(m_settings.items().begin(), size);
                    if (!m_defaultSettings.contains(key)) {
                        ypi::logger::warn() << "Unknown setting: " << key << " (ignoring)";
                        m_settings.erase(key);
                    }
                }
            }

            template<typename T>
            T getSetting(const std::string& key)
            {
                if (m_settings.contains(key)) {
                    return m_settings.at(key).get<T>();
                }
                throw std::runtime_error("Setting not found: " + key);
            }

            template<typename T>
            Config& setSetting(const std::string& key, T value)
            {
                m_settings[key] = value;
                return *this;
            }

            std::vector<std::string> getAvailableSettings()
            {
                std::vector<std::string> settings;
                for (auto& [key, value] : m_settings.items()) {
                    settings.push_back(key);
                }
                return settings;
            }

            template<typename T>
            Config& setDefaultSetting(const std::string& key, T value)
            {
                m_defaultSettings[key] = value;
                return *this;
            }

        private:

            nlohmann::json m_settings;
            nlohmann::json m_defaultSettings;
    };
}