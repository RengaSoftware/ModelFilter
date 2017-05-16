//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtWidgets/QDialog>
#include <QtGui/QStandardItemModel>

class FilterDialog;
class FilterData;

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

private:
  void enableButtons(bool isEnable);
  void showFilterDialog(FilterDialog* pFilterDialog, QStandardItem* pFilterItem = nullptr);
  void setUniqueName(FilterData& data);

private:
  std::unique_ptr<Ui::MainDialog> m_pUi;
  std::unique_ptr<QStandardItemModel> m_pListModel;
  std::vector<FilterData> m_filterDataArray;
};
