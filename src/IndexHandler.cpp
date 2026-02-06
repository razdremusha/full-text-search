#include "IndexHandler.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <charconv>
#include <cstring>

void IndexHandler::StartProgram(std::istream& stream) {
    std::cout << "IndexHandler executed. Create or open indexing folder.\n";
    auto SplitStringToVector = [](const std::string& str) -> std::vector<std::string> {
        std::istringstream iss(str);
        return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
    };
    std::string command_argument = "none";
    while (SplitStringToVector(command_argument)[0] != kCreationArgument) {
        getline(stream, command_argument);
    }
    file_path = (SplitStringToVector(command_argument)[1]);
    std::filesystem::path folder_path = file_path;
    folder_path.append(kFolderName);
    if (!exists(folder_path)) {
        std::filesystem::create_directory(folder_path);
        std::cout << "Created. Add files or save information. " << (char) (2) << "\n";
    } else {
        std::cout << "Opened. Add files or save information. " << (char) (2) << "\n";
        std::filesystem::path doc_path = file_path;
        doc_path.append(kFolderName);
        doc_path.append(kDocumentLength);
        documents_counter = file_size(doc_path) / 24;
    }
    for (int i = 0; i < sizeof(kSeparatingSymbols) / sizeof(char); ++i) {
        auto t = folder_path;
        t.append(kBarrelName);
        t += std::to_string(i);
        t += ".dta";
        names.push_back(t.string());
        if (!exists(t))
            std::ofstream ss(t);
    }
    while (getline(stream, command_argument)) {
        auto temp_vector = SplitStringToVector(command_argument);
        if (temp_vector[0] == kCreationArgument) {
            std::cout << "File already created\n";
            continue;
        }
        if (temp_vector[0] == kAddArgument) {
            for (size_t i = 1; i < temp_vector.size(); ++i) {
                AddFile(temp_vector[i]);
                std::cout << "File/files " << temp_vector[i] << " added " << (char) (2) << "\n";
            }
            continue;
        }
        if (temp_vector[0] == kSaveArgument) {
            Summarize();
            std::cout << "File successfully saved in " << file_path.string() << (char) (2) << "\n";
        }
        if (temp_vector[0] == kExitArgument) {
            exit(EXIT_SUCCESS);
        }
    }
}

void IndexHandler::Summarize() {
    std::map<std::string, std::vector<doc_parameters>> inverted_indexes;
    auto split_string = [](const std::string& str) -> std::vector<std::string> {
        std::istringstream iss(str);
        return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
    };
    for (size_t i = 0; i < names.size(); ++i) {
        std::string& name = names[i];
        std::fstream file(name, std::ios_base::binary | std::ios_base::in);
        std::string word;
        while (std::getline(file, word)) {
            auto line_data = split_string(word);
            size_t doc_id, line;
            std::from_chars(line_data[0].data(), line_data[0].data() + line_data[0].size(), doc_id, 10);
            std::from_chars(line_data[2].data(), line_data[2].data() + line_data[2].size(), line, 10);
            if (inverted_indexes[line_data[1]].empty() || inverted_indexes[line_data[1]].back().id != doc_id) {
                inverted_indexes[line_data[1]].emplace_back(doc_id);
            }
            inverted_indexes[line_data[1]].back().lines.push_back(line);
        }
        for (auto& u: inverted_indexes) {
            std::filesystem::path fn(name);
            std::string file_input = kFinalName + std::to_string(i) + kExtension;
            fn.replace_filename(file_input);
            WriteInFile(u.first, u.second, fn.string());
        }
        inverted_indexes.clear();
    }
}

void IndexHandler::AddFile(const std::filesystem::path& file) {
    auto indexing_file = [&](const std::filesystem::path& path) {
        std::ifstream file(path, std::ios_base::binary | std::ios_base::in);
        std::string word;
        size_t lines_counter = 0;
        size_t words_counter = 0;
        while (file >> word) {
            char* c_str = word.data();
            auto delim_punct = "\t!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
            char* pch = strtok(c_str, delim_punct);
            while (pch != nullptr) {
                ++words_counter;
                std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) -> unsigned char {
                    if (std::ispunct(c))
                        return '\0';
                    return std::tolower(c);
                });
                std::ofstream out;
                if (pch[0] <= kSeparatingSymbols[0]) {
                    out = std::ofstream(names[0], std::ios_base::app);
                }
                if (kSeparatingSymbols[0] < pch[0] && pch[0] <= kSeparatingSymbols[1]) {
                    out = std::ofstream(names[1], std::ios_base::app);
                }
                if (kSeparatingSymbols[1] < pch[0] && pch[0] <= kSeparatingSymbols[2]) {
                    out = std::ofstream(names[2], std::ios_base::app);
                }
                if (kSeparatingSymbols[2] < pch[0] && pch[0] <= kSeparatingSymbols[3]) {
                    out = std::ofstream(names[3], std::ios_base::app);
                }
                out << std::to_string(documents_counter) << ' ' << pch << ' ' << std::to_string(lines_counter)
                    << "\n";
                pch = strtok(nullptr, delim_punct);
            }
            char ch;
            do {

                if (file >> std::noskipws >> ch) {
                    if (ch == '\n') {
                        ++lines_counter;
                    }
                } else
                    break;
            } while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
            file.seekg(-1, std::ios_base::cur);

        }
        documents_counter++;
        std::filesystem::path document_path(names[0]);
        document_path.replace_filename(kDocumentLength);
        std::ofstream document_file(document_path, std::ios_base::binary | std::ios_base::app);
        document_file.write(SplitToBytes(words_counter).c_str(), sizeof(words_counter));
        document_path = (names[0]);
        document_path.replace_filename(kDocumentNames);
        std::string name = path.filename().string() + "\n";
        std::ofstream document_name(document_path, std::ios_base::binary | std::ios_base::app);
        document_name.write(name.c_str(), name.size());
    };
    if (is_directory(file)) {
        for (auto& i: std::filesystem::recursive_directory_iterator(file)) {
            if (!is_directory(i.path())) {
                indexing_file(i.path());
            }
        }
    } else {
        indexing_file(file);
    }
}

void IndexHandler::WriteInFile(const std::string& token, const std::vector<doc_parameters>& dp,
                               const std::string& file_name) {
    auto make_varint = [](size_t number) {
        std::vector<unsigned char> returning_value;
        do {
            unsigned char temp = static_cast<char>(number & 127);
            number = number >> 7;
            if (number == 0) {
                temp += (1 << 7);
            }
            returning_value.push_back(temp);
        } while (number != 0);
        return returning_value;
    };
    std::filesystem::path exist(file_name);
    if (!exists(exist)) {
        std::ofstream file(file_name);
        file.close();
    }
    std::fstream file(file_name, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
    file.seekp(0, std::ios_base::end);
    auto beg = file.tellg();
    std::string in_file = token + ' ';
    for (size_t i = 0; i < dp.size(); ++i) {
        std::vector<unsigned char> temp_vector;
        if (i == 0) {
            temp_vector = make_varint(dp[i].id);
        } else {
            temp_vector = make_varint(dp[i].id - dp[i - 1].id);
        }
        in_file += std::string(temp_vector.begin(), temp_vector.end());
        temp_vector = make_varint(dp[i].lines.size());
        in_file += std::string(temp_vector.begin(), temp_vector.end());
        for (size_t j = 0; j < dp[i].lines.size(); ++j) {
            if (j == 0) {
                temp_vector = make_varint(dp[i].lines[j]);
            } else {
                temp_vector = make_varint(dp[i].lines[j] - dp[i].lines[j - 1]);
            }
            in_file += std::string(temp_vector.begin(), temp_vector.end());
        }
    }
    file.write(in_file.c_str(), in_file.size());
    auto end = file.tellg();
    std::filesystem::path skip_path(file_name);
    std::string skip_list_name = kSkipName + file_name[file_name.size() - 5] + kExtension;
    skip_path.replace_filename(skip_list_name);
    std::ofstream skip_file(skip_path, std::ios_base::binary | std::ios_base::app);
    skip_file.write(SplitToBytes(std::hash<std::string>{}(token)).c_str(), sizeof(size_t));
    skip_file.write(SplitToBytes(beg).c_str(), sizeof(size_t));
    skip_file.write(SplitToBytes(end).c_str(), sizeof(size_t));
    skip_file.close();
    file.close();
}

