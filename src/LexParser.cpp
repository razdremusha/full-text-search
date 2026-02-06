#include <algorithm>
#include "LexParser.h"

WordNode::WordNode(const std::string_view& body) : body(body) {};

WordNode::WordNode(const std::string_view& body, OrExpression& ae) : body(body), expr(std::move(ae)) {};

AndNode::AndNode(WordExpression& l) : left(std::move(l)) {};

AndNode::AndNode(WordExpression& l, AndExpression& r) : left(std::move(l)), right(std::move(r)) {};

OrNode::OrNode(AndExpression& l) : left(std::move(l)) {};

OrNode::OrNode(AndExpression& l, OrExpression& r) : left(std::move(l)), right(std::move(r)) {};

std::expected<WordExpression, ParseError> Parser::ParseWord() {
    if (!analyzer_.GetType())
        return std::unexpected(ParseError::WordError);
    if (analyzer_.GetType() == TokenType::Word) {
        auto cur_token = analyzer_.GetAndContinue();
        return std::make_unique<WordNode>(cur_token->token);
    }
    if (analyzer_.GetType() == TokenType::LeftBracket) {
        analyzer_.GetAndContinue();
        auto m = ParseOr();
        if (analyzer_.GetType() == TokenType::RightBracket) {
            analyzer_.GetAndContinue();
            return std::make_unique<WordNode>("", *m);
        }
    }
    return std::unexpected(ParseError::WordError);
}

std::expected<AndExpression, ParseError> Parser::ParseAnd() {
    auto left = ParseWord();
    if (!left.has_value())
        return std::unexpected(left.error());
    if (analyzer_.GetType() == TokenType::And) {
        analyzer_.GetAndContinue();
        auto right = ParseAnd();
        if (right.has_value())
            return std::make_unique<AndNode>(*left, *right);
    } else
        return std::make_unique<AndNode>(*left);
    return std::unexpected(ParseError::AndError);
}

std::expected<OrExpression, ParseError> Parser::ParseOr() {
    auto left = ParseAnd();
    if (!left.has_value())
        return std::unexpected(left.error());
    if (analyzer_.GetType() == TokenType::Or) {
        analyzer_.GetAndContinue();
        auto right = ParseOr();
        if (right.has_value())
            return std::make_unique<OrNode>(*left, *right);
    } else
        return std::make_unique<OrNode>(*left);
    return std::unexpected(ParseError::OrError);
}

std::expected<OrExpression, ParseError> Parser::ParseExpression() {
    auto temp = ParseOr();
    if (temp.has_value())
        return temp;
    std::cout << "Wrong expression, trouble with: ";
    switch (temp.error()) {
        case ParseError::WordError: {
            std::cout << "word\n";
            break;
        }
        case ParseError::AndError: {
            std::cout << "and form\n";
            break;
        }
        case ParseError::OrError: {
            std::cout << "or form\n";
            break;
        }
    }
    exit(EXIT_FAILURE);
}

void Inorder(WordExpression& expr, std::set<std::string>& tokens){
    if(!expr->body.empty()) {
        std::string added(expr->body.cbegin(),expr->body.end());
        std::transform(added.begin(),added.end(),added.begin(), [](unsigned char c) { return std::tolower(c); });
        tokens.insert(added);
    }
    else
        Inorder(*(expr->expr),tokens);
}

void Inorder(AndExpression& expr, std::set<std::string>& tokens){
    if(expr->left)
        Inorder(expr->left,tokens);
    if(expr->right.has_value())
        Inorder(*(expr->right),tokens);
}

void Inorder(OrExpression& expr, std::set<std::string>& tokens){
    if(expr->left)
        Inorder(expr->left,tokens);
    if(expr->right.has_value())
        Inorder(*(expr->right),tokens);
}
