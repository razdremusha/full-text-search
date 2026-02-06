#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

void ErrorMessage(const char* error_message);

namespace ArgumentParser {
    class ArgumentStringObject {
        friend class ArgParser;

    public:
        ArgumentStringObject() = default;

        ArgumentStringObject(const std::string& str) {
            long_name_ = str;
        };

        ArgumentStringObject(const char& ch, const std::string& str) {
            short_name_ = ch;
            long_name_ = str;
        }

        ArgumentStringObject(const char& ch, const std::string& str, const std::string& description) {
            short_name_ = ch;
            long_name_ = str;
            description_ = description;
        }

        ArgumentStringObject(const std::string& str, const std::string& description) {
            long_name_ = str;
            description_ = description;
        }

        ArgumentStringObject& MultiValue();

        ArgumentStringObject& MultiValue(const uint64_t& i);

        ArgumentStringObject& Default(const std::string& str);

        ArgumentStringObject& StoreValue(std::string& str);

        ArgumentStringObject& StoreValues(std::vector<std::string>& vect);

        ArgumentStringObject& Positional();

        std::string Print();

    private:
        char short_name_ = ' ';
        std::string long_name_;
        std::string description_;
        bool is_stored_ = false;
        bool is_multivalue_ = false;
        bool is_positional_ = false;
        uint64_t min_сounter_ = 1;
        bool is_filled_ = false;
        std::vector<std::string> values_;
        std::vector<std::string>* pointer_to_vector_ = nullptr;
        std::string* pointer_to_element_ = nullptr;
    };

    class ArgumentIntObject {
        friend class ArgParser;

    public:
        ArgumentIntObject() = default;

        ArgumentIntObject(const std::string& str) {
            long_name_ = str;
        }

        ArgumentIntObject(const char& ch, const std::string& str) {
            short_name_ = ch;
            long_name_ = str;
        }

        ArgumentIntObject(const char& ch, const std::string& str, const std::string& description) {
            short_name_ = ch;
            long_name_ = str;
            description_ = description;
        }

        ArgumentIntObject(const std::string& str, const std::string& description) {
            long_name_ = str;
            description_ = description;
        }

        ArgumentIntObject& MultiValue();

        ArgumentIntObject& MultiValue(const uint64_t& i);

        ArgumentIntObject& Default(const int32_t &value);

        ArgumentIntObject& StoreValue(int32_t &value);

        ArgumentIntObject& StoreValues(std::vector<int32_t>& vect);

        ArgumentIntObject& Positional();

        std::string Print();

    private:
        char short_name_ = ' ';
        std::string long_name_;
        std::string description_;
        bool is_stored_ = false;
        bool is_multivalue_ = false;
        bool is_positional_ = false;
        uint64_t min_сounter_ = 1;
        bool is_filled_ = false;
        std::vector<int> values_;
        std::vector<int>* pointer_to_vector_ = nullptr;
        int* pointer_to_element_ = nullptr;
    };

    class ArgumentBoolObject {
        friend class ArgParser;

    public:
        ArgumentBoolObject() = default;

        ArgumentBoolObject(const std::string& str) {
            long_name_ = str;
        }

        ArgumentBoolObject(const char& ch, const std::string& str) {
            short_name_ = ch;
            long_name_ = str;
        }

        ArgumentBoolObject(const char& ch, const std::string& str, const std::string& description) {
            short_name_ = ch;
            long_name_ = str;
            description_ = description;
        }

        ArgumentBoolObject(const std::string& str, const std::string& description) {
            long_name_ = str;
            description_ = description;
        }

        ArgumentBoolObject& Default(const bool& flag);

        ArgumentBoolObject& StoreValue(bool& flag);

        ArgumentBoolObject& Positional();

        std::string Print();

    private:
        char short_name_ = ' ';
        std::string long_name_;
        std::string description_;
        bool* pointer_to_element_ = nullptr;
        bool is_stored_ = false;
        bool is_positional_ = false;
        bool default_value_;
        bool value_ = false;
    };

    class ArgumentHelp {
        friend class ArgParser;

    public:
        ArgumentHelp() = default;

        ArgumentHelp(const char& ch, const std::string& str, const std::string& description) {
            short_name_ = ch;
            long_name_ = str;
            description_ = description;
        };

        std::string Print();

    private:
        char short_name_ = ' ';
        std::string long_name_;
        std::string description_;
    };


    class ArgParser {

    public:
        ArgParser(std::string str) : parser_name_(std::move(str)) {};

        ArgumentStringObject& AddStringArgument(const std::string& str);

        ArgumentStringObject& AddStringArgument(const char& ch, const std::string& str);

        ArgumentStringObject& AddStringArgument(const char& ch, const std::string& str, const std::string& description);

        ArgumentStringObject& AddStringArgument(const std::string& str, const std::string& description);

        ArgumentIntObject& AddIntArgument(const std::string& str);

        ArgumentIntObject& AddIntArgument(const char& ch, const std::string& str);

        ArgumentIntObject& AddIntArgument(const char& ch, const std::string& str, const std::string& description);

        ArgumentIntObject& AddIntArgument(const std::string& str, const std::string& description);

        ArgumentBoolObject& AddFlag(const char& ch, const std::string& str);

        ArgumentBoolObject& AddFlag(const std::string& str);

        ArgumentBoolObject& AddFlag(const char& ch, const std::string& str, const std::string& description);

        ArgumentBoolObject& AddFlag(const std::string& str, const std::string& description);

        void AddHelp(const char& ch, const std::string& str, const std::string& description);

        std::string GetStringValue(const std::string& str);

        std::string GetStringValue(const std::string& str, const uint64_t& number);

        int GetIntValue(const std::string& str);

        int GetIntValue(const std::string& str, const uint64_t& number);

        bool GetFlag(const std::string& str);

        std::string HelpDescription();

        bool Help();

        bool Parse(const std::vector<std::string>& splitted_string);

        bool Parse(const int argc, char** argv);

    private:
        void SetFlagArgument(const uint64_t& j);

        bool TryToSetStringValue(const uint64_t& j, const std::string& str);

        bool TryToSetIntValue(const uint64_t& j, const std::string& str);

        bool isAllStringArgumentsFilled();

        bool isAllIntArgumentsFilled();

        bool IfHelpArgument(const std::string& str);

        bool CanSetPositionalValue(const std::string& str);

        ArgumentHelp help_;
        bool has_help_ = false;
        std::string parser_name_;
        std::vector<ArgumentStringObject> string_arguments_;
        std::vector<ArgumentIntObject> int_arguments_;
        std::vector<ArgumentBoolObject> flag_arguments_;
    };

} // namespace ArgumentParser