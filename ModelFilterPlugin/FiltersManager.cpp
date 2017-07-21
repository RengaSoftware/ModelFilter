#include "stdafx.h"
#include "FiltersManager.h"

namespace
{

}

FiltersManager::FiltersManager(const QDir & pluginDataDir)
  : m_pluginDataDir(pluginDataDir)
{
  loadFilters();
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
  saveFilterFile(filter);
}

void FiltersManager::updateFilter(size_t index, const FilterData & filter)
{
  m_filterDataArray[index] = filter;
  saveFilterFile(filter);
}

void FiltersManager::deleteFilter(size_t index)
{
  deleteFilterFile(m_filterDataArray[index]);
  m_filterDataArray.erase(m_filterDataArray.begin() + index);
}

void FiltersManager::loadFilters()
{
  QFileInfoList entryList = m_pluginDataDir.entryInfoList({ "*.rnf" }, QDir::Files | QDir::Readable);
  // open each .rnf file
  for (auto& fileInfo : entryList)
  {
    std::unique_ptr<QFile> filterFile(new QFile(fileInfo.canonicalFilePath()));
    if (!filterFile->open(QIODevice::ReadOnly | QIODevice::Text))
      continue;

    FilterData filterData = FilterData::importData(filterFile.get());
    if (!filterData.isValid())
      continue;

    m_filterDataArray.push_back(filterData);
  }
}

void FiltersManager::deleteFilterFile(const FilterData& data)
{
  QFile filterFile(QString("%1/%2.rnf").arg(m_pluginDataDir.canonicalPath()).arg(data.m_filterName));
  if (filterFile.exists())
    filterFile.remove();
}

void FiltersManager::saveFilterFile(const FilterData& data)
{
  QFile filterFile(QString("%1/%2.rnf").arg(m_pluginDataDir.canonicalPath()).arg(data.m_filterName));
  if (filterFile.open(QIODevice::WriteOnly | QIODevice::Text))
    data.exportData(&filterFile);
  else
    assert(false);
}