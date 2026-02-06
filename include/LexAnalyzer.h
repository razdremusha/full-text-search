#pragma once

#include <iostream>
#include <optional>

enum class TokenType {
    And,
    Or,
    Word,
    LeftBracket,
    RightBracket
};

struct Token {
    Token(const std::string_view& token, size_t anEnd, TokenType type) : token(token), end(anEnd), type(type) {}

    std::string_view token;
    size_t end;
    TokenType type = TokenType::Word;
};

class LexAnalyzer {
public:
    explicit LexAnalyzer(std::string_view string) : parse_string_(string) {
        current_ = CreateToken(0);
    };

    std::optional<TokenType> GetType() const;

    std::optional<Token> GetAndContinue() {
        auto cur = current_;
        Next();
        return cur;
    }

private:
    std::optional<Token> CreateToken(size_t beginning);

    void Next();

    std::optional<Token> current_;
    std::string_view parse_string_;
};

