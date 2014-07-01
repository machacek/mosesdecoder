#include "SegranksScorer.h"

#include <Python.h>
#include <iostream>
#include <map>
#include <string>

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
    
    /*
     * Adding path to segranks module to sys.path
     */
    PyObject* path_object = PySys_GetObject("path");
    PyObject* path_to_segranks_object = PyString_FromString(path_to_segranks);
    if (PyList_Append(path_object, path_to_segranks_object)) {
        cerr << "Cannot append path to sys.path" << endl;
        return;
    }
    Py_DECREF(path_to_segranks_object);

    /*
     * Importing segrank_scorer module
     */
    PyObject* segranks_module_name = PyString_FromString("segranks.segranks_scorer");
    PyObject* segranks_module = PyImport_Import(segranks_module_name);
    Py_DECREF(segranks_module_name);

    if (segranks_module == NULL) {
        cerr << "Cannot import module" << endl;
        return;
    }

    /*
     * Creating config dictionary
     */
    map<string,string>& configMap = getConfigMap();
    PyObject* configDict = PyDict_New();
    for (map<string,string>::iterator it = configMap.begin(); it != configMap.end(); ++it) {
        const char* key = it->first.c_str();
        PyObject* value = PyString_FromString(it->second.c_str());
        cerr << "Inserting key " << key << " with value " << it->second << endl;
        if (PyDict_SetItemString(configDict, key, value)) {
            cerr << "Cannot insert key " << key << " with value " << it->second << endl;
        }
        Py_DECREF(value);
    }

    /*
     * Creating scorer instance
     */
    PyObject* scorer_class = PyObject_GetAttrString(segranks_module, "SegranksScorer");
    if(scorer_class == NULL) {
        cerr << "Cannot get SegranksScorer class" << endl;
    }
    if(PyType_Check(scorer_class)) {
        cerr << "PyType returned true" << endl;
    } else {
        cerr << "PyType returned false" << endl;
    }
    this->segranks_scorer = PyObject_Call(scorer_class, NULL, NULL);
    if(this->segranks_scorer == NULL) {
        cerr << "Cannot create segranks scorer instance" << endl;
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
    }
    else
    {
        PyObject* repr = PyObject_Repr(this->segranks_scorer);
        cerr << PyString_AsString(repr) << endl;
    }
    Py_DECREF(configDict);
    Py_DECREF(scorer_class);
}

SegranksScorer::~SegranksScorer() {
    Py_DECREF(this->segranks_scorer);
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
