//
//  processor.cpp
//  Tendency
//
//  Created by Suneeth Torke on 10/21/20.
//

#include <stdio.h>
#include "base.hpp"
#include <iomanip>


int main(int argc, const char * argv[]) {
    // insert code here...
    
    std::cout.precision(9);
    
    vector<data> statistics(100);
    source corpus("/Users/torke/Jar/Tendency/Tendency/corpus.txt", 100);
    //corpus
    
    // curzan, schlissel, varsity
    source curzan("/Users/torke/Jar/Tendency/Tendency/curzan.txt", -1);
    source schlissel("/Users/torke/Jar/Tendency/Tendency/schlissel.txt", -1);
    source varsity("/Users/torke/Jar/Tendency/Tendency/varsity.txt", -1);
    
    corpus.calculate_presence(corpus.freq);
    curzan.calculate_presence(corpus.freq);
    schlissel.calculate_presence(corpus.freq);
    varsity.calculate_presence(corpus.freq);
    
    vector<source> sources;
    sources.push_back(curzan);
    sources.push_back(schlissel);
    sources.push_back(varsity);
    
    // generate the presence
    const size_t source_count = sources.size();
    
    // calculate mean
    for (size_t m = 0; m < 100; ++m) {
        double sum = 0;
        for (size_t n = 0; n < source_count; ++n) {
            sum += sources[n].presence[m].second;
        }
        statistics[m].mean = sum / source_count;
    }
    // calculate stdev
    for (size_t i = 0; i < 100; ++i) {
        double stdev = 0;
        for (size_t j = 0; j < source_count; ++j) {
            double diff = sources[j].presence[i].second - statistics[i].mean;
            stdev = diff * diff;
        }
        stdev /= (source_count - 1);
        stdev = sqrt(stdev);
        statistics[i].stdev = stdev;
    }
    
    for (size_t z = 0; z < 100; ++z) {
        size_t ccount = corpus.word_count;
        statistics[z].zscore = ((corpus.freq[z].second / ccount) - statistics[z].mean) / statistics[z].stdev;
        cout << "Test Case z-score for feature [" + corpus.freq[z].first + "] is: " + to_string(statistics[z].zscore) << endl;
    }
    
    for (int i = 0; i < 100; ++i) {
        //cout << corpus.freq[i].second / corpus.word_count << endl;
        //cout << statistics[i].mean << " " << statistics[i].stdev << endl;
    }

    
    
    //curzan.print_p();
    //corpus.print();
    
    std::cout << "Hello, World!\n";
    return 0;
}

