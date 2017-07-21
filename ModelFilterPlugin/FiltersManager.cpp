#include "stdafx.h"
#include "FiltersManager.h"

namespace
{
  void loadFilters(QDir pluginDataDir, std::vector<FilterData>& filters)
  {
    QFileInfoList entryList = pluginDataDir.entryInfoList({ "*.rnf" }, QDir::Files | QDir::Readable);
    // open each .rnf file
    for (auto& fileInfo : entryList)
    {
      std::unique_ptr<QFile> filterFile(new QFile(fileInfo.canonicalFilePath()));
      if (!filterFile->open(QIODevice::ReadOnly | QIODevice::Text))
        continue;

      FilterData filterData = FilterData::importData(filterFile.get());
      if (!filterData.isValid())
        continue;

      filters.push_back(filterData);
    }
  }
}

FiltersManager::FiltersManager(const QDir & pluginDataDir)
  : m_pluginDataDir(pluginDataDir)
{
  loadFilters(m_pluginDataDir, m_filterDataArray);
}

size_t FiltersManager::count() const
{
  return m_filterDataArray.size();
}

const FilterData & FiltersManager::filter(size_t index) const
{
  return m_filterDataArray.at(index);
}

void FiltersManager::addFilter(const FilterData & filter)
{
  m_filterDataArray.push_back(filter);
}

void FiltersManager::updateFilter(size_t index, const FilterData & filter)
{
  m_filterDataArray[index] = filter;
}

void FiltersManager::deleteFilter(size_t index)
{
  m_filterDataArray.erase(m_filterDataArray.begin() + index);
}
