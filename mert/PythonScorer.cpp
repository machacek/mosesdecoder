#include "PythonScorer.h"

#include <Python.h>
#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>
#include "Util.h"

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
#include "ScoreDataIterator.h"
#include "FeatureDataIterator.h"
#include "Vocabulary.h"
*/

using namespace std;

namespace MosesTuning
{

void check_for(const char* message) {
    if (PyErr_Occurred()){
        PyErr_Print();
        cerr << message << endl;
        exit(EXIT_FAILURE);
    }
}

PyObject* to_python_unicode(const string& s) {
    PyObject* python_string = PyString_FromString(s.c_str());
    PyObject* python_unicode = PyObject_CallMethod(python_string, "decode", "(s)", "utf-8");
    Py_DECREF(python_string);
    return python_unicode;
}


PythonScorer::PythonScorer(const string& config)
  : StatisticsBasedScorer("SEGRANKS", config),
    python_scorer(NULL),
    m_number_of_scores(0),
    m_use_alignment(false)
{
    const string path = getConfig("path","");
    const string module_name = getConfig("module","");
    
    Py_SetProgramName("PythonScorer");
    Py_Initialize();
    
    /*
     * Adding path to python module to sys.path
     */
    PyObject* path_object = PySys_GetObject("path");
    PyObject* path_to_add_object = PyString_FromString(path.c_str());
    PyList_Append(path_object, path_to_add_object);
    check_for("Cannot append to sys.path");


    /*
     * Importing python module
     */
    PyObject* python_module_name = PyString_FromString(module_name.c_str());
    PyObject* module = PyImport_Import(python_module_name);
    check_for("Cannot import python module");


    /*
     * Creating config dictionary
     */
    map<string,string>& configMap = getConfigMap();
    PyObject* configDict = PyDict_New();
    for (map<string,string>::iterator it = configMap.begin(); it != configMap.end(); ++it) {
        const char* key = it->first.c_str();
        PyObject* value = PyString_FromString(it->second.c_str());
        PyDict_SetItemString(configDict, key, value);
        Py_DECREF(value);
    }

    /*
     * Creating scorer instance
     */
    PyObject* scorer_class = PyObject_GetAttrString(module, "Scorer");
    check_for("Cannot get Scorer class");
    PyObject* emptyTuple = PyTuple_New(0);
    this->python_scorer = PyObject_Call(scorer_class, emptyTuple, configDict);
    check_for("Cannot create Scorer instance");

    /*
     * Caching number of scores
     */
    PyObject* number_of_scores = PyObject_CallMethod(this->python_scorer, "number_of_scores", NULL);
    check_for("Python method number_of_scores failed");
    this->m_number_of_scores = (int) PyInt_AsLong(number_of_scores);
    check_for("Python method number_of_scores did not return int");

    /*
     * Caching whther the scorer uses alignment
     */
    PyObject* use_alignment = PyObject_CallMethod(this->python_scorer, "use_alignment", NULL);
    check_for("Python method use_alignment failed");
    this->m_use_alignment = (bool) PyObject_IsTrue(use_alignment);
    check_for("Python method use_alignment did not return bool");

    /*
     * Cleaning
     */
    Py_DECREF(path_to_add_object);
    Py_DECREF(python_module_name);
    Py_DECREF(module);
    Py_DECREF(configDict);
    Py_DECREF(emptyTuple);
    Py_DECREF(scorer_class);
    Py_DECREF(number_of_scores);
    Py_DECREF(use_alignment);
}

PythonScorer::~PythonScorer() {
    if (this->python_scorer) {
        Py_DECREF(this->python_scorer);
    }
    Py_Finalize();
}

void PythonScorer::setReferenceFiles(const vector<string>& referenceFiles)
{
    /*
     * Prepare list of reference files
     */
    int n = referenceFiles.size();
    PyObject* list = PyList_New(n);
    for (int i = 0; i < n; ++i) {
        PyList_SetItem(list, i, PyString_FromString(referenceFiles[i].c_str()));
    }

    PyObject* result = PyObject_CallMethod(this->python_scorer, "set_reference_files", "(O)", list);
    check_for("Python set_reference_files method failed");

    /*
     * Cleanup
     */
    Py_DECREF(list);
    Py_DECREF(result);
}

void PythonScorer::prepareStats(size_t sid, const string& text, ScoreStats& entry)
{
  string sentence = this->preprocessSentence(text);
  vector<int> stats;
  prepareStatsVector(sid, sentence, stats);
  entry.set(stats);
}

void PythonScorer::prepareStatsVector(size_t sid, const string& text, vector<int>& stats)
{
    PyObject* py_stats;
    if (this->useAlignment()) {
        string sentence = "";
        string align = text;
        size_t alignmentData = text.find("|||");
        //Get sentence and alignment parts
        if(alignmentData != string::npos) {
            getNextPound(align,sentence, "|||");
        } else {
            cerr << "Text to prepare stats does not contain |||" << endl;
            exit(EXIT_FAILURE);
        }
        PyObject* sentence_unicode = to_python_unicode(sentence);
        PyObject* align_unicode = to_python_unicode(align);
        py_stats = PyObject_CallMethod(this->python_scorer, "prepare_stats", "iOO", sid, sentence_unicode, align_unicode);
        Py_DECREF(sentence_unicode);
        Py_DECREF(align_unicode);
    } 
    else 
    {
        PyObject* sentence_unicode = to_python_unicode(text);
        py_stats = PyObject_CallMethod(this->python_scorer, "prepare_stats", "iO", sid, sentence_unicode);
        Py_DECREF(sentence_unicode);
    }

    check_for("Python prepare_stats method failed");

    int n = PySequence_Length(py_stats);
    check_for("Python prepare_stats method did not return sequence");

    for (int i = 0; i < n; ++i) {
        PyObject* item = PySequence_GetItem(py_stats, i);
        int stat = PyInt_AsLong(item);
        check_for("Python prepare_stats method has to return integer sequence");
        stats.push_back(stat);
        Py_DECREF(item);
    }

    /*
     * Cleaning
     */
    Py_DECREF(py_stats);
}

statscore_t PythonScorer::calculateScore(const vector<int>& comps) const
{
    /*
     * Prepare list of components
     */
    int n = comps.size();
    PyObject* comps_list = PyList_New(n);
    for (int i = 0; i < n; ++i) {
        PyList_SetItem(comps_list, i, PyInt_FromLong(comps[i]));
    }

    PyObject* score = PyObject_CallMethod(this->python_scorer, "calculate_score", "(O)", comps_list);
    check_for("Python calculate_score method failed");

    double result = PyFloat_AsDouble(score);
    check_for("Python calculate_score method did not return float");

    /*
     * Cleanup
     */
    Py_DECREF(comps_list);
    Py_DECREF(score);

    return result;
}

}
