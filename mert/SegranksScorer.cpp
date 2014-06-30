#include "SegranksScorer.h"

#include <Python.h>
#include <iostream>

/*
#include <algorithm>
#include <cassert>
#include <cmath>
#include <climits>
#include <fstream>
#include <stdexcept>
*/

/*
#include "util/exception.hh"
#include "Ngram.h"
#include "Reference.h"
#include "Util.h"
#include "ScoreDataIterator.h"
#include "FeatureDataIterator.h"
#include "Vocabulary.h"
*/

using namespace std;

namespace MosesTuning
{
SegranksScorer::SegranksScorer(const string& config)
  : StatisticsBasedScorer("SEGRANKS", config)
{
    Py_Initialize();
}

SegranksScorer::~SegranksScorer() {
    Py_Finalize();
}

void SegranksScorer::setReferenceFiles(const vector<string>& referenceFiles)
{
}

void SegranksScorer::prepareStats(size_t sid, const string& text, ScoreStats& entry)
{
  string sentence = this->preprocessSentence(text);
  vector<int> stats;
  prepareStatsVector(sid, sentence, stats);
  entry.set(stats);
}

void SegranksScorer::prepareStatsVector(size_t sid, const string& text, vector<int>& stats)
{
    stats.push_back(1);
    stats.push_back(2);
}

statscore_t SegranksScorer::calculateScore(const vector<int>& comps) const
{
    cout << "comps[0]: " << comps[0] << " comps[1]: " << comps[1] << endl;
    double result = comps[0];
    result /= comps[1];
    return result;
}
}
