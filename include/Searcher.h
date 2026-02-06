#pragma once

#include "LexParser.h"

#include <iostream>
#include <vector>
#include<map>
#include <fstream>
#include <filesystem>
#include<sstream>

const static std::string kLongCounter = "counter";
const static std::string kLongInfo = "info";
const static std::string kLongExpression = "expression";
const static std::string kLongExit = "exit";
const static std::string kLongK = "k";
const static std::string kLongB = "b";


class SearchManger {
public:
    void StartSimulation(std::istream& stream);


private:
    struct ID {
        size_t id;
        std::vector<size_t> lines;
    };
    using IdPointer = std::unique_ptr<ID>;
    using ScoreData = std::pair<float, size_t>;

    std::set<ScoreData,std::greater<>>
    CalculateScore(const std::vector<size_t>& collection);

    void PrintScore(const std::set<ScoreData,std::greater<>>& data);

    void ParseCommandArguments(std::vector<std::string>& splitted_string);

    std::vector<size_t> GetList(WordExpression& expr);

    std::vector<size_t> GetList(AndExpression& expr);

    std::vector<size_t> GetList(OrExpression& expr);

    std::optional<std::pair<size_t, size_t>> GetTokenPosition(const std::string& token);

    IdPointer ReadID(std::ifstream& file, size_t adding);

    std::filesystem::path CreateFileName(const std::string& token, const std::string& file_name);

    std::string expression;
    std::string folder_path;
    size_t relevant_number;
    float k_parameter;
    float b_parameter;
    std::map<std::string_view, std::pair<size_t, size_t>> positions;
    std::map<std::string_view,std::vector<IdPointer>>words;
};