#pragma once
#include <string>
#include "StatelessFeatureFunction.h"

namespace Moses
{

// Rule Scope - not quite completely implemented yet
class RuleScope : public StatelessFeatureFunction
{
public:
	RuleScope(const std::string &line);

	  virtual bool IsUseable(const FactorMask &mask) const
	  { return true; }

	  virtual void Evaluate(const Phrase &source
							, const TargetPhrase &targetPhrase
							, ScoreComponentCollection &scoreBreakdown
							, ScoreComponentCollection &estimatedFutureScore) const;

	  virtual void Evaluate(const InputType &input
	                         , const InputPath &inputPath
	                         , const TargetPhrase &targetPhrase
	                         , const StackVec *stackVec
	                         , ScoreComponentCollection &scoreBreakdown
	                         , ScoreComponentCollection *estimatedFutureScore = NULL) const
	  {}

	  virtual void Evaluate(const Hypothesis& hypo,
	                        ScoreComponentCollection* accumulator) const
	  {}

	  virtual void EvaluateChart(const ChartHypothesis &hypo,
	                             ScoreComponentCollection* accumulator) const
	  {}

	  void SetParameter(const std::string& key, const std::string& value);

protected:
  bool m_sourceSyntax;
};

}

