# Tendency
Tendency is a C++ program that implements a modification of the [John Burrows Delta-Method](https://pdfs.semanticscholar.org/52b3/85e2d0b098a2dadd279cf1787ea0291a7c95.pdf). The Burrows Delta-Method allows multiple sources from multiple authors to be compared at the same time to determine who wrote a document of unknown authorship. More precisely, Delta measures *how the anonymous text and sets of texts written by an arbitrary number of known authors* all diverge from the average of all of them put together. 
> The Delta Method gives equal weight to every feature that it measures, thus avoiding the problem of common words overwhelming the results, which is an issue with chi-squared tests. 

## Usage
Clone the relevant git files .
Modify the following parts of the code with your relevant source material:
- [x] set the working directory to the provided `samples` folder
- [x] update the `corpus` with the cumulative texts
- [x] create a text file for each author
- [x] create a file for the unknown sample

```c++
    // the shared text body
    corpus_freq("corpus", corpus);

    //text by author
    vector<source> sources {{corpus, "curzan"}, {corpus, "schlissel"},
    {corpus,"varsity"}, {corpus,"unknown"}};
```
1. the `corpus` which contains a lump sum of all of the text inputs
2. the vector of `source` objects which holds each individual source i.e. author
3. the updated files must be located in the provided samples folder to Link correctly

## Interpretation
The program output's a series of `delta scores` which are calculated in the following way:

```
Delta score for cand [curzan] is: 4.409655
Delta score for cand [schlissel] is: 3.554583
Delta score for cand [varsity] is: 4.601074
```
Therefore the calculated result with the smallest `delta score` author with the greatest  probability of writing the sample.

### Authors
>*bvssy* and @t0rke

