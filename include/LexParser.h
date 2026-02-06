#pragma once

#include "LexAnalyzer.h"

#include<iostream>
#include<optional>
#include<expected>
#include <memory>
#include<set>

enum class ParseError {
    WordError,
    AndError,
    OrError
};

struct OrNode;

struct AndNode;

struct WordNode;

using OrExpression = std::unique_ptr<OrNode>;
using AndExpression = std::unique_ptr<AndNode>;
using WordExpression = std::unique_ptr<WordNode>;

struct WordNode {
    WordNode() = default;

    WordNode(const std::string_view& body);

    WordNode(const std::string_view& body, OrExpression& ae);

    std::string_view body;
    std::optional<OrExpression> expr;
};

struct AndNode {
    AndNode() = default;

    AndNode(WordExpression& l);

    AndNode(WordExpression& l, AndExpression& r);

    WordExpression left;
    std::optional<AndExpression> right;
};

struct OrNode {
    OrNode() = default;

    OrNode(AndExpression& l);

    OrNode(AndExpression& l, OrExpression& r);

    AndExpression left;
    std::optional<OrExpression> right;
};

class Parser {
public:
    Parser(std::string_view str) : analyzer_(str) {};

    std::expected<OrExpression, ParseError> ParseExpression();

private:
    std::expected<WordExpression, ParseError> ParseWord();

    std::expected<AndExpression, ParseError> ParseAnd();

    std::expected<OrExpression, ParseError> ParseOr();

    LexAnalyzer analyzer_;
};

void Inorder(OrExpression& expr, std::set<std::string>& tokens);

void Inorder(AndExpression& expr, std::set<std::string>& tokens);

void Inorder(WordExpression& expr, std::set<std::string>& tokens);

