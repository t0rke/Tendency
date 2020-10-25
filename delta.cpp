//
//  main.cpp
//  Tendency
//
//  Created by Suneeth Torke on 10/20/20.
//

#include "base.hpp"
#include <iostream>
#include <iostream>
#include <vector>
#include <ostream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <deque>


using namespace std;

// a function to gather the raw input and remove unwanted characters
// a function to then break that raw input into strings of words
// a function to then determine the frequency of each word


string sanitize(string &t) {
    replace(t.begin(), t.end(), '-', ' ');
    replace(t.begin(), t.end(), '/', ' ');
    string j = ".-?!/$^~<>=+\":;{}(),";
    for (char i: j) t.erase(std::remove(t.begin(), t.end(), i), t.end());
    return t;
}

// reads the raw and writes to the clean file
void modify_source() {
    ifstream in("/Users/torke/Jar/Tendency/Tendency/raw.txt");
    ofstream out("/Users/torke/Jar/Tendency/Tendency/source.txt");
    if (!in) cerr << "Unable to open file raw.txt " << endl;
    if (!out) cerr << "Unable to open file source.txt " << endl;
    
    string line;
    vector<string> sentences;
    while (getline(in, line)) {
        transform(begin(line), end(line), begin(line), [](char c){return std::tolower(c);});
        if (line.length() > 0) out << (sanitize(line)) << "\n";
    }
    out << "**********"  << endl;
}

vector<string> generate_list(string path) {
    ifstream inf(path);
    string bit;
    vector<string> list;
    stringstream buffer;

    if (!inf) cerr << "Unable to open: " + path << endl;
    
    buffer << inf.rdbuf();
    string temp;
    stringstream ss(buffer.str());
    
    while (ss >> temp) list.emplace_back(temp);
    return list;
}

void process_emails() {
    vector<email> emails;
    ifstream in("/Users/torke/Jar/Tendency/Tendency/source.txt");
    char key;
    in >> key;
    int date;
    in >> date;
    
    stringstream buffer;
    buffer << in.rdbuf();
    vector<string> list;
    string word;
    stringstream ss(buffer.str());
    double count = 0, len = 0;
    
    
    while (ss >> word) {
        if (word != "**********") {
            len += word.length();
            list.emplace_back(word);
            ++count;
        }
        else {
            len = len/count;
            const size_t list_size = list.size();
            sort(begin(list), end(list));
            deque<pair<string, int>> freq;
            
            string root = list.front();
            int dupes = 0;
            for (size_t i = 0; i < list_size; ++i) {
                if (root == list[i]) ++dupes;
                else {
                    freq.push_back({root, dupes});
                    root = list[i];
                    dupes = 1;
                }
            }
            sort(begin(freq), end(freq), compare());
            freq.resize(20);
            
            emails.push_back({key, freq, len, 0});
            
            for (int i = 0; i < emails.size(); ++i) {
                cout << emails[i].profile << endl;
                cout << emails[i].words[0].first << endl; // " "
                cout << emails[i].avg_len << endl;
                cout << emails[i].sentiment << endl;
                //cout << "bvssy" << endl;
            }
            
            ss >> key;
            ss >> date;
        }
        
    }
    // calculates the average string len
}

string remove_extra_whitespaces(const string &input) {
    string output;
    unique_copy (input.begin(), input.end(), back_insert_iterator<string>(output),
    [](char a, char b){return isspace(a) && isspace(b);});
    return output;
}

void update_source() {
    ifstream in("/Users/torke/Jar/Tendency/Tendency/varsity.txt");
    ofstream out("/Users/torke/Jar/Tendency/Tendency/source.txt");
    
    if (!in) cerr << "Unable to open file raw.txt " << endl;
    if (!out) cerr << "Unable to open file source.txt " << endl;
    
    string line;
    vector<string> sentences;
    while (getline(in, line)) {
        transform(begin(line), end(line), begin(line), [](char c){return std::tolower(c);});
        //line.erase(remove_if(line.begin(), line.end(),
        //[](char c) { return !isalpha(c); } ), line.end());
        if (line.length() > 0) out << (sanitize(line)) << "\n";
    }
}

void preprocess() {
    update_source();
    //modify_source();
    //process_emails();
    
}

