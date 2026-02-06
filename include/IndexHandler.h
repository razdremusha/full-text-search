#pragma once

#include<map>
#include <string>
#include <set>
#include<cstdint>
#include <sstream>
#include<filesystem>
#include <vector>

static const std::string kBarrelName = "block";
static const std::string kFinalName = "fblock";
static const std::string kSkipName = "skipfile";
static const std::string kExtension = ".dta";
static const std::string kFolderName = "storage";
static const std::string kDocumentLength = "document_data.dta";
static const std::string kDocumentNames = "document_name.dta";

static const std::string kCreationArgument = "--create";
static const std::string kSaveArgument = "--save";
static const std::string kAddArgument = "--add";
static const std::string kExitArgument = "--exit";

static const char kSeparatingSymbols[] = {'g', 'n', 'u', 'z'};

class IndexHandler {
private:
    struct doc_parameters {
        doc_parameters(size_t id_):id(id_){};
        size_t id;
        std::vector<size_t> lines;
    };

    void AddFile(const std::filesystem::path& file);
    void Summarize();
    void WriteInFile(const std::string& token, const std::vector<doc_parameters>& dp,
                            const std::string& file_name);

    template<class T>
    auto SplitToBytes(T argument){
        long long counter = sizeof(T);
        std::vector<char>writing;
        for (long long i = 0; i < counter; i++) {
            char byte = static_cast<char>((argument >> (8 * (counter - i - 1))) & ((1 << 8) - 1));
            writing.push_back(byte);
        }
        return std::string(writing.begin(),writing.end());
    }
    std::filesystem::path file_path;
    std::vector<std::string> names;
    size_t documents_counter = 0;

public:
    void StartProgram(std::istream& stream);
};