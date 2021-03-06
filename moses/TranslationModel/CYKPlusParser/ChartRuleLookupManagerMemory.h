/***********************************************************************
  Moses - factored phrase-based language decoder
  Copyright (C) 2011 University of Edinburgh

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***********************************************************************/

#pragma once
#ifndef moses_ChartRuleLookupManagerMemory_h
#define moses_ChartRuleLookupManagerMemory_h

#include <vector>

#include "ChartRuleLookupManagerCYKPlus.h"
#include "CompletedRuleCollection.h"
#include "moses/NonTerminal.h"
#include "moses/TranslationModel/PhraseDictionaryMemory.h"
#include "moses/TranslationModel/PhraseDictionaryNodeMemory.h"
#include "moses/StackVec.h"

namespace Moses
{

class ChartParserCallback;
class WordsRange;

//! Implementation of ChartRuleLookupManager for in-memory rule tables.
class ChartRuleLookupManagerMemory : public ChartRuleLookupManagerCYKPlus
{
public:
  ChartRuleLookupManagerMemory(const ChartParser &parser,
                               const ChartCellCollectionBase &cellColl,
                               const PhraseDictionaryMemory &ruleTable);

  ~ChartRuleLookupManagerMemory() {};

  virtual void GetChartRuleCollection(
    const WordsRange &range,
    size_t lastPos, // last position to consider if using lookahead
    ChartParserCallback &outColl);

private:

void GetTerminalExtension(
    const PhraseDictionaryNodeMemory *node,
    size_t pos);

void GetNonTerminalExtension(
    const PhraseDictionaryNodeMemory *node,
    size_t startPos,
    size_t endPos);

  void AddAndExtend(
    const PhraseDictionaryNodeMemory *node,
    size_t endPos,
    const ChartCellLabel *cellLabel);

  const PhraseDictionaryMemory &m_ruleTable;

  // permissible soft nonterminal matches (target side)
  bool m_isSoftMatching;
  const std::vector<std::vector<Word> >& m_softMatchingMap;

  // temporary storage of completed rules (one collection per end position; all rules collected consecutively start from the same position)
  std::vector<CompletedRuleCollection> m_completedRules;

  size_t m_lastPos;
  size_t m_unaryPos;

  StackVec m_stackVec;
  ChartParserCallback* m_outColl;

};

}  // namespace Moses

#endif
