//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "FilterData.h"

#include <QtWidgets/QDialog>
#include <QtGui/QStandardItemModel>

class GroupDialog;
class GroupData;

namespace Ui
{
  class FilterDialog;
}

class FilterDialog : public QDialog
{
  Q_OBJECT

public:
  FilterDialog(QDialog* parent);
  FilterDialog(QDialog* parent, const FilterData& filterData);
  ~FilterDialog();

  FilterData getFilterDescription();

private slots:
  void onAddButton();
  void onEditButton();
  void onDeleteButton();

private:
  void showGroupDialog(GroupDialog* pGroupDialog, QStandardItem* pParentItem = nullptr);
  QStandardItem* getCurrentGroupItem();

private:
  std::unique_ptr<Ui::FilterDialog> m_pUi;
  std::unique_ptr<QStandardItemModel> m_pTreeModel;
  // TODO: should be FilterData instead of std::vector<GroupData> - this dialog is responsible for FilterData creation
  std::vector<GroupData> m_groupDataArray;
};
