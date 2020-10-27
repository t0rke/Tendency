//
//  processor.cpp
//  Tendency
//
//  Created by Suneeth Torke on 10/21/20.
//

#include <stdio.h>
#include "base.hpp"
#include <iomanip>

using namespace std;

const size_t PRECIS = 100;

string sanitize(string &t) {
    replace(t.begin(), t.end(), '-', ' ');
    replace(t.begin(), t.end(), '/', ' ');
    string j = ".-?!/$^~<>=+\":;{}(),";
    for (char i: j) t.erase(std::remove(t.begin(), t.end(), i), t.end());
    return t;
}

void update_source() {
    ifstream in("hamiltonandmadison.txt");
    ofstream out("ytemp.txt");
    
    if (!in) cerr << "Unable to open file unknown.txt " << endl;
    if (!out) cerr << "Unable to open file source.txt " << endl;
    
    string line;
    vector<string> sentences;
    while (getline(in, line)) {
        transform(begin(line), end(line), begin(line), [](char c){return std::tolower(c);});
        //line.erase(remove_if(line.begin(), line.end(),
        //[](char c) { return !isalpha(c); } ), line.end());
        if (line.length() > 0) out << (sanitize(line)) << "\n";
    }
    exit(1);
}

void corpus_freq(string filename, vector<pair<string, int>> &corpus) {
    ifstream in(filename + ".txt");
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
    
    size_t word_count = list.size();
    sort(begin(list), end(list));
        
    string root = list.front();
    int dupes = 0;
    for (size_t i = 0; i < word_count; ++i) {
        if (root == list[i]) ++dupes;
        else {
            corpus.push_back({root, dupes});
            root = list[i];
            dupes = 1;
        }
    }
    sort(begin(corpus), end(corpus), compare());
    corpus.resize(PRECIS);
}

int main(int argc, const char * argv[]) {
    //update_source();
    cout << std::setprecision(7);
    
    vector<data> statistics(PRECIS);
    vector<pair<string,int>> corpus;
    corpus_freq("corpi", corpus);
    
    // curzan, schlissel, varsity
    // calcualtes the freq and the precence automatically
    vector<source> sources {{corpus, "hamilton"}, {corpus, "madison"},
        {corpus,"jay"}, {corpus,"shared"}, {corpus,"unknown"}};

    
    const size_t source_count = sources.size() - 1;
    
    // calculate mean
    for (size_t m = 0; m < PRECIS; ++m) {
        double sum = 0;
        for (size_t n = 0; n < source_count; ++n) {
            sum += sources[n].presence[m];
        }
        statistics[m].mean = sum / source_count;
    }
    // calculate stdev
    for (size_t i = 0; i < PRECIS; ++i) {
        //vector<double> parts(source_count);
        double curr_dev = 0;
        for (size_t j = 0; j < source_count; ++j) {
            double diff = sources[j].presence[i] - statistics[i].mean;
            diff = diff * diff;
            curr_dev += diff;
        }
    
        curr_dev /= (source_count - 1);
        curr_dev = sqrt(curr_dev);
        statistics[i].stdev = curr_dev;
    }
    
    // calculate zscore for corpus and samples
    for (size_t y = 0; y < sources.size(); ++y) {
        for (size_t z = 0; z < PRECIS; ++z) {
            sources[y].zscore[z] = (sources[y].presence[z] - statistics[z].mean) / statistics[z].stdev;
            //cout << sources[y].zscore[z] << " " << sources[y].presence[z] << " " << statistics[z].mean << " " << statistics[z].stdev << endl;
        }
        //cout << "----------------" << endl;
    }
    
    // links the deltas together
    const size_t last = sources.size() - 1;
    for (size_t y = 0; y < source_count; ++y) {
        double delta = 0;
        for (size_t z = 0; z < PRECIS; ++z) {
            delta += abs(sources[last].zscore[z] - sources[y].zscore[z]);
            //cout << abs(sources[last].zscore[z] - sources[y].zscore[z]) << endl;
        }
        delta /= PRECIS;
        cout << "Delta score for cand [" + sources[y].profile + "] is: " + to_string(delta) << endl;
        //cout << "--------------" << endl;
    }
    
    for (int i = 0; i < corpus.size(); ++i) {
    cout << statistics[i].stdev << endl;
      // cout << sources[4].standard_freq[i] <<  endl;
        //cout << (corpus[i].second/ (double) corpus.size()) * 10 << endl;
    }
    return 0;
}

