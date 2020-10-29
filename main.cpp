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

const bool OUTPUT = true;
const int SOURCE_COUNT = 85;
// TODO: change 'DOCUMENT' to change which document is being analyzed
// The DISPUTED ARTICLES ARE [49, 50, 51, 52, 53, 54, 55, 56, 57]
const int DISPUTED_ESSAY = 50;

// TODO: change the following values, to manipulate the Feature Depth, Interval and number of runs
// maximum feature count
const int MAX_DEPTH = 1000;
const int INTERVAL = 10;
// number of averaged sample runs
const int RUNS = MAX_DEPTH / INTERVAL;
const vector<string> CANDIDATES {{"Alexander Hamilton"}, {"James Madison"},
    {"John Jay"}, {"Disputed Authorship"}};

// strips each essay of punctuation and lowecases all characters
void sanitize(string &str) {
    replace(str.begin(), str.end(), '-', ' ');
    replace(str.begin(), str.end(), '/', ' ');
    replace(str.begin(), str.end(), '\n', ' ');
    string j = ".-?!/$^~<>=+\":;{}(),";
    for (char i: j) str.erase(std::remove(str.begin(), str.end(), i), str.end());
    transform(begin(str), end(str), begin(str), [](char c){return std::tolower(c);});
}

// reads every essay in the provided folder and credits it to the correct author
// output a vectors of authors, whose elements contain each authors subcorpus of the essays
vector<pair<string,string>> prep_data() {
    vector<string> files(SOURCE_COUNT);
    
    // holds the relevant essays seperated by author, doesnt include the disputed essays
    for (const auto & entry :__fs::filesystem::directory_iterator("/Users/torke/Desktop/essays")) files.push_back(entry.path());
    const vector<vector<int>> essays {{1, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 21, 22, 23, 24, 25,
                        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 59, 60, 61, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85}, {10, 18, 19, 20, 37, 38, 39,
                            40, 41, 42, 43, 44, 45, 46, 47, 48, 58}, {2, 3, 4, 5, 64}, {DISPUTED_ESSAY}};
    vector<pair<string,string>> sources(CANDIDATES.size());
    cout << "Prepped Files:" << endl;
    for (int a = 0; a < CANDIDATES.size(); ++a) sources[a] = {CANDIDATES[a], ""};
    for (int i  = 0; i < SOURCE_COUNT; ++i) {
        string filename;
        if (i < 9) filename = "essay0" + to_string(i + 1) + ".txt";
        else filename = "essay" + to_string(i + 1) + ".txt";
        std::ifstream in(filename);
        std::stringstream buffer;
        buffer << in.rdbuf();
        std::string essay = buffer.str();
        sanitize(essay);
        
        for (size_t j = 0; j < sources.size(); ++j) {
            // if the essay number, its adds it to the relevant author's text string
            const auto source_iter = std::find(begin(essays[j]), end(essays[j]), i + 1);
            if (source_iter != end(essays[j])) {
                if (OUTPUT) cout << filename + " -> "  + sources[j].first << endl;
                sources[j].second = sources[j].second + " " + essay;
            }
        }
    }
    cout << "Analysing Document: {" + to_string(DISPUTED_ESSAY) + "}" << endl;
    return sources;
}

// prints the run report after a completed run
const void generate_output (const vector<pair<string,string>> &authors, const vector<double> &deltas) {
    cout << endl;
    cout << "   |The following Results were calculted by slicing every: " + to_string(INTERVAL) + "th possible <fDepth> and" << endl;
    cout << "   |averaging all subcomponenents in "  + to_string(RUNS) + " runs to calculate the Mixed Depth Delta Score." << endl << endl;
    
    for (int l = 0; l < deltas.size(); ++l) {
        cout << "       The MDD Score for [" + authors[l].first + "]: " + to_string(deltas[l] / RUNS) << endl;
    }
    cout << endl;
    // finds the lowest author's delta score
    double min = *min_element(begin(deltas), end(deltas));
    cout << "   |Based on these findings ESSAY " + to_string(DISPUTED_ESSAY) + " was" << endl;
    cout << "   |most likely written by: ";
    for (size_t i = 0; i < deltas.size(); ++i) if (min == deltas[i]) cout << CANDIDATES[i] << endl << endl;;
}

// creates a frequency table of ALL POSSIBLE features
vector<pair<string,int>> generate_features(string &body) {
    vector<pair<string, int>> corpus;
    string word;
    vector<string> list;
    stringstream ss(body);
    
    // splits the string into a vector of strings
    while (ss >> word) list.emplace_back(word);
    
    size_t word_count = list.size();
    // puts duplicate elements size by side
    sort(begin(list), end(list));
        
    string root = list.front();
    int dupes = 0;
    // calculates the frequency of every word by counting duplicate neighbors
    for (size_t i = 0; i < word_count; ++i) {
        if (root == list[i]) ++dupes;
        else {
            corpus.push_back({root, dupes});
            root = list[i];
            dupes = 1;
        }
    }
    // sorts in decending order
    sort(begin(corpus), end(corpus), compare());
    cout << "Created Corpus:" << endl;
    return corpus;
}

int main(int argc, const char * argv[]) {
    string cumulative;
    // preps and generates all of the full texts ONE time
    const vector<pair<string,string>> authors = prep_data();
    for (int i = 0; i < authors.size(); ++i) cumulative += authors[i].second;
    // generates a completed frequency table of features
    const vector<pair<string,int>> features = generate_features(cumulative);
    vector<double> deltas (authors.size() - 1);
    
    for (int i = 1; i < RUNS + 1; ++i) {
        // resizes the feature amount to the prechosen INTERVAl for this run
        vector<pair<string,int>> mirror = features;
        mirror.resize(i * INTERVAL);
        delta temp = delta(mirror, authors, false);
        for (int k = 0; k < temp.sub_delta.size(); ++k) deltas[k] += temp.sub_delta[k];
        cout << "   [RUN " + to_string(i) + "] -> fDepth: " + to_string(i * INTERVAL) << endl;
    }
    // prints the run report
    generate_output(authors, deltas);
     
}

