//
//  base.hpp
//  Tendency
//
//  Created by Suneeth Torke on 10/20/20.
//

#ifndef base_hpp
#define base_hpp
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>

struct compare {
    bool operator()(const std::pair<std::string,int> &a, const std::pair<std::string,int> &b) const {
        return (a.second > b.second);
    }
};

struct data {
    double mean = 0, stdev = 0, delta = 0;
};

class source {
public:
    bool analysis = false;
    size_t word_count = 0;
    std::string profile;
    std::vector<double> zscore, presence;
    std::vector<int> standard_freq;
    std::vector<std::pair<std::string,int>> freq;
    
    // builds the default dictionary of words
    source(const std::vector<std::pair<std::string,int>> &corpus, std::string filename) {
        profile = filename;
        std::ifstream in(profile + ".txt");
        if (!in) {
            std::cout << "couldnt open file " + profile << std::endl;
            exit(1);
        }
        std::stringstream buffer;
        buffer << in.rdbuf();
        std::vector<std::string> list;
        std::string word;
        std::stringstream ss(buffer.str());
        
        while (ss >> word) list.emplace_back(word);
        word_count = list.size();
        std::sort(begin(list), end(list));
        
        freq.resize(list.size());
        std::string root = list.front();
        int dupes = 0;
        for (size_t i = 0; i < word_count; ++i) {
            if (root == list[i]) ++dupes;
            else {
                freq[i] = {root, dupes};
                root = list[i];
                dupes = 1;
            }
        }
        sort(begin(freq), end(freq), compare());
        calculate_presence(corpus);
    }
   
    // calculates the precence of word n from corpus in subcorp, calculates resulting mean and the stdev
    void const calculate_presence(const std::vector<std::pair<std::string,int>> &corpus) {
        const size_t corpus_size = corpus.size();
        const size_t freq_size = freq.size();
        presence.resize(corpus_size);
        if (analysis) standard_freq.resize(corpus_size);
        zscore.resize(corpus_size);
        for (size_t i = 0; i < corpus_size; ++i) {
            size_t j = 0;
            while (j < freq_size) {
                if (corpus[i].first.length() == freq[j].first.length()) {
                    if (corpus[i].first == freq[j].first) {
                        //std::cout << freq[j].first << std::endl;
                        //std::cout << (freq[j].second/ (double) word_count << std::endl;
                        presence[i] = {freq[j].second / (double) word_count};
                        if (analysis) standard_freq[i] = freq[j].second;
                        break;
                    }
                }
                ++j;
            }
            // the greater the multiplier the more skew for the 0 values.
            //std::cout << presence[i]<< std::endl;
        }
        //std::cout << "--------------------------" << std::endl;
    }
};

#endif /* base_hpp */
