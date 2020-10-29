# Tendency
Tendency is a C++ program that utilizes the [John Burrows Delta-Method](https://pdfs.semanticscholar.org/52b3/85e2d0b098a2dadd279cf1787ea0291a7c95.pdf) and other methods, to determine the statistical authorship of a body work. The standard Burrows Delta-Method allows multiple sources from multiple authors to be compared at the same time to determine who wrote a document of unknown authorship. More precisely, Delta measures *how the anonymous text and sets of texts written by an arbitrary number of known authors* all diverge from the average of all of them put together. This implementation not only utilizes one feature depth to determine divergence, but rather every feature depth.
> The Delta Method gives equal weight to every feature that it measures, thus avoiding the problem of common words overwhelming the results, which is an issue with chi-squared tests. 

## Methods
When attempting to determine which author wrote an unaccredited or disputed body of work there are mutliple things to consider when attempting to create a baseline for the candidate authors:
- What does this authors work look like?
- How much has this author written about these topics?
- What language styles does this author use?
- What words does this author use?

Out of the following items mentioned above, the main thing I consideres what what this author's work 'looked like'. Now I am not talking about the standard appeareance of what the actual text looked like but rather what words the texts were comprised of.

I chose the analyze the Federalist Papers written by: Alexander Hamilton, James Madison and John Jay.
Out of the 85 essays, essays 49 - 57, 62, 63 were never officially assigned authors, however, Scientists and Mathmaticians [Frederick Mosteller](https://en.wikipedia.org/wiki/Frederick_Mosteller) and David Wallace have accredited all of the disputed author's peices to James Madison using `non-contextual text discriminators` which involve calculating the relative frequency of key words like 'upon' from each author to determine possible authorship.

In my implementation I attempt to use the relative `presence` (relative frequency) of a predefined number of the most frequent elements in the cumulative body of works (sum of all author's works) noted as `corpus` from here onwards. 

##### Definitions
**corpus** - a list of the frequency of every word contained in the cumulative works the provided samples 
**features** - a set of 'key words' chosen based on the frequency of words oberserved *within* the corpus
**authors** - each body of text written by a unique candidate
**presence**- The relative frequency of a `feature` in any authors sup-corpus

##### Processes
The relevant calculations can be reduced the following few steps:
1. Create a set of an arbitrary number of `features` to compare to each author.
2. For each one of the `features` calculate the precense of this feature for each author and the unknown source.
3. This is accomplished by normalizing the appearances by volume relative to the `corpus` (Mean of Means to account for authors have different sized input bodies of work).
4. Calculting the `Mean`, `Sample Standard Deviation` for Each of the `features` in the corpus
5. Using these Feature Means and Stadard Deviations to calculate the standard score for this feature in each authors body of work by subtrating the mean presence (volumetric frequency) from the observed presence and dividing it by the corpus standard deviation for this feature.
6. Subtracting each authors standard score for each feature from the corpus standard score to achieve the `delta` score.
7. Averaging all `delta` scores for each feature by author to achieve the average delta score
8. The author with the lowest delta score is the most likely candidate for authorship

To increase the accuracy of the predictions I chose to extend the aformentioned steps with samples taken and repeated intervals of the possible feature levels. i.e I sampled at 10 features, and then 20 features and then 30 feature and so on up to 100) features or approximatley ten percent of the `corpus`. For every disputed essay, I averaged the results of approximatley 100 trials to determine authorship.
    
## Usage
Clone the relevant git files .
Modify the following parts of the code with your relevant source material:
- [ ] set the working directory to the provided `federalist` folder
- [ ] update the variable DISPUTED_ESSAY to the essay you want to analyze
- [ ] (optional) toggle the various output modes to generate more specific sub-data
- [ ] update the MAX_DEPTH to the maximum number of features you want to analyze
- [ ] update interval to the intervals you want to sample at
- [ ] update candidates to represent the authors you have chosen to compare
The following values are the ones I used for testing.

```c++ 
const bool OUTPUT = false;
const int SOURCE_COUNT = 85;
// TODO: change 'DOCUMENT' to change which document is being analyzed
// The DISPUTED ESSAYS ARE [49, 50, 51, 52, 53, 54, 55, 56, 57, 62, 63]
const int DISPUTED_ESSAY = 50;

// change the following values, to manipulate the Feature Depth, Interval and number of runs
// maximum feature count
const int MAX_DEPTH = 1000;
const int INTERVAL = 10;
// number of averaged sample runs
const int RUNS = MAX_DEPTH / INTERVAL;
const vector<string> CANDIDATES {{"Alexander Hamilton"}, {"James Madison"},
    {"John Jay"}, {"Disputed Authorship"}};
```
To verify that the input documents are being linked correcly set `output` to true to generate the following sample output: 
```c++
essay01.txt -> Alexander Hamilton
essay02.txt -> John Jay
essay03.txt -> John Jay
essay04.txt -> John Jay
essay05.txt -> John Jay
essay06.txt -> Alexander Hamilton
essay07.txt -> Alexander Hamilton
essay08.txt -> Alexander Hamilton
essay09.txt -> Alexander Hamilton
essay10.txt -> James Madison
essay11.txt -> Alexander Hamilton
// ... etc.
```
After each run at a partcular feature depth the program will output the following to let you know how far along it is through the entire run. As the feature depths increase runtime per slice will likely take longer as more values are being compared.

```bash
   [RUN 10] -> fDepth: 100
   [RUN 11] -> fDepth: 110
   [RUN 12] -> fDepth: 120
   [RUN 13] -> fDepth: 130
 ```
 After a completed run the program will output the following information that includes, the number of runs, the average delta value at all feature depths up to your MAX_DEPTH as well as the most likely author. This implementation takes the lowest averaged delta score as the most likley Author Candidate.
 ```c++
   |The following Results were calculted by slicing every: 10th possible <fDepth> and
   |averaging all subcomponenents in 100 runs to calculate the Mixed Depth Delta Score.

       The MDD Score for [Alexander Hamilton]: 3.907033
       The MDD Score for [James Madison]: 3.757792
       The MDD Score for [John Jay]: 3.818030

   |Based on these findings ESSAY 50 was
   |most likely written by: James Madison
 ```
 Jinkies! **James Madison** most likely wrote article 50! 
 For all of the diputed Federalist papers *Tendency* determined that James Madison was the most likely author which is consistent which the Findings of Wallace and Mosteller from 1959.

## Interpretation
The program output's a series of `delta scores` which are calculated in the following way:
```
Delta score for cand [curzan] is: 4.409655
Delta score for cand [schlissel] is: 3.554583
Delta score for cand [varsity] is: 4.601074
```
Therefore the calculated result with the smallest `delta score` author with the greatest  probability of writing the sample.

### Author
>*bvssy* and @t0rke

