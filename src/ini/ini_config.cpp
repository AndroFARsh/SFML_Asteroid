//
// Created by Anton Kukhlevskyi on 2024-02-05.
//

#include "ini_config.h"
#include "../data/color.h"


// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


/**
 * Parse the INI file located to the given file path and store the values in Ini::values
 *
 * @param filePath - The path of the INI file to parse
 */
void IniConfig::parse(const std::string &filePath)
{
    std::ifstream fileReader(filePath, std::ifstream::in);
    if (fileReader.fail()) {
        return;
    }

    std::string   fileContent((std::istreambuf_iterator<char>(fileReader)), std::istreambuf_iterator<char>());

    std::regex    sections(R"((\s*\[(\w+)\])((?:\r?\n(?:[^[\r\n].*)?)*))");
    std::regex    parameters(R"((\w+) ?= ?(\"([^\"]+)\"|([^\r\n\t\f\v;#]+)))");

    std::smatch   sectionMatch;

    _iniPath = filePath;
    while (regex_search(fileContent, sectionMatch, sections)) {
        std::unordered_map<std::string, std::string> sectionParameters;
        std::string sectionString = sectionMatch[3].str();

        for (std::sregex_iterator parameter(sectionString.begin(), sectionString.end(), parameters); parameter != std::sregex_iterator(); ++parameter) {
            const std::smatch& parameterMatch = *parameter;
            auto parameterKey          = parameterMatch[1].str();
            auto parameterValue        = parameterMatch[3].matched ? parameterMatch[3].str() : parameterMatch[4].str();

            trim(parameterKey);
            trim(parameterValue);

            sectionParameters[parameterKey]   = parameterValue;
            // parameterMatch[1] is the key, parameterMatch[3] is a trim quoted string (string without double quotes)
            // and parameterMatch[4] is a number
        }

        auto sectionKey = sectionMatch[2].str();
        trim(sectionKey);

        _values[sectionKey] = sectionParameters;
        fileContent = sectionMatch.suffix();
    }
}

/**
 * Tells if the parameter exists in the given section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return True if the parameter exists in the given section at the given key, false otherwise
 */
bool IniConfig::hasSection(const std::string &section) const
{
    return _values.find(section) != _values.end();
}

/**
 * Tells if the parameter exists in the given section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return True if the parameter exists in the given section at the given key, false otherwise
 */
bool IniConfig::hasKey(const std::string &section, const std::string &key) const
{
    auto section_it = _values.find(section);
    if (section_it != _values.end()) {
        const auto& properties = section_it->second;
        const auto& prop_it      = properties.find(key);
        if (prop_it != properties.end()) {
            return true;
        }
    }
    return false;
}

/**
 * Throw an exception if the requested type is not defined
 */
template<typename T>
T IniConfig::get(const std::string &section, const std::string &key, T fallback) const
{
    throw std::runtime_error("The type of the given parameter is not defined: " + std::string(typeid(T).name()));
}


/**
 * Get the string parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested string value
 */
template<>
std::string IniConfig::get(const std::string &section, const std::string &key, std::string fallback) const
{
    auto section_it = _values.find(section);
    if (section_it != _values.end()) {
        const auto& properties = section_it->second;
        const auto& prop_it = properties.find(key);
        if (prop_it != properties.end()) {
            return prop_it->second;
        }
    }
    return fallback;
}

/**
 * Get the string parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested string value
 */
template<>
const char* IniConfig::get(const std::string &section, const std::string &key, const char* fallback) const
{
    auto section_it = _values.find(section);
    if (section_it != _values.end()) {
        const auto& properties = section_it->second;
        const auto& prop_it = properties.find(key);
        if (prop_it != properties.end()) {
            return prop_it->second.c_str();
        }
    }
    return fallback;
}


/**
 * Extract the integer number of the parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested integer number value
 */
long long IniConfig::extractIntegerNumber(const std::string &section, const std::string &key) const
{
    const auto & value = get<std::string>(section, key, "");
    if (value.empty()) {
        throw std::runtime_error("The key " + key + " does not exist in section " + section + " in " + _iniPath);
    }

    std::smatch intNumberMatch;
    std::regex  intNumber(R"(^\s*(\-?\d+)\s*$)");

    if (std::regex_match(value, intNumberMatch, intNumber)) {
        return std::strtoll(intNumberMatch[1].str().c_str(), nullptr, 10);
    } else {
        throw std::runtime_error("The given parameter is not an integer number");
    }
}

/**
 * Extract the floating point number of the parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested floating point number value
 */
long double IniConfig::extractFloatNumber(const std::string &section, const std::string &key) const
{
    const auto & value = get<std::string>(section, key, "");
    if (value.empty()) {
        throw std::runtime_error("The key " + key + " does not exist in section " + section + " in " + _iniPath);
    }

    std::smatch floatNumberMatch;
    std::regex  floatNumber(R"(^\s*(\-?(?:(?:\d+(?:\.\d*)?)|(?:\d+(?:\.\d+)?e(?:\+|\-)\d+))f?)\s*$)");

    if (std::regex_match(value, floatNumberMatch, floatNumber)) {
        return std::strtold(floatNumberMatch[1].str().c_str(), nullptr);
    } else {
        throw std::runtime_error("The given parameter is not a floating point number");
    }
}


/**
 * Get the boolean parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested bool value
 */
template<>
Color IniConfig::get(const std::string &section, const std::string &key, Color fallback) const
{
    const auto & value = get<std::string>(section, key, "");
    if (value.empty()) {
        return fallback;
    }

    std::smatch match;
    std::regex pattern("0x([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{0,2})");

    if (std::regex_match(value, match, pattern)) {
        uint8_t r = std::stoi(match[1].str(), nullptr, 16);
        uint8_t g = std::stoi(match[2].str(), nullptr, 16);
        uint8_t b = std::stoi(match[3].str(), nullptr, 16);
        uint8_t a = match[4].length() > 0//.str().empty()
                ? std::stoi(match[4].str(), nullptr, 16)
                : 255;


        return { r, g, b, a };
    } else {
        throw std::runtime_error("The given parameter is not a floating point number");
    }
}

/**
 * Get the boolean parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested bool value
 */
template<>
bool IniConfig::get(const std::string &section, const std::string &key, bool fallback) const
{
    auto value = get<std::string>(section, key, "");

    if (value.empty()) {
        return fallback;
    }
    return value == "true" || value == "1";
}

/**
 * Get the short number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested short number value
 */
template<>
short IniConfig::get(const std::string &section, const std::string &key, short fallback) const
{
    if (!hasKey(section, key)) return fallback;

    auto number = extractIntegerNumber(section, key);
    if (number >= -SHRT_MAX && number <= SHRT_MAX) {
        return static_cast<short>(number);
    } else {
        throw std::runtime_error("The number is out of range of a short integer");
    }
}

/**
 * Get the int number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested int number value
 */
template<>
int IniConfig::get(const std::string &section, const std::string &key, int fallback) const
{
    if (!hasKey(section, key)) return fallback;

    auto number = extractIntegerNumber(section, key);

    if (number >= -INT_MAX && number <= INT_MAX) {
        return static_cast<int>(number);
    } else {
        throw std::runtime_error("The number is out of range of a an integer");
    }
}

/**
 * Get the unsigned int number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested unsigned int number value
 */
template<>
unsigned int IniConfig::get(const std::string &section, const std::string &key, unsigned int fallback) const
{
    if (!hasKey(section, key)) return fallback;

    auto number = extractIntegerNumber(section, key);

    if (number < 0) {
        throw std::runtime_error("The number is negative so it cannot be unsigned");
    } else if (number <= UINT_MAX) {
        return static_cast<unsigned int>(number);
    } else {
        throw std::runtime_error("The number is out of range of a an unsigned integer");
    }
}

/**
 * Get the long int number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested long int number value
 */
template<>
long IniConfig::get(const std::string &section, const std::string &key, long fallback) const
{
    if (!hasKey(section, key)) return fallback;

    auto number = extractIntegerNumber(section, key);

    if (number >= -LONG_MAX && number <= LONG_MAX) {
        return number;
    } else {
        throw std::runtime_error("The number is out of range of a a long integer");
    }
}

/**
 * Get the long long int number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested long long int number value
 */
template<>
long long IniConfig::get(const std::string &section, const std::string &key, long long fallback) const
{
    if (!hasKey(section, key)) return fallback;

    return extractIntegerNumber(section, key);
}

/**
 * Get the float number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested float number value
 */
template<>
float IniConfig::get(const std::string &section, const std::string &key, float fallback) const
{
    if (!hasKey(section, key)) return fallback;

    auto number = extractFloatNumber(section, key);

    if (number >= -FLT_MAX && number <= FLT_MAX) {
        return static_cast<float>(number);
    } else {
        throw std::runtime_error("The number is out of range of a float");
    }
}

/**
 * Get the double number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested double number value
 */
template<>
double IniConfig::get(const std::string &section, const std::string &key, double fallback) const
{
    if (!hasKey(section, key)) return fallback;

    auto number = extractFloatNumber(section, key);

    if (number >= -DBL_MAX && number <= DBL_MAX) {
        return static_cast<double>(number);
    } else {
        throw std::runtime_error("The number is out of range of a double");
    }
}

/**
 * Get the long double number parameter in the given INI section at the given key
 *
 * @param section - The INI section to get the parameter from
 * @param key - The INI parameter key
 *
 * @return The requested long double number value
 */
template<>
long double IniConfig::get(const std::string &section, const std::string &key, long double fallback) const
{
    if (!hasKey(section, key)) return fallback;

    return extractFloatNumber(section, key);
}