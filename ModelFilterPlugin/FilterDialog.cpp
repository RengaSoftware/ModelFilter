//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FilterDialog.h"
#include "GroupDialog.h"
#include "TypeData.h"

#include "ui_FilterDialog.h"

static const unsigned int c_treeViewColumnsCount = 3;

FilterDialog::FilterDialog(QDialog* parent)
  : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
  , m_operatorData(OperatorData::Instance())
{
  m_pUi.reset(new Ui::FilterDialog());
  m_pUi->setupUi(this);

  m_pUi->lineEdit->selectAll();

  // load treeView
  m_pTreeModel.reset(new QStandardItemModel());
  m_pTreeModel->setColumnCount(c_treeViewColumnsCount);
  m_pUi->treeView->setModel(m_pTreeModel.get());

  // set treeView columns
  QHeaderView* pHeader = m_pUi->treeView->header();
  pHeader->setSelectionMode(QAbstractItemView::SingleSelection);
  const std::vector<QString> headerLabels = {
    QApplication::translate("header", "property"),
    QApplication::translate("header", "operator"),
    QApplication::translate("header", "value")
  };
  for (size_t i = 0; i < c_treeViewColumnsCount; i++)
  {
    m_pTreeModel->setHeaderData(i, Qt::Orientation::Horizontal, QVariant(headerLabels[i]));
    pHeader->setSectionResizeMode(pHeader->logicalIndex(i), QHeaderView::ResizeMode::Interactive);
  }

  // set buttons
  connect(m_pUi->addButton, SIGNAL(clicked()), this, SLOT(onAddButton()));
  connect(m_pUi->editButton, SIGNAL(clicked()), this, SLOT(onEditButton()));
  connect(m_pUi->deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteButton()));

  enableButtons(false);
}

FilterDialog::FilterDialog(QDialog * parent, const FilterData& filterData)
  : FilterDialog(parent)
{
  mData = filterData;
  setWindowTitle(QApplication::translate("FilterDialog", "EditWindowName"));
  m_pUi->lineEdit->setText(filterData.m_filterName);

  // set data in tree
  for (auto& groupData : filterData.m_groupList)
  {
    TypeDataArray typeDataArray;
    QStandardItem* groupItem = new QStandardItem(typeDataArray.getObjectTypeName(groupData.m_groupType));
    for (auto& propertyData : groupData.m_propertyList)
    {
      groupItem->appendRow({
        new QStandardItem(propertyData.m_property.m_propertyName),
        new QStandardItem(m_operatorData->getOperatorName(propertyData.m_operatorType)),
        new QStandardItem(propertyData.m_value)
      });
    }
    m_pTreeModel->appendRow(groupItem);
  }
  m_pUi->treeView->setCurrentIndex(m_pTreeModel->index(0, 0));

  // expand tree
  for (int i = 0; i < m_pTreeModel->rowCount(); i++)
    m_pUi->treeView->expand(m_pTreeModel->index(i, 0));

  // set buttons
  m_pUi->okButton->setEnabled(true);
  m_pUi->editButton->setEnabled(true);
  m_pUi->deleteButton->setEnabled(true);
}

FilterDialog::~FilterDialog()
{}

FilterData FilterDialog::getFilterDescription()
{
  mData.m_filterName = m_pUi->lineEdit->text();
  return mData;
}

void FilterDialog::onAddButton()
{
  std::unique_ptr<GroupDialog> pGroupDialog(new GroupDialog(this));
  int exitValue = pGroupDialog->exec();
  if (exitValue != QDialog::Accepted)
    return;

  // store GroupData
  GroupData groupData = pGroupDialog->getGroupDescription();
  mData.m_groupList.push_back(groupData);

  // add group to the tree
  QStandardItem* groupItem = buildGroupItem(groupData);
  m_pTreeModel->appendRow(groupItem);

  // expand tree and select new group
  setGroupItem(groupItem);

  // set buttons
  enableButtons(true);
}

void FilterDialog::onEditButton()
{
  // get GroupData of selected group
  QStandardItem* pGroupItem = getCurrentGroupItem();
  const int itemRow = pGroupItem->row();
  assert(itemRow < mData.m_groupList.size());
  auto groupDataIt = findGroupData(itemRow);

  // show Dialog
  std::unique_ptr<GroupDialog> pGroupDialog(new GroupDialog(this, *groupDataIt));
  int exitValue = pGroupDialog->exec();
  if (exitValue != QDialog::Accepted)
    return;

  // store new GroupData
  *groupDataIt = pGroupDialog->getGroupDescription();

  // replace group item in the tree
  QStandardItem* groupItem = buildGroupItem(*groupDataIt);
  m_pTreeModel->removeRow(itemRow);
  m_pTreeModel->insertRow(itemRow, groupItem);

  // expand tree and select modified group
  setGroupItem(groupItem);

  // set buttons
  enableButtons(true);
}

void FilterDialog::onDeleteButton()
{
  // remove group from tree
  QModelIndex parentIndex = m_pTreeModel->indexFromItem(getCurrentGroupItem());
  const int parentRow = parentIndex.row();
  m_pTreeModel->removeRow(parentRow);

  // remove GroupData
  auto it = findGroupData(parentRow);
  mData.m_groupList.erase(it);

  // set buttons
  enableButtons(m_pTreeModel->rowCount() > 0);
  if (m_pTreeModel->rowCount() > 0)
  {
    m_pUi->treeView->setCurrentIndex(m_pTreeModel->index(std::min(parentRow, m_pTreeModel->rowCount() - 1), 0));
  }
}

QStandardItem * FilterDialog::getCurrentGroupItem()
{
  QStandardItem* pItem = m_pTreeModel->itemFromIndex(m_pUi->treeView->currentIndex());
  if (pItem->parent() != nullptr)
  {
    // child item was selected
    pItem = pItem->parent();
  }
  else
  {
    if (pItem->rowCount() == 0)
    {
      // selected item has the same row, but column != 0
      assert(pItem->column() > 0);
      pItem = m_pTreeModel->item(pItem->row(), 0);
    }
  }

  return pItem;
}

QStandardItem* FilterDialog::buildGroupItem(const GroupData& data) {
  // build tree from GroupData
  QStandardItem* groupItem = new QStandardItem();
  TypeDataArray typeDataArray;
  groupItem->setText(typeDataArray.getObjectTypeName(data.m_groupType));

  for (auto property : data.m_propertyList)
  {
    groupItem->appendRow({
      new QStandardItem(property.m_property.m_propertyName),
      new QStandardItem(m_operatorData->getOperatorName(property.m_operatorType)),
      new QStandardItem(property.m_value)
    });
  }

  return groupItem;
}

void FilterDialog::setGroupItem(const QStandardItem* groupItem) {
  const QModelIndex groupItemIndex = m_pTreeModel->indexFromItem(groupItem);
  m_pUi->treeView->expand(groupItemIndex);
  m_pUi->treeView->setCurrentIndex(groupItemIndex);
}

std::list<GroupData>::iterator FilterDialog::findGroupData(const int position) {
  std::list<GroupData>::iterator it = mData.m_groupList.begin();
  std::advance(it, position);
  return it;
}

void FilterDialog::enableButtons(bool isEnable) {
  m_pUi->okButton->setEnabled(isEnable);
  m_pUi->editButton->setEnabled(isEnable);
  m_pUi->deleteButton->setEnabled(isEnable);
}