#include "Searcher.h"
#include "IndexHandler.h"
#include <gtest/gtest.h>

#include<sstream>
#include<fstream>
#include <filesystem>

TEST(Index, Creation) {
    std::istringstream ss{"--create K:\\lab11\n--add K:\\texts\n"};
    IndexHandler ind;
    ind.StartProgram(ss);
    std::filesystem::path file_path = "K:\\lab11\\storage";
    ASSERT_TRUE(std::filesystem::is_directory(file_path));
    for (int i = 0; i < 4; i++) {
        auto p = file_path;
        p.append(kBarrelName);
        p.concat(std::to_string(i));
        p.concat(kExtension);
        ASSERT_TRUE(exists(p));
    }
    auto p = file_path;
    p.append(kDocumentNames);
    ASSERT_TRUE(exists(p));
    p =  file_path;
    p.append(kDocumentLength);
    ASSERT_TRUE(exists(p));
}

TEST(Index,CreationAndSaving) {
    std::filesystem::remove_all("K:\\lab11\\storage");
    std::istringstream ss{"--create K:\\lab11\n--add K:\\texts\n--save\n"};
    IndexHandler ind;
    ind.StartProgram(ss);
    std::filesystem::path file_path = "K:\\lab11\\storage";
    ASSERT_TRUE(std::filesystem::is_directory(file_path));
    for (int i = 0; i < 4; i++) {
        auto p = file_path;
        p.append(kBarrelName);
        p.concat(std::to_string(i));
        p.concat(kExtension);
        ASSERT_TRUE(exists(p));
    }
    for (int i = 0; i < 4; i++) {
        auto p = file_path;
        p.append(kFinalName);
        p.concat(std::to_string(i));
        p.concat(kExtension);
        ASSERT_TRUE(exists(p));
    }
    for (int i = 0; i < 4; i++) {
        auto p = file_path;
        p.append(kSkipName);
        p.concat(std::to_string(i));
        p.concat(kExtension);
        ASSERT_TRUE(exists(p));
    }
    auto p = file_path;
    p.append(kDocumentNames);
    ASSERT_TRUE(exists(p));
    p =  file_path;
    p.append(kDocumentLength);
    ASSERT_TRUE(exists(p));
}
TEST(Index,CheckingDocParameters) {
    std::filesystem::remove_all("K:\\lab11\\storage");
    std::istringstream ss{"--create K:\\lab11\n--add K:\\texts\n--save\n"};
    IndexHandler ind;
    ind.StartProgram(ss);
    std::filesystem::path file_path = "K:\\lab11\\storage";
    auto f = file_path;
    f.append(kDocumentLength);
    auto s = file_path;
    s.append(kDocumentNames);
    std::ifstream doc_names(s);
    std::string temp;
    int counter = 0;
    while(std::getline(doc_names,temp))
    {
        counter++;
    }
    ASSERT_EQ(file_size(f),8*counter);
}
TEST(Index,CheckingWordsParameters) {
    std::filesystem::remove_all("K:\\lab11\\storage");
    std::istringstream ss{"--create K:\\lab11\n--add K:\\texts\n--save\n"};
    IndexHandler ind;
    ind.StartProgram(ss);
    std::filesystem::path file_path = "K:\\lab11\\storage";
    auto f = file_path;
    f.append(kDocumentLength);
    auto s = file_path;
    s.append(kDocumentNames);
    std::ifstream doc_names(s);
    std::ifstream doc_length(f);
    std::string temp;
    int counter = 0;
    auto GetValue = [&](size_t counter) {
        doc_length.seekg(counter, std::ios::beg);
        char buffer[8];
        doc_length.read(buffer, 8);
        size_t back = 0;
        for (char i: buffer) {
            back = back << 8;
            back += static_cast<unsigned char>(i);
        }
        return back;
    };
    while(std::getline(doc_names,temp))
    {
        counter++;
    }
    size_t words_counter =0;
    for(auto i = 0;i<counter;++i){
        words_counter+=GetValue(8*i);
    }
    ASSERT_EQ(50,words_counter);
}
TEST(Search,FindOneWord){
    std::istringstream ss{"--info=K:\\lab11\\storage --k=2 --b=1 --expression=(spaghetti AND instead) OR meow --counter=1\n"};
    SearchManger sm;
    sm.StartSimulation(ss);
};
TEST(Search,FindTwoWord){
    std::istringstream ss{"--info=K:\\lab11\\storage --k=2 --b=1 --expression=(spaghetti AND instead) OR meow --counter=2\n"};
    SearchManger sm;
    sm.StartSimulation(ss);
};

