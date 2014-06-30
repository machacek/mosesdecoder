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
  : StatisticsBasedScorer("SEGRANKS", config),
    segranks_scorer(NULL)
{
    char* path_to_segranks = "/home/mmachace/diplomka/segranks";
    
    Py_SetProgramName("SegranksScorer");
    Py_Initialize();
    
    PyObject* sys_module_name = PyString_FromString("sys");
    PyObject* sys_module = PyImport_Import(sys_module_name);
    Py_DECREF(sys_module_name);
    PyObject* path_object = PyObject_GetAttrString(sys_module, "path")
    
    PyObject* segranks_module_name = PyString_FromString("segranks");
    PyObject* segranks_module = PyImport_Import(segranks_module_name);
    Py_DECREF(segranks_module_name);


    if (module == NULL) {
        cerr << "Cannot import module" << endl;
        return;
    }
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
