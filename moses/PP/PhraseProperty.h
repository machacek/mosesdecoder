#pragma once

#include <string>
#include <iostream>

namespace Moses
{

/** base class for all phrase properties.
 */
class PhraseProperty
{
public:
  PhraseProperty() : m_value(NULL) {};
  ~PhraseProperty() { if ( m_value != NULL ) delete m_value; };

  virtual void ProcessValue(const std::string &value) { m_value = new std::string(value); };

  virtual const std::string *GetValueString() const { return m_value; };

protected:

  std::string *m_value;  

};

} // namespace Moses

