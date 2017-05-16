#include "stdafx.h"
#include "NameGenerator.h"


CIncrementalNameGenerator::CIncrementalNameGenerator(const std::wstring& preferedName, bool useBraces)
  : m_counter(1)
  , m_preferedName(preferedName)
  , m_useBraces(useBraces)
{
}


std::wstring CIncrementalNameGenerator::generateNext()
{
  if (m_useBraces)
    return m_preferedName + L" (" + std::to_wstring(m_counter++) + L")";
  
  return m_preferedName + L" " + std::to_wstring(m_counter++);
}

