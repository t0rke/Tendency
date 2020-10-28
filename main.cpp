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


const bool output = false;

void trim(string &str) {
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
    trim(str);
    return str;
}

vector<pair<string,string>> prep_data() {
    vector<string> files;
    std::string path = "/Users/torke/Desktop/essays";
    for (const auto & entry : std::__fs::filesystem::directory_iterator(path)) files.push_back(entry.path());
    std::sort(begin(files), end(files));
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
        if (i + 1 == 53) disputed_full.second = essay;
        if (output) cout << filename + " -> "  + disputed_full.first << endl;
        
    }
    cout << "Prepped Files:" << endl;
    return {hamilton_full, madison_full, jay_full, shared_full, disputed_full};
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
    cout << "Created Corpus:" << endl;
    return corpus;
}

int main(int argc, const char * argv[]) {
    const vector<pair<string,string>> authors = prep_data();
    string cumulative;
    for (int i = 0; i < authors.size(); ++i) cumulative += authors[i].second;
    const vector<pair<string,int>> corpus = corpus_freq(cumulative);
    
    double delta_hamilton = 0;
    double delta_madison = 0;
    double delta_jay = 0;
    double delta_shared = 0;
    
    // TODO: change 'interval' to manipulate the number of runs
    int interval = 1;
    int bound = (1000 / interval) + 1;
    cout << "----------------------------------" << endl;
    for (int i = 1; i < bound; ++i) {
        vector<pair<string,int>> mirror = corpus;
        mirror.resize(i);
        delta temp = delta(mirror, authors, false);
        delta_hamilton += temp.sub_delta[0];
        //cout << delta_hamilton << endl;
        delta_madison += temp.sub_delta[1];
        delta_jay += temp.sub_delta[2];
        delta_shared += temp.sub_delta[3];
        cout << "   [Ran " + to_string(i) + "] -> fDepth: " + to_string(i) << endl;
    }
    cout << "----------------------------------" << endl;
    //cout << (delta_hamilton / 100)  << " " << (delta_madison / 100) << " " << (delta_jay / 100) << " " << (delta_shared / 100) << endl;
    
    cout << endl << endl;
    cout << "The following Results were calculted by slicing every: " + to_string(interval) + "th possible" << endl;
    cout << "Feature Depth (fDepth) for a total of: "  + to_string(bound - 1) + " Runs" << endl;
    
    
    cout << "   The Mixed Depth Delta Score for Cand [" + authors[0].first + "] was: " + to_string(delta_hamilton) << endl;
    cout << "   The Mixed Depth Delta Score for Cand [" + authors[1].first + "] was: " + to_string(delta_madison) << endl;
    cout << "   The Mixed Depth Delta Score for Cand [" + authors[2].first + "] was: " + to_string(delta_jay) << endl;
    cout << "   The Mixed Depth Delta Score for Cand [" + authors[3].first + "] was: " + to_string(delta_shared) << endl << endl;
    
    double min  = std::min({ delta_hamilton, delta_madison, delta_jay, delta_shared});
    string suspect;
    if (min == delta_hamilton) suspect = "Alexander Hamilton";
    else if (min == delta_madison) suspect = "James Madison";
    else if (min == delta_jay) suspect = "John Jay";
    else if (min == delta_jay) suspect = "both Alexander Hamilton and James Madison";
    
    cout << "   |Based on these findings the unknown sample was" << endl;
    cout << "   |most likely written by: " + suspect << endl;
     
}

