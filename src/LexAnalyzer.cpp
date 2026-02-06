#include "LexAnalyzer.h"

std::optional<TokenType> LexAnalyzer::GetType() const {
    if (current_.has_value())
        return current_->type;
    return std::nullopt;
}

void LexAnalyzer::Next() {
    if (current_) {
        auto beginning = current_->end;
        current_ = CreateToken(beginning);
        return;
    }
}

std::optional<Token> LexAnalyzer::CreateToken(size_t beginning) {
    if (beginning >= parse_string_.size())
        return {};
    auto FindWord = [&](size_t beginning) {
        size_t counter = beginning + 1;
        while (counter < parse_string_.size() && parse_string_[counter] != ')' && parse_string_[counter] != '(' &&
               parse_string_[counter] != ' ') {
            ++counter;
        }
        return parse_string_.substr(beginning, counter - beginning);
    };
    while (parse_string_.size() > beginning && parse_string_[beginning] == ' ')
        ++beginning;
    switch (parse_string_[beginning]) {
        case '(': {
            return Token(parse_string_.substr(beginning, 1), beginning + 1, TokenType::LeftBracket);
        }
        case ')': {
            return Token(parse_string_.substr(beginning, 1), beginning + 1, TokenType::RightBracket);
        }
        case 'A': {
            auto temp = FindWord(beginning);
            if (temp == "AND")
                return Token(temp, beginning + temp.size(), TokenType::And);
            return Token(temp, beginning + temp.size(), TokenType::Word);
        }
        case 'O': {
            auto temp = FindWord(beginning);
            if (temp == "OR")
                return Token(temp, beginning + temp.size(), TokenType::Or);
            return Token(temp, beginning + temp.size(), TokenType::Word);
        }
        default: {
            auto temp = FindWord(beginning);
            return Token(temp, beginning + temp.size(), TokenType::Word);
        }
    }
}
