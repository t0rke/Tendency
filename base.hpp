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
#include <deque>
#include <sstream>
#include <fstream>

#include <stdio.h>

using namespace std;

struct compare {
    bool operator()(const std::pair<std::string,int> &a, const std::pair<std::string,int> &b) const {
        return (a.second > b.second);
    }
};

struct email {
    std::deque<std::pair<std::string,int>> words;
    double avg_len = 0.0;
    int sentiment = -10;
    char profile = '^';
    
    email(char profile_in, std::deque<std::pair<std::string,int>> words_in, double avg_len_in, int sentiment_in) : profile(profile_in), words(words_in), avg_len(avg_len_in),  sentiment(sentiment_in) {}
};

struct data {
    double mean = 0, stdev = 0, zscore = 0;
    //data (double mean_in, double stdev_in, double zscore_in) :
    //mean(mean_in), stdev(stdev_in), zscore(zscore_in) {}
};

class source {
public:
    size_t word_count = 0;
    //double mean = 0, stdev = 0;
    std::vector<std::pair<int,double>> presence;
    std::vector<std::pair<std::string,int>> freq;
    
    // builds the default dictionary of words
    source(std::string filename, int n) {
        ifstream in(filename);
        if (!in) {
            cout << "couldnt open your file" << endl;
            exit(1);
        }
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();
        vector<string> list;
        string word;
        stringstream ss(buffer.str());
        
        while (ss >> word) list.emplace_back(word);
        
        word_count = list.size();
        sort(begin(list), end(list));
            
        string root = list.front();
        int dupes = 0;
        for (size_t i = 0; i < word_count; ++i) {
            if (root == list[i]) ++dupes;
            else {
                freq.push_back({root, dupes * 1000000});
                root = list[i];
                dupes = 1;
            }
        }
        sort(begin(freq), end(freq), compare());
        if (n > 0) {
            this->freq.resize(n);
        }
        //calculate_presence(corpus);
    }
   
    // calculates the precence of word n from corpus in subcorp, calculates resulting mean and the stdev
    void const calculate_presence(const vector<pair<string,int>> &corpus) {
        const size_t corpus_size = corpus.size();
        double sum = 0, temp = 0;
        for (size_t i = 0; i < corpus_size; ++i) {
            size_t j = 0;
            double sub_freq = 0;
            while (j < word_count) {
                if (corpus[i].first.length() == freq[j].first.length()) {
                    if (corpus[i].first == freq[j].first) {
                        sub_freq = freq[j].second;
                        break;
                    }
                }
                ++j;
            }
//            temp = (sub_freq / (double) word_count);
//            sum += temp;
            presence.push_back({i, sub_freq / (double) word_count});
        }
//        mean = sum / word_count;

//        for (size_t i = 0; i < presence.size(); ++i) {
//            double diff = presence[i].second - mean;
//            //cout << presence[i].second << endl; // << " " << diff << endl;
//            stdev += diff * diff;
//        }
//        stdev /= (n - 1);
//        stdev = sqrt(stdev);
//        cout << mean << " " << stdev << endl;
    }
    
    //void update_source();

    
    
    const void print() const {
        for (int i = 0; i < freq.size(); ++i) {
            cout << freq[i].first << " " << freq[i].second << "\n";
        }
    }
    
    const void print_p() const {
        for (int i = 0; i < presence.size(); ++i) {
            cout << presence[i].first << " " << presence[i].second << "\n";
        }
    }
    
    
};



#endif /* base_hpp */
