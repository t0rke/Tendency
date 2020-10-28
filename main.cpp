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

size_t PRECIS = 100;
const bool output = false;

void removeSpaces(string &str) {
    int n = str.length();
    int i = 0, j = -1;
    bool spaceFound = false;
  
    while (++j < n && str[j] == ' ');
  
    while (j < n) {
        if (str[j] != ' ') {
            if ((str[j] == '.' || str[j] == ',' || str[j] == '?') && i - 1 >= 0 && str[i - 1] == ' ')
                str[i - 1] = str[j++];
            else str[i++] = str[j++];

            spaceFound = false;
        }
        else if (str[j++] == ' ') {
            if (!spaceFound) {
                str[i++] = ' ';
                spaceFound = true;
            }
        }
    }
    if (i <= 1) str.erase(str.begin() + i, str.end());
    else str.erase(str.begin() + i - 1, str.end());
}

string sanitize(string &str) {
    replace(str.begin(), str.end(), '-', ' ');
    replace(str.begin(), str.end(), '/', ' ');
    replace(str.begin(), str.end(), '\n', ' ');
    string j = ".-?!/$^~<>=+\":;{}(),";
    for (char i: j) str.erase(std::remove(str.begin(), str.end(), i), str.end());
    transform(begin(str), end(str), begin(str), [](char c){return std::tolower(c);});
    removeSpaces(str);
    return str;
}

vector<pair<string,string>> prep_data() {
    vector<string> files;
    std::string path = "/Users/torke/Desktop/essays";
    for (const auto & entry : std::__fs::filesystem::directory_iterator(path)) files.push_back(entry.path());
    std::sort(begin(files), end(files));
    // for (size_t i = 0; i < files.size(); ++i) cout << files[i] << endl;
    
    const vector<int> hamilton = {1, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 21, 22, 23, 24, 25,
                    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 59, 60, 61, 65, 66, 67, 68, 69, 70,
                    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
    const vector<int> madison = {10, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 58};
    const vector<int> jay = {2, 3, 4, 5, 64};
    const vector<int> shared = {18, 19, 20};
    //const vector<int> disputed = {49, 50, 51, 52, 53, 54, 55, 56, 57, 62, 63};
    
    pair<string,string> hamilton_full = {"hamilton", ""}, madison_full = {"madison", ""},
    jay_full = {"jay", ""}, shared_full = {"shared", ""}, disputed_full{"disputed", ""};
    
    for (int i  = 0; i < 85; ++i) {
        string filename = "essay" + to_string(i + 1) + ".txt";
        if (i < 9) {
            filename = "essay0" + to_string(i + 1) + ".txt";
        }
        std::ifstream in(filename);
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();
        std::string essay = buffer.str();
        essay = sanitize(essay);
        // hamilton
        auto ham = std::find(begin(hamilton), end(hamilton), i + 1);
        if (ham != end(hamilton)) {
            if (output) cout << filename + " -> "  + hamilton_full.first << endl;
            hamilton_full.second = hamilton_full.second + " " + essay;
            continue;
        }
        // madison
        auto mad = std::find(begin(madison), end(madison), i + 1);
        if (mad != end(madison)) {
            if (output) cout << filename + " -> "  + madison_full.first << endl;
            madison_full.second = madison_full.second + " " + essay;
            continue;
        }
        // shared
        auto sha = std::find(begin(shared), end(shared), i + 1);
        if (sha != end(shared)) {
            if (output) cout << filename + " -> "  + shared_full.first << endl;
            shared_full.second = shared_full.second + " " + essay;
            continue;
        }
        // jay
        auto ja = std::find(begin(jay), end(jay), i + 1);
        if (ja != end(jay)) {
            if (output) cout << filename + " -> "  + jay_full.first << endl;
            jay_full.second = jay_full.second + " " + essay;
            continue;
        }
        // DISPUTED <<IGNORE>> //
        if (i + 1 == 49) disputed_full.second = essay;
        if (output) cout << filename + " -> "  + disputed_full.first << endl;
        
    }
    // cout << jay_full.second << endl;
    return {hamilton_full, madison_full, jay_full, shared_full, disputed_full};
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

vector<pair<string,int>> corpus_freq(string body) {
    vector<pair<string, int>> corpus;
    string word;
    stringstream ss(body);
    vector<string> list;
    
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
    return corpus;
}

int main(int argc, const char * argv[]) {
    
    
    //update_source();
    cout << std::setprecision(7);
    
    
    const vector<pair<string,string>> authors = prep_data();
    
    
    // curzan, schlissel, varsity
    // calcualtes the freq and the precence automatically
    string cumulative;
    for (int i = 0; i < authors.size(); ++i) cumulative += authors[i].second;
    const vector<pair<string,int>> corpus = corpus_freq(cumulative);
    
    if (output) {
        ofstream out("dummy.txt");
        if (!out) cerr << "Unable to open file source.txt " << endl;
        out << cumulative;
    }
    
    for (int i = 1; i < 101; ++i) {
        vector<pair<string,int>> mirror = corpus;
        mirror.resize(i);
        delta(mirror, authors, false);
    }
    
    /*
    for (int p = 98; p < 100 + 1; ++p) {

        vector<data> statistics(PRECIS);
        corpus_freq(corpus, cumulative);
        for (size_t i = 0; i < corpus.size(); ++i) {
            //cout << corpus[i].second << endl;
        }
        
        vector<source> sources;
        for (size_t i = 0; i < authors.size(); ++i) {
            sources.push_back({corpus, authors[i].first, authors[i].second});
        }

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
                //cout << sources[y].zscore[z] << endl;
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
            // cout << "Delta score for cand [" + sources[y].profile + "] is: " + to_string(delta) << endl;
            cout << delta << ",";
            //cout << "--------------" << endl;
        }
        cout << endl;
        
        for (int i = 0; i < corpus.size(); ++i) {
            //cout << statistics[i].stdev << endl;
            //cout << statistics[i].mean << endl;
            //cout << sources[0].standard_freq[i] <<  endl;
            //cout << (corpus[i].second/ (double) corpus.size()) * 10 << endl;
        }
    }
    */
    return 0;
}

