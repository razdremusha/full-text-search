#include "ArgParser.h"
#include <cstring>
#include <iostream>
#include <string>


using namespace ArgumentParser;

void ErrorMessage(const char* error_message) {
    std::cerr << error_message;
    exit(EXIT_FAILURE);
}

//StringObject
ArgumentStringObject& ArgParser::AddStringArgument(const std::string& str) {
    ArgumentStringObject new_arg = ArgumentStringObject(str);
    string_arguments_.push_back(new_arg);
    return string_arguments_.back();
}

ArgumentStringObject& ArgParser::AddStringArgument(const char& ch, const std::string& str) {
    ArgumentStringObject new_arg = ArgumentStringObject(ch, str);
    string_arguments_.push_back(new_arg);
    return string_arguments_.back();
}

ArgumentStringObject&
ArgParser::AddStringArgument(const char& ch, const std::string& str, const std::string& description) {
    ArgumentStringObject new_arg = ArgumentStringObject(ch, str, description);
    string_arguments_.push_back(new_arg);
    return string_arguments_.back();
}

ArgumentStringObject& ArgParser::AddStringArgument(const std::string& str, const std::string& description) {
    ArgumentStringObject new_arg = ArgumentStringObject(str, description);
    string_arguments_.push_back(new_arg);
    return string_arguments_.back();
}

std::string ArgumentStringObject::Print() {
    std::string answer;
    if (short_name_ != ' ')
        answer = answer + "-" + short_name_ + " ";
    if (!long_name_.empty())
        answer = answer + "--" + long_name_ + " ";
    if (!description_.empty())
        answer = answer + description_ + " ";
    if (is_multivalue_)
        answer += "[multivalued] ";
    if (is_positional_)
        answer += "[positional] ";
    answer += "\n";
    return answer;
}

ArgumentStringObject& ArgumentStringObject::MultiValue() {
    is_multivalue_ = true;
    return *this;
}

ArgumentStringObject& ArgumentStringObject::MultiValue(const uint64_t& i) {
    is_multivalue_ = true;
    min_сounter_ = i;
    return *this;
}

ArgumentStringObject& ArgumentStringObject::Default(const std::string& str) {
    if (is_multivalue_) {
        for (uint64_t i = 0; i < min_сounter_; i++) {
            values_.push_back(str);
        }
    } else {
        values_.push_back(str);
    }
    is_filled_ = true;
    return *this;
};

ArgumentStringObject& ArgumentStringObject::StoreValue(std::string& str) {
    is_stored_ = true;
    pointer_to_element_ = &str;
    if (!values_.empty())
        *pointer_to_element_ = values_[0];
    return *this;
}

ArgumentStringObject& ArgumentStringObject::StoreValues(std::vector<std::string>& vect) {
    is_stored_ = true;
    pointer_to_vector_ = &vect;
    if (!values_.empty())
        *pointer_to_vector_ = values_;
    return *this;
};

ArgumentStringObject& ArgumentStringObject::Positional() {
    is_positional_ = true;
    return *this;
}

std::string ArgParser::GetStringValue(const std::string& str) {
    uint64_t index = 0;
    for (; index < string_arguments_.size(); ++index) {
        if (str == string_arguments_[index].long_name_)
            break;
    }
    if (str != string_arguments_[index].long_name_) {
        ErrorMessage("Argument with such name doesn't exist");
    }
    if (string_arguments_[index].is_multivalue_)
        ErrorMessage("It's multivalued parameter, specify relevant_number of element");
    if (string_arguments_[index].is_stored_)
        return (*string_arguments_[index].pointer_to_element_);
    return string_arguments_[index].values_[0];

}

std::string ArgParser::GetStringValue(const std::string& str, const uint64_t& number) {
    uint64_t index = 0;
    for (; index < string_arguments_.size(); ++index) {
        if (str == string_arguments_[index].long_name_)
            break;
    }

    if (str != string_arguments_[index].long_name_) {
        ErrorMessage("Argument with such name doesn't exist");
    }
    if (!string_arguments_[index].is_multivalue_)
        ErrorMessage("Argument is not multi-value_");

    if (string_arguments_[index].is_stored_) {
        if (number < (*string_arguments_[index].pointer_to_vector_).size())
            return (*string_arguments_[index].pointer_to_vector_)[number];
        ErrorMessage("Argument have less elements");
    }
    if (number < string_arguments_[index].values_.size())
        return string_arguments_[index].values_[number];
    ErrorMessage("Argument have less elements");
    return "";
}

//IntObject

ArgumentIntObject& ArgParser::AddIntArgument(const std::string& str) {
    ArgumentIntObject new_arg = ArgumentIntObject(str);
    int_arguments_.push_back(new_arg);
    return int_arguments_.back();
}

ArgumentIntObject& ArgParser::AddIntArgument(const char& ch, const std::string& str) {
    ArgumentIntObject new_arg = ArgumentIntObject(ch, str);
    int_arguments_.push_back(new_arg);
    return int_arguments_.back();

}

ArgumentIntObject& ArgParser::AddIntArgument(const char& ch, const std::string& str, const std::string& description) {
    ArgumentIntObject new_arg = ArgumentIntObject(ch, str, description);
    int_arguments_.push_back(new_arg);
    return int_arguments_.back();
}

ArgumentIntObject& ArgParser::AddIntArgument(const std::string& str, const std::string& description) {
    ArgumentIntObject new_arg = ArgumentIntObject(str, description);
    int_arguments_.push_back(new_arg);
    return int_arguments_.back();
}

std::string ArgumentIntObject::Print() {
    std::string answer;
    if (short_name_ != ' ')
        answer = answer + "-" + short_name_ + " ";
    if (!long_name_.empty())
        answer = answer + "--" + long_name_ + " ";
    if (!description_.empty())
        answer = answer + description_ + " ";
    if (is_multivalue_)
        answer += "[multivalued] ";
    if (is_positional_)
        answer += "[positional] ";
    answer += "\n";
    return answer;
}

ArgumentIntObject& ArgumentIntObject::MultiValue() {
    is_multivalue_ = true;
    return *this;
}

ArgumentIntObject& ArgumentIntObject::MultiValue(const uint64_t& i) {
    is_multivalue_ = true;
    min_сounter_ = i;
    return *this;
}

ArgumentIntObject& ArgumentIntObject::Default(const int& value) {
    if (is_multivalue_) {
        for (uint64_t i = 0; i < min_сounter_; i++) {
            values_.push_back(value);
        }
    } else {
        values_.push_back(value);
    }
    is_filled_ = true;
    return *this;
};

ArgumentIntObject& ArgumentIntObject::StoreValue(int& value) {
    is_stored_ = true;
    pointer_to_element_ = &value;
    if (!values_.empty())
        *pointer_to_element_ = values_[0];
    return *this;
}

ArgumentIntObject& ArgumentIntObject::StoreValues(std::vector<int>& vect) {
    is_stored_ = true;
    pointer_to_vector_ = &vect;
    if (!values_.empty())
        *pointer_to_vector_ = values_;
    return *this;
};

ArgumentIntObject& ArgumentIntObject::Positional() {
    is_positional_ = true;
    return *this;
}

int ArgParser::GetIntValue(const std::string& str) {
    uint64_t index = 0;
    for (; index < int_arguments_.size(); ++index) {
        if (str == int_arguments_[index].long_name_)
            break;
    }
    if (str != int_arguments_[index].long_name_) {
        ErrorMessage("Argument with such name doesn't exist");
    }
    if (int_arguments_[index].is_multivalue_)
        ErrorMessage("It's multivalued parameter, specify relevant_number of element");
    if (int_arguments_[index].is_stored_)
        return (*int_arguments_[index].pointer_to_element_);
    return int_arguments_[index].values_.back();
}

int ArgParser::GetIntValue(const std::string& str, const uint64_t& number) {
    uint64_t index = 0;
    for (; index < int_arguments_.size(); ++index) {
        if (str == int_arguments_[index].long_name_)
            break;
    }

    if (str != int_arguments_[index].long_name_) {
        ErrorMessage("Argument with such name doesn't exist");
    }
    if (!int_arguments_[index].is_multivalue_)
        ErrorMessage("Argument is not multivalued");

    if (int_arguments_[index].is_stored_) {
        if (number < (*int_arguments_[index].pointer_to_vector_).size())
            return (*int_arguments_[index].pointer_to_vector_)[number];
        ErrorMessage("Argument have less elements");
    }
    if (number < int_arguments_[index].values_.size())
        return int_arguments_[index].values_[number];
    ErrorMessage("Argument have less elements");
    return 0;
}

//BoolObject
ArgumentBoolObject& ArgParser::AddFlag(const char& ch, const std::string& str) {
    ArgumentBoolObject new_arg = ArgumentBoolObject(ch, str);
    flag_arguments_.push_back(new_arg);
    return flag_arguments_.back();
}

ArgumentBoolObject& ArgParser::AddFlag(const std::string& str) {
    ArgumentBoolObject new_arg = ArgumentBoolObject(str);
    flag_arguments_.push_back(new_arg);
    return flag_arguments_.back();
}

ArgumentBoolObject& ArgParser::AddFlag(const char& ch, const std::string& str, const std::string& description) {
    ArgumentBoolObject new_arg = ArgumentBoolObject(ch, str, description);
    flag_arguments_.push_back(new_arg);
    return flag_arguments_.back();
}

ArgumentBoolObject& ArgParser::AddFlag(const std::string& str, const std::string& description) {
    ArgumentBoolObject new_arg = ArgumentBoolObject(str, description);
    flag_arguments_.push_back(new_arg);
    return flag_arguments_.back();
}

bool ArgParser::GetFlag(const std::string& str) {
    uint64_t index = 0;
    for (; index < flag_arguments_.size(); ++index) {
        if (str == flag_arguments_[index].long_name_)
            break;
    }
    if (str != flag_arguments_[index].long_name_) {
        ErrorMessage("Flag with such name doesn't exist");
    }
    if (flag_arguments_[index].is_stored_)
        return *flag_arguments_[index].pointer_to_element_;
    return flag_arguments_[index].value_;
}

std::string ArgumentBoolObject::Print() {
    std::string answer;
    if (short_name_ != ' ')
        answer = answer + "-" + short_name_ + " ";
    if (!long_name_.empty())
        answer = answer + "--" + long_name_ + " ";
    if (!description_.empty())
        answer = answer + description_ + " ";
    if (is_positional_)
        answer += "[positional] ";
    answer += "\n";
    return answer;
}

ArgumentBoolObject& ArgumentBoolObject::Default(const bool& flag) {
    value_ = flag;
    return *this;
};

ArgumentBoolObject& ArgumentBoolObject::StoreValue(bool& flag) {
    is_stored_ = true;
    pointer_to_element_ = &flag;
    *pointer_to_element_ = value_;
    return *this;
};

ArgumentBoolObject& ArgumentBoolObject::Positional() {
    is_positional_ = true;
    return *this;
}

//help
void ArgParser::AddHelp(const char& ch, const std::string& str, const std::string& description) {
    ArgumentHelp new_arg = ArgumentHelp(ch, str, description);
    help_ = new_arg;
}

bool ArgParser::Help() {
    return has_help_;
}

std::string ArgumentHelp::Print() {
    std::string answer;
    if (short_name_ != ' ')
        answer = answer + "-" + short_name_ + " ";
    if (!long_name_.empty())
        answer = answer + "--" + long_name_ + " ";
    if (!description_.empty())
        answer = answer + description_ + " ";
    answer += "\n";
    return answer;
}

std::string ArgParser::HelpDescription() {
    std::string answer;
    if (has_help_) {
        answer = answer + parser_name_ + "\n";
        answer += "=======================\n";
        answer += "Description for program \n";
        answer += "=======================\n";
        for (int i = 0; i < string_arguments_.size(); ++i) {
            answer += string_arguments_[i].Print();
        }
        for (int i = 0; i < int_arguments_.size(); ++i) {
            answer += int_arguments_[i].Print();
        }
        for (int i = 0; i < flag_arguments_.size(); ++i) {
            answer += flag_arguments_[i].Print();
        }
        answer += help_.Print();
    }
    return answer;
}

void ArgParser::SetFlagArgument(const uint64_t& j) {
    if (flag_arguments_[j].is_stored_) {
        (*flag_arguments_[j].pointer_to_element_) = true;
    } else {
        flag_arguments_[j].value_ = true;
    }
}

bool ArgParser::TryToSetStringValue(const uint64_t& j, const std::string& str) {
    uint64_t index = str.find('=');
    if (string_arguments_[j].is_positional_ and index == std::string::npos)
        index = -1;
    if (index + 1 >= str.size())
        return false;
    if (string_arguments_[j].is_stored_) {
        if (string_arguments_[j].is_multivalue_) {
            (*string_arguments_[j].pointer_to_vector_).push_back(str.substr(index + 1));
        } else {
            (*string_arguments_[j].pointer_to_element_) = str.substr(index + 1);
        }
    } else {
        string_arguments_[j].values_.push_back(str.substr(index + 1));
    }
    string_arguments_[j].is_filled_ = true;
    return true;
}

bool ArgParser::TryToSetIntValue(const uint64_t& j, const std::string& str) {
    uint64_t index = str.find('=');
    if (int_arguments_[j].is_positional_ and index == std::string::npos)
        index = -1;
    if (index + 1 >= str.size())
        return false;
    std::string digit_string = str.substr(index + 1);
    for (uint64_t i = 0; i < digit_string.size(); ++i) {
        if (!isdigit(digit_string[i]))
            return false;
    }
    if (int_arguments_[j].is_stored_) {
        if (int_arguments_[j].is_multivalue_) {
            (*int_arguments_[j].pointer_to_vector_).push_back(stoi(digit_string));
        } else {
            (*int_arguments_[j].pointer_to_element_) = stoi(digit_string);
        }
    } else {
        int_arguments_[j].values_.push_back(stoi(digit_string));
    }
    int_arguments_[j].is_filled_ = true;
    return true;
}

bool ArgParser::isAllStringArgumentsFilled() {
    for (uint64_t i = 0; i < string_arguments_.size(); ++i) {
        if (!string_arguments_[i].is_filled_)
            return false;
        if (string_arguments_[i].is_multivalue_) {
            if (string_arguments_[i].is_stored_) {
                if ((*string_arguments_[i].pointer_to_vector_).size() < string_arguments_[i].min_сounter_)
                    return false;
            } else {
                if (string_arguments_[i].values_.size() < string_arguments_[i].min_сounter_)
                    return false;
            }
        }
    }
    return true;
}

bool ArgParser::isAllIntArgumentsFilled() {
    for (uint64_t i = 0; i < int_arguments_.size(); ++i) {
        if (!int_arguments_[i].is_filled_)
            return false;
        if (int_arguments_[i].is_multivalue_) {
            if (int_arguments_[i].is_stored_) {
                if ((*int_arguments_[i].pointer_to_vector_).size() < int_arguments_[i].min_сounter_)
                    return false;
            } else {
                if (int_arguments_[i].values_.size() < int_arguments_[i].min_сounter_)
                    return false;
            }
        }
    }
    return true;
}

bool ArgParser::IfHelpArgument(const std::string& str) {
    if (str[0] != '-')
        return false;
    if (str[1] == '-' && str.substr(strlen("--")) == help_.long_name_) {
        has_help_ = true;
        return true;
    }
    if (str[0] == '-' and str[1] == help_.short_name_) {
        has_help_ = true;
        return true;
    }
    return false;
};

bool ArgParser::CanSetPositionalValue(const std::string& str) {
    for (uint64_t j = 0; j < string_arguments_.size(); ++j) {
        if (string_arguments_[j].is_positional_) {
            if (!TryToSetStringValue(j, str))
                return false;
            return true;
        }
    }
    for (uint64_t j = 0; j < int_arguments_.size(); ++j) {
        if (int_arguments_[j].is_positional_) {
            if (!TryToSetIntValue(j, str))
                return false;
            return true;
        }
    }
    return false;
}

bool ArgParser::Parse(const std::vector<std::string>& splitted_string) {
    for (uint64_t i = 0; i < splitted_string.size(); ++i) {
        bool is_parsed = false;
        if (IfHelpArgument(splitted_string[i]))
            return true;
        if (splitted_string[i].substr(0, strlen("--")) == "--") {
            for (uint64_t j = 0; !is_parsed && j < flag_arguments_.size(); ++j) {
                if ((splitted_string[i].substr(strlen("--")) ==
                     flag_arguments_[j].long_name_)) {
                    SetFlagArgument(j);
                    is_parsed = true;
                }
            }
            uint64_t index = splitted_string[i].find('=');
            if (!is_parsed && index == std::string::npos)
                return false;
            for (uint64_t j = 0; !is_parsed && j < string_arguments_.size(); ++j) {
                if ((splitted_string[i].substr(strlen("--"), index - strlen("--")) ==
                     string_arguments_[j].long_name_)) {
                    if (!TryToSetStringValue(j, splitted_string[i]))
                        return false;
                    is_parsed = true;
                }
            }
            for (uint64_t j = 0; !is_parsed && j < int_arguments_.size(); ++j) {
                if ((splitted_string[i].substr(strlen("--"), index - strlen("--")) ==
                     int_arguments_[j].long_name_)) {
                    if (!TryToSetIntValue(j, splitted_string[i]))
                        return false;
                    is_parsed = true;
                }
            }
        } else {
            if (splitted_string[i][0] == '-') {
                for (uint64_t j = 0; !is_parsed && j < flag_arguments_.size(); ++j) {
                    if (splitted_string[i][1] ==
                        flag_arguments_[j].short_name_) {
                        SetFlagArgument(j);
                        uint64_t numbers_chars = 2;
                        while (!splitted_string[i].substr(numbers_chars).empty()) {
                            for (uint64_t l = 0; l < flag_arguments_.size(); ++l) {
                                if (splitted_string[i][numbers_chars] ==
                                    flag_arguments_[l].short_name_) {
                                    SetFlagArgument(l);
                                    numbers_chars++;
                                    break;
                                }
                            }
                        }
                        is_parsed = true;
                    }
                }
                uint64_t index = splitted_string[i].find('=');
                if (!is_parsed && index != 2)
                    return false;
                for (uint64_t j = 0; !is_parsed && j < string_arguments_.size(); ++j) {
                    if (splitted_string[i][1] == string_arguments_[j].short_name_) {
                        if (!TryToSetStringValue(j, splitted_string[i]))
                            return false;
                        is_parsed = true;
                    }
                }
                for (uint64_t j = 0; !is_parsed && j < int_arguments_.size(); ++j) {
                    if (splitted_string[i][1] ==
                        int_arguments_[j].short_name_) {
                        if (!TryToSetIntValue(j, splitted_string[i]))
                            return false;
                        is_parsed = true;
                    }
                }
            } else {
                is_parsed = CanSetPositionalValue(splitted_string[i]);
            }
        }
        if (!is_parsed)
            return false;
    }
    if (!isAllStringArgumentsFilled())
        return false;
    if (!isAllIntArgumentsFilled())
        return false;
    return true;
}

bool ArgParser::Parse(const int32_t argc, char** argv) {
    std::vector<std::string> strings;
    for (int32_t i = 0; i < argc; ++i) {
        strings.push_back(static_cast<std::string>(argv[i]));
    }
    return Parse(strings);
}