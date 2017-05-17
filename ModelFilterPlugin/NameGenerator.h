#pragma once

class CIncrementalNameGenerator
{
public:
  CIncrementalNameGenerator(const std::wstring& preferedName, bool useBraces);
  std::wstring generateNext();

private:
  CIncrementalNameGenerator& operator= (const CIncrementalNameGenerator&);

  size_t m_counter;
  std::wstring m_preferedName;
  bool m_useBraces;
};


struct CUniqueNameGenerator
{
  template <typename Container>
  static std::wstring generate(const Container& existingNames, const std::wstring& preferedName, bool useBraces)
  {
    std::wstring newName(preferedName);
    auto duplicateNameIt = std::find_if(existingNames.cbegin(), existingNames.cend(), [newName](typename Container::const_reference ref)
    {
      return ref.second.toStdWString() == newName;
    });
    CIncrementalNameGenerator incrementalNameGenerator(preferedName, useBraces);
    while (duplicateNameIt != existingNames.end())
    {
      newName = incrementalNameGenerator.generateNext();
      duplicateNameIt = std::find_if(existingNames.cbegin(), existingNames.cend(), [newName](typename Container::const_reference ref)
      {
        return ref.second.toStdWString() == newName;
      });
    }
    return newName;
  }
};
