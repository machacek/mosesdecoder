#include "SegranksScorer.h"

/*
#include <algorithm>
#include <cassert>
#include <cmath>
#include <climits>
#include <fstream>
#include <iostream>
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
  : StatisticsBasedScorer("BLEU", config)
{
}

SegranksScorer::~SegranksScorer() {}

void SegranksScorer::setReferenceFiles(const vector<string>& referenceFiles)
{
}

void SegranksScorer::prepareStats(size_t sid, const string& text, ScoreStats& entry)
{
  // stats for this line
  vector<ScoreStatsType> stats(2);
  string sentence = preprocessSentence(text);
  stats.push_back(5);
  entry.set(stats);
}

statscore_t SegranksScorer::calculateScore(const vector<int>& comps) const
{
  return float(5);
}
}
