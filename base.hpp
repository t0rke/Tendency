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

using namespace std;

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
    source(const std::vector<std::pair<std::string,int>> &corpus, std::string filename, std::string body) {
        profile = filename;
        std::vector<std::string> list;
        std::string word;
        std::stringstream ss(body);
        
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
            if (analysis) std::cout << presence[i]<< std::endl;
        }
        if (analysis) std::cout << "--------------------------" << std::endl;
    }
};


class delta {
public:
    vector<data> statistics;
    vector<source> sources;
    size_t PRECIS = 0;
    int upper_bound = 0;
    
    void calculate_mean() {
        for (size_t i = 0; i < PRECIS; ++i) {
            double sum = 0;
            for (size_t j = 0; j < upper_bound; ++j) {
                sum += sources[j].presence[i];
            }
            statistics[i].mean = sum / upper_bound;
        }
    }
    
    void calculate_stdev() {
        for (size_t i = 0; i < PRECIS; ++i) {
            double curr_dev = 0;
            for (size_t j = 0; j < upper_bound; ++j) {
                double diff = sources[j].presence[i] - statistics[i].mean;
                diff = diff * diff;
                curr_dev += diff;
            }
            curr_dev /= (upper_bound - 1);
            curr_dev = sqrt(curr_dev);
            statistics[i].stdev = curr_dev;
        }
    }
    
    void calculate_zscore() {
        for (size_t y = 0; y < sources.size(); ++y) {
            for (size_t z = 0; z < PRECIS; ++z) {
                sources[y].zscore[z] = (sources[y].presence[z] - statistics[z].mean) / statistics[z].stdev;
            }
        }
    }
    
    void calculate_delta() {
        for (size_t y = 0; y < upper_bound; ++y) {
            double delta = 0;
            for (size_t z = 0; z < PRECIS; ++z) {
                delta += abs(sources.back().zscore[z] - sources[y].zscore[z]);
            }
            delta /= PRECIS;
            cout << delta << "        ";
            //cout << "--------------" << endl;
        }
        cout << endl;
    }
    
    delta(const vector<pair<string,int>> &corpus, const vector<pair<string, string>> &authors, bool verbose) {
        statistics.resize(corpus.size());
        PRECIS = corpus.size();
        
        // creates the features for each author
        for (size_t i = 0; i < authors.size(); ++i) {
            sources.push_back({corpus, authors[i].first, authors[i].second});
        }
        upper_bound = int(sources.size() - 1);
        
        calculate_mean();
        calculate_stdev();
        calculate_zscore();
        calculate_delta();
    }
    // print things : WORD
};
#endif /* base_hpp */
