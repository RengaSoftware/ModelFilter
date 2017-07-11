//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "FilterData.h"

#include <QtCore/QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QDialog>

#include "ObjectFactory.h"

namespace Ui
{
  class GroupDialog;
}

class GroupDialog : public QDialog
{
  Q_OBJECT

public:
  GroupDialog(QDialog* parent);
  GroupDialog(QDialog* parent, const GroupData& groupData);
  ~GroupDialog();

  GroupData getGroupDescription();

  private slots:
  void onAddButton();
  void onDeleteButton();
  void onTypeBoxIndexChanged(const int changedIndex);
  void onPropertyBoxIndexChanged(const int changedIndex);
  void onOperatorBoxIndexChanged(const int changedIndex);
  void onValueChanged(const QString& value);

private:
  void buildProperties(const GroupData& groupData);
  void setTypeBox(const rengaapi::ObjectType& groupType);
  void itemCountChanged(bool isZero);

  void loadTypeBox();

  void loadPropertyBox();
  void reloadPropertyBox();

  void loadOperatorBox();
  void reloadOperatorBox();

  void reloadValueLine(int changedIndex);

private:
  std::unique_ptr<Ui::GroupDialog> m_pUi;

  std::unique_ptr<ObjectProperty> m_pBuilder;

  std::unique_ptr<QStandardItemModel> m_pTreeModel;
  std::unique_ptr<QStandardItemModel> m_pTypeBoxModel;
  std::unique_ptr<QStandardItemModel> m_pPropertyBoxModel;
  std::unique_ptr<QStandardItemModel> m_pOperatorBoxModel;
  std::unique_ptr<QSortFilterProxyModel> m_pTypeBoxSortModel;
  std::unique_ptr<QSortFilterProxyModel> m_pPropertyBoxSortModel;
};
