//
// Created by Anton Kukhlevskyi on 2024-02-05.
//

#ifndef INTROECS_INI_CONFIG_H
#define INTROECS_INI_CONFIG_H

#include <fstream>
#include <unordered_map>
#include <string>
#include <regex>
#include <cfloat>
#include <climits>

#ifndef PARAMETERS_INI_FILE_PATH

#define PARAMETERS_INI_FILE_PATH "./game_config.ini"

#endif //PARAMETERS_INI_FILE_PATH

struct IniConfig
{
private:
    typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> IniStructure;

    // [section][parameter name][parameter value]
    IniStructure _values;
    std::string  _iniPath;

public:
    IniConfig() = default;
    explicit IniConfig(const std::string &filePath) {
        parse(filePath);
    };

    void parse(const std::string &filePath = PARAMETERS_INI_FILE_PATH);

    template<typename T>
    [[nodiscard]] T get(const std::string &section, const std::string &key, T fallback) const;
    [[nodiscard]] bool hasSection(const std::string &section) const;
    [[nodiscard]] bool hasKey(const std::string &section, const std::string &key) const;

private:
    [[nodiscard]] long long extractIntegerNumber(const std::string &section, const std::string &key) const;
    [[nodiscard]] long double extractFloatNumber(const std::string &section, const std::string &key) const;
};

#endif //INTROECS_INI_CONFIG_H
