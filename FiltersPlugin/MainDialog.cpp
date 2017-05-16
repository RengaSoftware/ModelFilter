//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "MainDialog.h"
#include "FilterDialog.h"
#include "ui_MainDialog.h"
#include "NameGenerator.h"
#include "ObjectPropertyBuilderFactory.h"

#include <QtCore/QFile.h>
#include <QtWidgets/QFileDialog.h>
#include <QtWidgets/QMessageBox.h>

#include <RengaAPI/Application.h>
#include <RengaAPI/LevelView.h>
#include <RengaAPI/Model.h>
#include <RengaAPI/ModelObjectCollection.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/ObjectVisibility.h>
#include <RengaAPI/Project.h>

MainDialog::MainDialog()
  : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
  m_pUi.reset(new Ui::MainDialog());
  m_pUi->setupUi(this);

  // load list view
  m_pListModel.reset(new QStandardItemModel);
  m_pUi->listView->setModel(m_pListModel.get());
  connect(m_pUi->listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onEditFilter()));

  // connect buttons
  connect(m_pUi->addButton, SIGNAL(clicked()), this, SLOT(onAddFilter()));
  connect(m_pUi->copyButton, SIGNAL(clicked()), this, SLOT(onCopyFilter()));
  connect(m_pUi->editButton, SIGNAL(clicked()), this, SLOT(onEditFilter()));
  connect(m_pUi->deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteFilter()));
  connect(m_pUi->exportButton, SIGNAL(clicked()), this, SLOT(onExportFilter()));
  connect(m_pUi->importButton, SIGNAL(clicked()), this, SLOT(onImportFilter()));
  connect(m_pUi->okButton, SIGNAL(clicked()), this, SLOT(onApplyFilter()));
  enableButtons(false);
}

MainDialog::~MainDialog()
{}

void MainDialog::onAddFilter()
{
  std::unique_ptr<FilterDialog> m_pFilterDialog(new FilterDialog(this));
  showFilterDialog(m_pFilterDialog.get());
}

void MainDialog::onEditFilter()
{
  // get filter item
  QModelIndex selectedItemIndex = m_pUi->listView->currentIndex();
  QStandardItem* pFilterItem = m_pListModel->itemFromIndex(selectedItemIndex);
  std::unique_ptr<FilterDialog> pFilterDialog(new FilterDialog(this, m_filterDataArray[selectedItemIndex.row()]));
  showFilterDialog(pFilterDialog.get(), pFilterItem);
}

void MainDialog::onDeleteFilter()
{
  QString filterName = " " + m_pListModel->itemFromIndex(m_pUi->listView->currentIndex())->text() + "?";
  QMessageBox deleteMessageBox(
    QMessageBox::Question,
    QApplication::translate("deleteMessageBox", "title"),
    QApplication::translate("deleteMessageBox", "text").append(filterName),
    QMessageBox::Ok | QMessageBox::Cancel,
    this);
  int result = deleteMessageBox.exec();
  if (result == QMessageBox::Cancel)
    return;

  // get filter item
  QModelIndex filterIndex = m_pUi->listView->currentIndex();
  assert(filterIndex.row() >= 0);

  // remove filter from tree
  const int filterRow = filterIndex.row();
  m_pListModel->removeRow(filterRow);

  // TODO: tooooooooo difficult to understand this removing
  // it is hard and not good to support same order in gui and in m_filterDataArray
  std::swap(m_filterDataArray[filterRow], m_filterDataArray[m_filterDataArray.size() - 1]);
  m_filterDataArray.pop_back();

  enableButtons(m_pListModel->rowCount() > 0);
  if (m_pListModel->rowCount() > 0)
    m_pUi->listView->setCurrentIndex(m_pListModel->index(std::min(filterRow, m_pListModel->rowCount() - 1), 0));
}

void MainDialog::onCopyFilter()
{
  const int filterRow = m_pUi->listView->currentIndex().row();
  FilterData filterCopy = m_filterDataArray[filterRow];
  setUniqueName(filterCopy);
  QStandardItem* copyItem = new QStandardItem(filterCopy.m_filterName);
  m_pListModel->appendRow(copyItem);
  m_filterDataArray.push_back(filterCopy);
}

// TODO: extract methods/responsibilities from here (collecting according filter/applying action(hide/show))
void MainDialog::onApplyFilter()
{
  const int filterRow = m_pUi->listView->currentIndex().row();

  // collect rengaapi::ObjectId collection
  rengaapi::Model rengaProjectModel = rengaapi::Project::model();
  rengaapi::ModelObjectCollection objectCollection = rengaProjectModel.objects();
  rengaapi::ObjectIdCollection matchIdCollection;
  rengaapi::ObjectIdCollection notMatchIdCollection;
  rengaapi::ObjectIdCollection levelIdCollection; // TODO: realy need this?

  // check all model objects
  for (auto pObject : objectCollection)
  {
    if (pObject->type() == rengaapi::ModelObjectTypes::LevelType)
    {
      levelIdCollection.add(pObject->objectId());
      continue;
    }
    // apply all groups on object
    bool isObjectMatchFilter = false;
    for (auto& groupData : m_filterDataArray[filterRow].m_groupList)
    {
      if (groupData.m_groupType != pObject->type())
        continue;

      // apply all properties from group
      bool isObjectMatchGroup = true;
      for (auto& propertyData : groupData.m_propertyList)
      {
        bool isObjectMatchProperty;
        if (propertyData.m_value.length() == 0)
        {
          // empty value from user: means all objects match to this property
          isObjectMatchProperty = true;
        }
        else
        {
          // apply filter
          ObjectPropertyBuilderFactory m_propertyBuilderFactory;
          std::unique_ptr<ObjectPropertyBuilder> pObjectBuilder(m_propertyBuilderFactory.createBuilder(pObject->type()));
          isObjectMatchProperty = pObjectBuilder->isObjectMatchFilter(propertyData, pObject);
        }
        // if object does not match property -> object does not match group
        if (!isObjectMatchProperty)
        {
          isObjectMatchGroup = false;
          break;
        }
      }

      // if object match group -> object match filter
      if (isObjectMatchGroup)
      {
        isObjectMatchFilter = true;
        break;
      }
    }

    // check filter matching
    if (isObjectMatchFilter)
      matchIdCollection.add(pObject->objectId());
    else
      notMatchIdCollection.add(pObject->objectId());
  }

  // show or hide matching objects
  bool isShow = m_pUi->izolateRadioButton->isChecked();

  rengaapi::View* pView = rengaapi::Application::activeView();
  switch (pView->type())
  {
  case rengaapi::ViewType::View3D:
    rengaapi::ObjectVisibility::setVisibleIn3DView(levelIdCollection, true); // TODO: realy need this?
    rengaapi::ObjectVisibility::setVisibleIn3DView(notMatchIdCollection, !isShow);
    rengaapi::ObjectVisibility::setVisibleIn3DView(matchIdCollection, isShow);
    break;
  case rengaapi::ViewType::Level:
  {
    rengaapi::ObjectId levelId = dynamic_cast<rengaapi::LevelView*>(pView)->levelId();
    rengaapi::ObjectVisibility::setVisibleOnLevel(notMatchIdCollection, levelId, !isShow);
    rengaapi::ObjectVisibility::setVisibleOnLevel(matchIdCollection, levelId, isShow);
    break;
  }
  default:
    break;
  }
  return;
}

void MainDialog::onExportFilter()
{
  QFileDialog fileDialog(this, QApplication::translate("MainDialog", "Export filter"), "", "Renga Filter (*.rnf)");
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if (fileDialog.exec() == QDialog::Accepted)
  {
    QString fileName = fileDialog.selectedFiles().first();
    std::unique_ptr<QFile> filterFile(new QFile(fileName));
    if (!filterFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox exportMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("exportMessageBox", "title"),
        QApplication::translate("exportMessageBox", "openError"),
        QMessageBox::Ok,
        this);
      exportMessageBox.exec();
      return;
    }
    m_filterDataArray[m_pUi->listView->currentIndex().row()].exportData(filterFile.get());
  }
}

void MainDialog::onImportFilter()
{
  QFileDialog fileDialog(this, QApplication::translate("MainDialog", "Import filter"), "", "Renga Filter (*.rnf)");
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if (fileDialog.exec() == QDialog::Accepted)
  {
    QString fileName = fileDialog.selectedFiles().first();
    std::unique_ptr<QFile> filterFile(new QFile(fileName));
    if (!filterFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox importMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("importMessageBox", "title"),
        QApplication::translate("importMessageBox", "openError"),
        QMessageBox::Ok,
        this);
      importMessageBox.exec();
      return;
    }
    FilterData filterData = FilterData::importData(filterFile.get());
    if (filterData.m_groupList.size() == 0) {
      QMessageBox importMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("importMessageBox", "title"),
        QApplication::translate("importMessageBox", "parseError"),
        QMessageBox::Ok,
        this);
      importMessageBox.exec();
      return;
    }
    setUniqueName(filterData);
    m_filterDataArray.push_back(filterData);

    QStandardItem* item = new QStandardItem(filterData.m_filterName);
    m_pListModel->appendRow(item);
    m_pUi->listView->setCurrentIndex(m_pListModel->indexFromItem(item));
    enableButtons(true);
  }
}

// TODO: this should be a slot (or observer) for "selection changed" or "current index changed" or smth like that?
void MainDialog::enableButtons(bool isEnable)
{
  m_pUi->copyButton->setEnabled(isEnable);
  m_pUi->editButton->setEnabled(isEnable);
  m_pUi->deleteButton->setEnabled(isEnable);
  m_pUi->exportButton->setEnabled(isEnable);
  m_pUi->okButton->setEnabled(isEnable);
}

//TODO: remove responsibility add/edit from here 
//this function should just open dialog and return result from it.
//Functions above should understand is this an adding or editing and do all stuff with renaming and adding to list
// can be "bool MainDialog::showFilterDialog(FilterDialog* pFilterDialog, FilterData& outFilterData)"
void MainDialog::showFilterDialog(FilterDialog* pFilterDialog, QStandardItem* pFilterItem)
{
  int exitValue = pFilterDialog->exec();
  if (exitValue == QDialog::Accepted)
  {
    FilterData filterData = pFilterDialog->getFilterDescription();
    // TODO: save filter data in QStandardItem, do not use unique names as identifier
    // maybe you can save in the QStandardItem an index in m_filterDataArray as id
    if (pFilterItem != nullptr)
    {
      const int oldFilterRow = m_pListModel->indexFromItem(pFilterItem).row();
      if (pFilterItem->text().compare(filterData.m_filterName) != 0)
      {
        setUniqueName(filterData);
        pFilterItem->setText(filterData.m_filterName);
      }
      m_filterDataArray[oldFilterRow] = filterData;
    }
    else
    {
      setUniqueName(filterData);
      QStandardItem* newFilterItem = new QStandardItem(filterData.m_filterName);
      m_pListModel->appendRow(newFilterItem);
      m_pUi->listView->setCurrentIndex(m_pListModel->indexFromItem(newFilterItem));
      m_filterDataArray.push_back(filterData);
    }
    enableButtons(true);
  }
  else
  {
    assert(exitValue == QDialog::Rejected);
  }
}

void MainDialog::setUniqueName(FilterData& data)
{
  std::map<int, QString> filterNames;
  for (int i = 0; i < m_pListModel->rowCount(); i++)
    filterNames.insert(std::make_pair(i, m_pListModel->item(i)->text()));
  std::wstring uniqueFilterName = CUniqueNameGenerator::generate(filterNames, data.m_filterName.toStdWString(), true);
  data.m_filterName = QString::fromStdWString(uniqueFilterName);
}
