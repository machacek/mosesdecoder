#ifndef MERT_SEGRANKS_SCORER_H_
#define MERT_SEGRANKS_SCORER_H_

//#include <ostream>
#include <string>
#include <vector>

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
  virtual statscore_t calculateScore(const std::vector<int>& comps) const;
  virtual std::size_t NumberOfScores() const {
    return 2;
  }

protected:

  // no copying allowed
  SegranksScorer(const SegranksScorer&);
  SegranksScorer& operator=(const SegranksScorer&);
};

}

#endif  // MERT_SEGRANKS_SCORER_H_
