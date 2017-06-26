//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtCore/QDir>
#include <QtCore/QItemSelectionModel.h>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QDialog>

#include <RengaAPI/ModelObjectCollection.h>

class FilterDialog;
class FilterData;

typedef std::pair<rengaapi::ObjectIdCollection, rengaapi::ObjectIdCollection> objectIdCollection;

namespace Ui
{
  class MainDialog;
}

class MainDialog : public QDialog
{
  Q_OBJECT

public:
  MainDialog();
  ~MainDialog();

private slots:
  void onAddFilter();
  void onCopyFilter();
  void onEditFilter();
  void onDeleteFilter();
  void onApplyFilter();
  void onExportFilter();
  void onImportFilter();
  void enableButtons(const QItemSelection& selected);

private:
  void loadLocalFilters();
  objectIdCollection collectObjects(const FilterData& data);
  void setObjectsVisibility(const objectIdCollection& idCollection);
  void setUniqueName(FilterData& data);
  void saveFilterFile(FilterData& data);
  void deleteFilterFile(FilterData& data);

private:
  std::unique_ptr<Ui::MainDialog> m_pUi;
  std::unique_ptr<QStandardItemModel> m_pListModel;
  std::vector<FilterData> m_filterDataArray;
  QDir pluginDataDir;
};
