#pragma once
#include <QtCore/QDir>

#include "FilterData.h"

class FiltersManager
{
public:
  FiltersManager(const QDir& pluginDataDir);
  size_t count() const;
  const FilterData& filter(size_t index) const;
  void addFilter(const FilterData& filter);
  void updateFilter(size_t index, const FilterData& filter);
  void deleteFilter(size_t index);

private:
  std::vector<FilterData> m_filterDataArray;
  QDir m_pluginDataDir;
};
