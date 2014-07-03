#ifndef MERT_SEGRANKS_SCORER_H_
#define MERT_SEGRANKS_SCORER_H_

//#include <ostream>
#include <string>
#include <vector>
#include <Python.h>

#include "Types.h"
#include "ScoreData.h"
#include "StatisticsBasedScorer.h"
//#include "ScopedVector.h"

namespace MosesTuning
{

/**
 * Bleu scoring
 */
class SegranksScorer: public StatisticsBasedScorer
{
public:
  explicit SegranksScorer(const std::string& config);
  ~SegranksScorer();

  virtual void setReferenceFiles(const std::vector<std::string>& referenceFiles);
  virtual void prepareStats(std::size_t sid, const std::string& text, ScoreStats& entry);
  virtual void prepareStatsVector(std::size_t sid, const std::string& text, std::vector<int>& stats);
  virtual statscore_t calculateScore(const std::vector<int>& comps) const;
  virtual std::size_t NumberOfScores() const { return this->m_number_of_scores; }
  virtual bool useAlignment() const{ return this->m_use_alignment; }

protected:
  SegranksScorer(const SegranksScorer&);
  SegranksScorer& operator=(const SegranksScorer&);

private:
  PyObject* python_scorer;
  bool m_use_alignment;
  int m_number_of_scores;
};

}

#endif  // MERT_SEGRANKS_SCORER_H_
