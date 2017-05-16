//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FilterDialog.h"
#include "GroupDialog.h"
#include "ObjectPropertyBuilderFactory.h"
#include "TypeData.h"

#include "ui_FilterDialog.h"

static const unsigned int c_treeViewColumnsCount = 3;

FilterDialog::FilterDialog(QDialog* parent)
  : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
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

  // TODO: extract method
  m_pUi->okButton->setEnabled(false);
  m_pUi->editButton->setEnabled(false);
  m_pUi->deleteButton->setEnabled(false);
}

FilterDialog::FilterDialog(QDialog * parent, const FilterData& filterData)
  : FilterDialog(parent)
{
  setWindowTitle(QApplication::translate("FilterDialog", "EditWindowName"));
  m_pUi->lineEdit->setText(filterData.m_filterName);

  // set data in tree
  for (auto& groupData : filterData.m_groupList)
  {
    ObjectPropertyBuilderFactory factory;
    std::shared_ptr<ObjectPropertyBuilder> objectBuilder(factory.createBuilder(groupData.m_groupType));
    TypeDataArray typeDataArray;
    QStandardItem* groupItem = new QStandardItem(typeDataArray.getObjectTypeName(groupData.m_groupType));
    for (auto& propertyData : groupData.m_propertyList)
    {
      groupItem->appendRow({
        new QStandardItem(propertyData.m_property.m_propertyName),
        new QStandardItem(objectBuilder->getOperatorName(propertyData.m_operatorType)) ,
        new QStandardItem(propertyData.m_value)
      });
    }
    m_pTreeModel->appendRow(groupItem);
    m_groupDataArray.push_back(groupData);
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
  // TODO: FilterData - should be the state of dialog, this dialog is responsible for FilterData creation
  FilterData result(m_pUi->lineEdit->text());
  for (auto& groupData : m_groupDataArray)
    result.m_groupList.push_back(groupData);
  return result;
}

void FilterDialog::onAddButton()
{
  std::unique_ptr<GroupDialog> pGroupDialog(new GroupDialog(this));
  showGroupDialog(pGroupDialog.get());
}

void FilterDialog::onEditButton()
{
  QStandardItem* pGroupItem = getCurrentGroupItem();

  // get groupData of selected group
  GroupData groupData = m_groupDataArray[pGroupItem->row()];

  // show GroupDialog
  std::unique_ptr<GroupDialog> pGroupDialog(new GroupDialog(this, groupData));
  showGroupDialog(pGroupDialog.get(), pGroupItem);
}

void FilterDialog::onDeleteButton()
{
  // remove group from tree
  QModelIndex parentIndex = m_pTreeModel->indexFromItem(getCurrentGroupItem());
  const int parentRow = parentIndex.row();
  m_pTreeModel->removeRow(parentRow);

  // remove GroupData
  // TODO: same as in MainDialog - do not synchronize container (m_groupDataArray) with UI
  std::swap(m_groupDataArray[parentRow], m_groupDataArray[m_groupDataArray.size() - 1]);
  m_groupDataArray.pop_back();

  // set buttons
  if (m_pTreeModel->rowCount() == 0)
  {
    m_pUi->okButton->setEnabled(false);
    m_pUi->editButton->setEnabled(false);
    m_pUi->deleteButton->setEnabled(false);
  }
  else
  {
    m_pUi->treeView->setCurrentIndex(m_pTreeModel->index(std::min(parentRow, m_pTreeModel->rowCount() - 1), 0));
  }
}

void FilterDialog::showGroupDialog(GroupDialog* pGroupDialog, QStandardItem* pParentItem)
{
  int exitValue = pGroupDialog->exec();
  if (exitValue == QDialog::Accepted)
  {
    // set group item
    QStandardItem* groupItem = new QStandardItem();
    GroupData groupData = pGroupDialog->getGroupDescription();
    TypeDataArray typeDataArray;
    groupItem->setText(typeDataArray.getObjectTypeName(groupData.m_groupType));

    // build tree from groupData
    ObjectPropertyBuilderFactory factory;
    std::unique_ptr<ObjectPropertyBuilder> pBuilder(factory.createBuilder(groupData.m_groupType));
    for (auto property : groupData.m_propertyList)
    {
      // TODO: extract method
      groupItem->appendRow({
        new QStandardItem(property.m_property.m_propertyName),
        new QStandardItem(pBuilder->getOperatorName(property.m_operatorType)),
        new QStandardItem(property.m_value)
      });
    }

    // TODO: move this responsibility out of here, function should just create GroupData, functions above should do all stuff about editing/adding
    // add group to the tree
    if (pParentItem != nullptr)
    {
      const int parentItemRow = m_pTreeModel->indexFromItem(pParentItem).row();
      m_pTreeModel->removeRow(parentItemRow);
      m_pTreeModel->insertRow(parentItemRow, groupItem);
      m_groupDataArray[parentItemRow] = groupData;
    }
    else
    {
      m_pTreeModel->appendRow(groupItem);
      m_groupDataArray.push_back(groupData);
    }

    // TODO: extract method
    // expand tree and select modified group
    const QModelIndex groupItemIndex = m_pTreeModel->indexFromItem(groupItem);
    m_pUi->treeView->expand(groupItemIndex);
    m_pUi->treeView->setCurrentIndex(groupItemIndex);

    // TODO: extract method
    // set buttons
    m_pUi->okButton->setEnabled(true);
    m_pUi->editButton->setEnabled(true);
    m_pUi->deleteButton->setEnabled(true);
  }
  else
  {
    assert(exitValue == QDialog::Rejected);
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
