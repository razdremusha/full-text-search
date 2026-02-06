#include "Searcher.h"
#include "ArgParser.h"
#include "LexParser.h"
#include "IndexHandler.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <map>
#include <iterator>
#include <fstream>


void SearchManger::StartSimulation(std::istream& stream) {
    std::cout << "SearchManager executed. Enter path to indexing folder, numbers of relevant queries and requests\n";
    std::string command_argument;
    auto split_string = [](const std::string& str) -> std::vector<std::string> {
        std::istringstream iss(str);
        return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
    };
    while (getline(stream, command_argument)) {
        if (command_argument.empty())
            continue;
        positions.clear();
        words.clear();
        auto temp_vector = split_string(command_argument);
        ParseCommandArguments(temp_vector);
        temp_vector.clear();
        Parser lex_parser(expression);
        std::set<std::string> tokens;
        auto root = lex_parser.ParseExpression();
        Inorder(*root, tokens);
        for (auto& i: tokens) {
            auto t = GetTokenPosition(i);
            if (t.has_value())
                positions[i] = *t;
        }
        auto collection = GetList(*root);
        auto scores = CalculateScore(collection);
        PrintScore(scores);
    }
}

void SearchManger::ParseCommandArguments(std::vector<std::string>& splitted_string) {
    ArgumentParser::ArgParser parser("searcher");
    parser.AddStringArgument(kLongInfo, "Input path to indexing folder");
    parser.AddIntArgument(kLongCounter, "Specify relevant_number of relevant queries").Default(1);
    parser.AddStringArgument(kLongExpression, "Enter a search expression");
    parser.AddIntArgument(kLongK, "Specify the strength of the effect of repeated terms on relevance").Default(1);
    parser.AddIntArgument(kLongB, "Specify the strength of the effect of document length on relevance.").Default(0);
    parser.AddFlag(kLongExit, "Finish the work");
    for (size_t i = 0; i < splitted_string.size(); ++i) {
        if (splitted_string[i].starts_with("--" + kLongExpression)) {
            size_t index = i;
            while (!splitted_string[i + 1].starts_with("--" + kLongExpression) &&
                   !splitted_string[i + 1].starts_with("--" + kLongExit) &&
                   !splitted_string[i + 1].starts_with("--" + kLongInfo) &&
                   !splitted_string[i + 1].starts_with("--" + kLongCounter) &&
                   !splitted_string[i + 1].starts_with("--" + kLongK)&&
                   !splitted_string[i + 1].starts_with("--" + kLongB))
            {
                splitted_string[index].append(" ");
                splitted_string[index].append(splitted_string[i + 1]);
                ++i;
                if ((i + 1) >= splitted_string.size())
                    break;
            }
            splitted_string.erase(splitted_string.cbegin() + index + 1, splitted_string.cbegin() + i + 1);
            i = index;
        }
    }
    parser.Parse(splitted_string);
    if (parser.GetFlag("exit")) {
        exit(EXIT_SUCCESS);
    }
    folder_path = parser.GetStringValue(kLongInfo);
    relevant_number = parser.GetIntValue(kLongCounter);
    expression = parser.GetStringValue(kLongExpression);
    k_parameter = static_cast<float>(parser.GetIntValue(kLongK));
    b_parameter = static_cast<float>(parser.GetIntValue(kLongB));
}

std::optional<std::pair<size_t, size_t>> SearchManger::GetTokenPosition(const std::string& token) {
    std::pair<size_t, size_t> answer;
    std::filesystem::path skip_file = CreateFileName(token, kSkipName);
    std::filesystem::path data_file = CreateFileName(token, kFinalName);
    const size_t kSizeOfInstruction = 24;
    size_t words_counter = file_size(skip_file) / kSizeOfInstruction;
    std::ifstream skip(skip_file, std::ios::binary | std::ios::in);
    std::ifstream data(data_file, std::ios::binary | std::ios::in);
    auto GetValue = [&](size_t counter) {
        skip.seekg(counter, std::ios::beg);
        char buffer[8];
        skip.read(buffer, 8);
        size_t back = 0;
        for (char i: buffer) {
            back = back << 8;
            back += static_cast<unsigned char>(i);
        }
        return back;
    };
    int l = -1;
    int r = static_cast<int>(words_counter);
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (std::hash<std::string>{}(token) == GetValue(kSizeOfInstruction * m)) {
            answer.first = GetValue(kSizeOfInstruction * m + 8);
            answer.second = GetValue(kSizeOfInstruction * m + 16);
            return answer;
        } else {
            auto pos = GetValue(kSizeOfInstruction * m + 8);
            data.seekg(pos, std::ios::beg);
            std::string word;
            data >> word;
            if (word < token)
                l = m;
            else
                r = m;
        }
    }
    if (std::hash<std::string>{}(token) == GetValue(kSizeOfInstruction * r)) {
        answer.first = GetValue(kSizeOfInstruction * r + 8);
        answer.second = GetValue(kSizeOfInstruction * r + 16);
        return answer;
    }
    return std::nullopt;
}

std::filesystem::path SearchManger::CreateFileName(const std::string& token, const std::string& file_name) {
    std::filesystem::path answer = folder_path;
    answer.append(file_name);
    if (token[0] <= kSeparatingSymbols[0]) {
        answer.concat("0.dta");
    }
    if (kSeparatingSymbols[0] < token[0] && token[0] <= kSeparatingSymbols[1]) {
        answer.concat("1.dta");
    }
    if (kSeparatingSymbols[1] < token[0] && token[0] <= kSeparatingSymbols[2]) {
        answer.concat("2.dta");
    }
    if (kSeparatingSymbols[2] < token[0] && token[0] <= kSeparatingSymbols[3]) {
        answer.concat("3.dta");
    }
    return answer;
}

SearchManger::IdPointer SearchManger::ReadID(std::ifstream& file, size_t adding) {
    auto read_varint = [&]() {
        size_t answer = 0;
        unsigned char byte;
        file >> byte;
        int counter = 0;
        while (!(byte & (1 << 7))) {
            answer += static_cast<size_t>(byte) << static_cast<size_t>(7 * counter);
            file >> byte;
            ++counter;
        }
        byte -= (1 << 7);
        answer += static_cast<size_t>(byte) << (7 * counter);
        return answer;
    };
    IdPointer document(std::make_unique<ID>());
    document->id = read_varint() + adding;
    size_t vector_size = read_varint();
    for (size_t i = 0; i < vector_size; ++i) {
        if (i == 0)
            document->lines.push_back(read_varint());
        else
            document->lines.push_back(read_varint() + document->lines[i - 1]);
    }
    return document;
}

std::vector<size_t> SearchManger::GetList(WordExpression& expr) {
    if (!expr->body.empty()) {
        auto path = CreateFileName(std::string(expr->body), kFinalName);
        std::ifstream file(path, std::ios::binary | std::ios::in);
        file.seekg(positions[expr->body].first, std::ios::beg);
        size_t adding = 0;
        std::vector<size_t> answer;
        std::string word;
        file >> word;
        word.clear();
        char t;
        file >> std::noskipws >> t;
        while (file.tellg() != positions[expr->body].second) {
            auto doc = ReadID(file, adding);
            adding = doc->id;
            answer.push_back(doc->id);
            words[expr->body].push_back(std::move(doc));
        }
        return answer;
    }
    return GetList(*(expr->expr));
}

std::vector<size_t> SearchManger::GetList(AndExpression& expr) {
    std::vector<size_t> answer;
    std::vector<size_t> set_right;
    auto set_left = GetList(expr->left);
    if (expr->right.has_value()) {
        set_right = GetList(*(expr->right));
        std::set_intersection(set_left.begin(), set_left.end(), set_right.begin(), set_left.begin(),
                              std::back_inserter(answer));
        return answer;
    }
    return set_left;
}

std::vector<size_t> SearchManger::GetList(OrExpression& expr) {
    std::vector<size_t> answer;
    std::vector<size_t> set_right;
    auto set_left = GetList(expr->left);
    if (expr->right.has_value()) {
        set_right = GetList(*(expr->right));
        std::set_union(set_left.begin(), set_left.end(), set_right.begin(), set_right.end(),
                       std::back_inserter(answer));
        return answer;
    }
    return set_left;
}

std::set<SearchManger::ScoreData, std::greater<>>
SearchManger::CalculateScore(const std::vector<size_t>& collection) {
    std::set<std::pair<float, size_t>, std::greater<>> out;
    auto GetDocLength = [&](size_t index) {
        std::filesystem::path path = folder_path;
        path.append(kDocumentLength);
        std::ifstream file(path, std::ios::binary | std::ios::in);
        auto GetValue = [&](size_t counter) {
            file.seekg(counter, std::ios::beg);
            char buffer[8];
            file.read(buffer, 8);
            size_t back = 0;
            for (char i: buffer) {
                back = back << 8;
                back += static_cast<unsigned char>(i);
            }
            return back;
        };
        return (GetValue(8 * index));
    };
    auto GetDocAmount = [&]() {
        std::filesystem::path path = folder_path;
        path.append(kDocumentLength);
        auto ans = file_size(path) / 8;
        return ans;
    };
    auto numbers_of_doc = GetDocAmount();
    size_t total_word_sum = 0;
    for (auto k = 0; k < numbers_of_doc; ++k) {
        total_word_sum += GetDocLength(k);
    }
    float average = (float) (total_word_sum) / (float) (numbers_of_doc);
    for (auto& i: collection) {
        float score = 0;
        auto doc_length = GetDocLength(i);
        for (auto& word: positions) {
            float word_score = 0;
            float denominator_ = 0;
            auto token = word.first;
            float sum = 0;
            for (auto& doc: words[token]) {
                sum += static_cast<float>(GetDocLength(doc->id));
                if (doc->id == i) {
                    word_score = static_cast<float>(doc->lines.size());
                    word_score *= static_cast<float>(k_parameter + 1);
                    denominator_ = static_cast<float>(doc->lines.size());
                }
            }
            denominator_ += k_parameter * (1 - b_parameter + b_parameter * ((float) (doc_length) / average));
            word_score /= denominator_;
            auto another_mult = static_cast<float>(GetDocAmount() - (float) (words[token].size()) + 0.5);
            another_mult /= (float) ((float) (words[token].size()) + 0.5);
            word_score *= std::log2(another_mult);
            score += word_score;
        }
        out.insert({score, i});
    }
    return out;
}

void SearchManger::PrintScore(const std::set<ScoreData, std::greater<>>& data) {
    std::cout << "Results:\n";
    size_t sequence_number = 1;
    if (relevant_number == 0) {
        std::cout << "None";
        return;
    }
    auto FindDocName = [&](long long number) {
        std::filesystem::path path = folder_path;
        path.append(kDocumentNames);
        std::ifstream file(path, std::ios::binary | std::ios::in);
        std::string line;
        long long k = -1;
        while (k != number) {
            std::getline(file, line);
            ++k;
        }
        return line;
    };
    auto PrintingOneDocument = [&](size_t i) {
        auto doc_name = FindDocName(i);
        std::cout << sequence_number << ":" << doc_name << "\n";
        std::cout << "\tTerms in this:\n";
        for (auto& j: words) {
            for (auto& item: j.second) {
                if (item->id == i) {
                    std::cout << "\t" << j.first << ":";
                    for (auto& position: item->lines) {
                        std::cout << position << " ";
                    }
                    std::cout << "\n";
                    break;
                }
            }
        }
    };
    for (auto& i: data) {
        PrintingOneDocument(i.second);
        ++sequence_number;
        if (sequence_number > relevant_number)
            break;
    }
}
